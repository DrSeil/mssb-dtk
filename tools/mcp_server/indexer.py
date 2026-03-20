"""
indexer.py — Wires all sub-databases into a single ProjectIndex.

Provides:
  - build_all(): full initial index
  - reindex(targets): partial re-index of specific sub-indexes
  - Thread-safe access via RLock
  - Staleness detection via mtime tracking
"""

import threading
import time
import sys
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Optional

from symbol_db import SymbolDB
from struct_db import StructDB
from rodata_db import RotadaDB
from asm_db import AsmDB


@dataclass
class SubIndexStatus:
    name: str
    last_built: Optional[datetime] = None
    entry_count: int = 0
    stale: bool = False
    stale_reason: str = ""

    def to_dict(self) -> dict:
        return {
            "name": self.name,
            "last_built": self.last_built.isoformat() if self.last_built else None,
            "entry_count": self.entry_count,
            "stale": self.stale,
            **({"stale_reason": self.stale_reason} if self.stale_reason else {}),
        }


# Singleton instance — shared between MCP server, watcher, and sidecar
_shared_index: Optional["ProjectIndex"] = None


def get_shared_index() -> Optional["ProjectIndex"]:
    return _shared_index


def set_shared_index(index: "ProjectIndex") -> None:
    global _shared_index
    _shared_index = index


class ProjectIndex:
    def __init__(self, project_root: Path):
        self.project_root = project_root
        self._lock = threading.RLock()
        self._rebuilding = False

        self.symbol_db = SymbolDB()
        self.struct_db = StructDB()
        self.rodata_db = RotadaDB()
        self.asm_db = AsmDB()

        self._status: dict[str, SubIndexStatus] = {
            "symbols":  SubIndexStatus("symbols"),
            "structs":  SubIndexStatus("structs"),
            "rodata":   SubIndexStatus("rodata"),
            "asm":      SubIndexStatus("asm"),
        }

        # Source mtimes at last build (for staleness detection)
        self._source_mtimes: dict[str, dict[Path, float]] = {
            "symbols": {},
            "structs": {},
            "rodata": {},
            "asm": {},
        }

    def build_all(self) -> None:
        """Build all sub-indexes. Called once at startup."""
        _log("Building index...")
        t0 = time.time()

        self._build_symbols()
        self._build_structs()
        self._build_asm_and_rodata()

        elapsed = time.time() - t0
        _log(f"Index built in {elapsed:.1f}s")

    def reindex(self, targets: list[str]) -> dict:
        """Re-index the specified sub-indexes. Thread-safe."""
        if "all" in targets:
            targets = ["symbols", "structs", "asm"]

        with self._lock:
            self._rebuilding = True
            try:
                results = {}
                t0 = time.time()

                if "symbols" in targets:
                    self._build_symbols()
                    results["symbols"] = self._status["symbols"].entry_count

                if "structs" in targets:
                    self._build_structs()
                    results["structs"] = self._status["structs"].entry_count
                    results["externs"] = self.struct_db.extern_count

                if "asm" in targets or "rodata" in targets:
                    self._build_asm_and_rodata()
                    results["asm"] = self._status["asm"].entry_count
                    results["rodata"] = self._status["rodata"].entry_count

                elapsed_ms = int((time.time() - t0) * 1000)
                results["duration_ms"] = elapsed_ms
                results["reindexed"] = targets
                return results
            finally:
                self._rebuilding = False

    def wait_for_rebuild(self) -> None:
        """Block until any in-progress rebuild completes."""
        while self._rebuilding:
            time.sleep(0.05)

    # --- Internal builders ---

    def _build_symbols(self) -> None:
        t0 = time.time()
        _log("Indexing symbols...")
        self.symbol_db.build(self.project_root)
        elapsed = time.time() - t0
        s = self._status["symbols"]
        s.last_built = datetime.now()
        s.entry_count = self.symbol_db.count
        s.stale = False
        s.stale_reason = ""
        _log(f"  symbols: {s.entry_count} entries in {elapsed:.2f}s")

    def _build_structs(self) -> None:
        t0 = time.time()
        _log("Indexing structs and externs...")
        self.struct_db.build(self.project_root)
        elapsed = time.time() - t0
        s = self._status["structs"]
        s.last_built = datetime.now()
        s.entry_count = self.struct_db.count
        s.stale = False
        s.stale_reason = ""
        _log(f"  structs: {s.entry_count} structs, {self.struct_db.extern_count} externs in {elapsed:.2f}s")

    def _build_asm_and_rodata(self) -> None:
        t0 = time.time()
        _log("Indexing asm and rodata...")
        self.asm_db.build(self.project_root)
        asm_elapsed = time.time() - t0
        s_asm = self._status["asm"]
        s_asm.last_built = datetime.now()
        s_asm.entry_count = self.asm_db.count
        s_asm.stale = False
        s_asm.stale_reason = ""
        _log(f"  asm: {s_asm.entry_count} functions in {asm_elapsed:.2f}s")

        t1 = time.time()
        self.rodata_db.build(self.project_root)
        rodata_elapsed = time.time() - t1
        s_rd = self._status["rodata"]
        s_rd.last_built = datetime.now()
        s_rd.entry_count = self.rodata_db.count
        s_rd.stale = False
        s_rd.stale_reason = ""
        _log(f"  rodata: {s_rd.entry_count} entries in {rodata_elapsed:.2f}s")

    # --- Staleness marking (called by watcher) ---

    def mark_stale(self, sub_index: str, reason: str = "") -> None:
        with self._lock:
            if sub_index in self._status:
                self._status[sub_index].stale = True
                self._status[sub_index].stale_reason = reason

    def get_status(self) -> dict:
        with self._lock:
            return {
                "sub_indexes": [s.to_dict() for s in self._status.values()],
                "rebuilding": self._rebuilding,
            }


def _log(msg: str) -> None:
    print(f"[mssb-dtk] {msg}", file=sys.stderr, flush=True)
