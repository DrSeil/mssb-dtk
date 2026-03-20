"""
symbol_db.py — Parses symbols.txt files and exported_symbols.csv.

Provides:
  - Symbol lookup by name
  - Address resolution (address → symbol)
  - Ghidra name cross-reference (address ↔ Ghidra name)
"""

import re
import csv
from dataclasses import dataclass
from pathlib import Path
from typing import Optional


@dataclass
class SymbolDef:
    name: str
    section: str        # ".text", ".bss", ".data", ".rodata", ".init", etc.
    address: int        # section-relative (REL) or absolute (DOL)
    module: str         # "game", "challenge", "menus", "dol"
    sym_type: str       # "function", "object"
    size: int
    scope: str          # "global", "local"


_SYM_RE = re.compile(
    r'^(\S+)\s*=\s*(\.\w+):0x([0-9a-fA-F]+);\s*//\s*type:(\w+)\s+size:0x([0-9a-fA-F]+)(?:\s+scope:(\w+))?'
)

_MODULES = [
    ("dol",       "config/GYQE01/symbols.txt"),
    ("game",      "config/GYQE01/game/symbols.txt"),
    ("challenge", "config/GYQE01/challenge/symbols.txt"),
    ("menus",     "config/GYQE01/menus/symbols.txt"),
]


class SymbolDB:
    def __init__(self):
        self.by_name: dict[str, SymbolDef] = {}
        # address → symbol; last writer wins for collisions (shouldn't matter in practice)
        self.by_address: dict[int, SymbolDef] = {}
        # Ghidra cross-reference: address (int) → Ghidra name string
        self.ghidra_by_address: dict[int, str] = {}
        # Ghidra name → address
        self.ghidra_by_name: dict[str, int] = {}

    def build(self, project_root: Path) -> None:
        self.by_name.clear()
        self.by_address.clear()
        self.ghidra_by_address.clear()
        self.ghidra_by_name.clear()

        # --- symbols.txt files ---
        for module, rel_path in _MODULES:
            path = project_root / rel_path
            if not path.exists():
                continue
            for line in path.read_text(errors="replace").splitlines():
                m = _SYM_RE.match(line.strip())
                if not m:
                    continue
                name, section, addr_hex, sym_type, size_hex, scope = m.groups()
                sym = SymbolDef(
                    name=name,
                    section=section,
                    address=int(addr_hex, 16),
                    module=module,
                    sym_type=sym_type,
                    size=int(size_hex, 16),
                    scope=scope or "global",
                )
                self.by_name[name] = sym
                # For address lookup, DOL addresses are unique. REL section-relative
                # addresses may collide across modules; store all but last-write wins.
                self.by_address[sym.address] = sym

        # --- exported_symbols.csv (Ghidra export) ---
        csv_path = project_root / "exported_symbols.csv"
        if csv_path.exists():
            with open(csv_path, newline="", errors="replace") as f:
                reader = csv.DictReader(f)
                for row in reader:
                    try:
                        addr = int(row["Location"], 16)
                        name = row["Name"].strip()
                        if name:
                            self.ghidra_by_address[addr] = name
                            self.ghidra_by_name[name] = addr
                    except (ValueError, KeyError):
                        continue

    # --- Queries ---

    def lookup(self, name: str) -> Optional[SymbolDef]:
        return self.by_name.get(name)

    def resolve_address(self, address: int) -> Optional[SymbolDef]:
        return self.by_address.get(address)

    def ghidra_name(self, address: int) -> Optional[str]:
        return self.ghidra_by_address.get(address)

    def ghidra_address(self, name: str) -> Optional[int]:
        return self.ghidra_by_name.get(name)

    @property
    def count(self) -> int:
        return len(self.by_name)
