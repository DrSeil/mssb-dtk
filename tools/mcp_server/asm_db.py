"""
asm_db.py — Single-pass assembly file indexer.

Indexes all .s files under build/GYQE01/ to build:
  - Function registry (name → FuncAsmInfo)
  - Caller graph (func → list of callers)
  - Callee graph (func → list of callees)

Also supports snippet extraction for showing argument setup before a bl call.
"""

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class FuncAsmInfo:
    name: str
    module: str
    asm_file: str
    start_line: int
    lines: list[str]       # raw asm lines (stripped of newline)
    callers: list[str] = field(default_factory=list)
    callees: list[str] = field(default_factory=list)


# Module detection from path
_MODULE_SEGMENTS = [
    ("/game/", "game"),
    ("/challenge/", "challenge"),
    ("/menus/", "menus"),
]


def _detect_module(path_str: str) -> str:
    for segment, module in _MODULE_SEGMENTS:
        if segment in path_str:
            return module
    return "dol"


_FN_RE = re.compile(r"^\s*\.fn\s+(\S+?)(?:,\s*\w+)?\s*$")
_ENDFN_RE = re.compile(r"^\s*\.endfn\s+")
_BL_RE = re.compile(r"\bbl\s+([a-zA-Z_][a-zA-Z0-9_]*)\b")
# Unconditional branch — may be tail call
_B_TAIL_RE = re.compile(r"^\s*/\*[^*]*\*/\s*b\s+([a-zA-Z_][a-zA-Z0-9_]*)\b")


class AsmDB:
    def __init__(self):
        self.func_index: dict[str, FuncAsmInfo] = {}
        # func_name → list[caller_name]
        self.callers_index: dict[str, list[str]] = {}

    def build(self, project_root: Path) -> None:
        self.func_index.clear()
        self.callers_index.clear()

        asm_root = project_root / "build" / "GYQE01"
        asm_files = list(asm_root.rglob("*.s"))

        # Collect tail-call candidates for second-pass resolution
        tail_candidates: list[tuple[str, str]] = []  # (caller, target)

        for asm_file in asm_files:
            self._parse_file(asm_file, tail_candidates)

        # Second pass: promote `b target` to a callee relationship if target is a known function
        for caller_name, target in tail_candidates:
            if target in self.func_index:
                caller_info = self.func_index.get(caller_name)
                if caller_info and target not in caller_info.callees:
                    caller_info.callees.append(target)
                callers = self.callers_index.setdefault(target, [])
                if caller_name not in callers:
                    callers.append(caller_name)

    def _parse_file(self, asm_file: Path, tail_candidates: list) -> None:
        try:
            content = asm_file.read_text(errors="replace")
        except Exception:
            return

        lines = content.splitlines()
        module = _detect_module(str(asm_file))
        rel_path = str(asm_file)

        current_func: Optional[str] = None
        func_start: int = 0
        func_lines: list[str] = []
        func_callees: list[str] = []

        for lineno, line in enumerate(lines, 1):
            fn_m = _FN_RE.match(line)
            if fn_m:
                current_func = fn_m.group(1)
                func_start = lineno
                func_lines = []
                func_callees = []
                continue

            if _ENDFN_RE.match(line):
                if current_func:
                    info = FuncAsmInfo(
                        name=current_func,
                        module=module,
                        asm_file=rel_path,
                        start_line=func_start,
                        lines=func_lines,
                        callees=list(func_callees),
                    )
                    self.func_index[current_func] = info
                current_func = None
                func_lines = []
                func_callees = []
                continue

            if current_func is None:
                continue

            func_lines.append(line)

            # Track bl (direct calls)
            bl_m = _BL_RE.search(line)
            if bl_m:
                target = bl_m.group(1)
                if target not in func_callees:
                    func_callees.append(target)
                callers = self.callers_index.setdefault(target, [])
                if current_func not in callers:
                    callers.append(current_func)
                continue

            # Track potential tail calls (unconditional `b`)
            b_m = _B_TAIL_RE.match(line)
            if b_m:
                target = b_m.group(1)
                # Skip local branch labels (they start with ".L_" or "L_")
                if not (target.startswith(".") or target.startswith("L_")):
                    tail_candidates.append((current_func, target))

    # --- Queries ---

    def get_func(self, name: str) -> Optional[FuncAsmInfo]:
        return self.func_index.get(name)

    def get_callers(self, func_name: str) -> list[str]:
        return self.callers_index.get(func_name, [])

    def get_callees(self, func_name: str) -> list[str]:
        info = self.func_index.get(func_name)
        return info.callees if info else []

    def get_caller_snippet(
        self,
        func_name: str,
        caller_name: str,
        lines_before: int = 12,
        lines_after: int = 3,
    ) -> list[dict]:
        """
        Return snippets of assembly around each `bl func_name` call site
        within caller_name's body. Each snippet is a list of strings with
        the bl line prefixed by '>>> '.
        """
        info = self.func_index.get(caller_name)
        if not info:
            return []

        bl_pattern = re.compile(rf"\bbl\s+{re.escape(func_name)}\b")
        snippets = []

        for i, line in enumerate(info.lines):
            if bl_pattern.search(line):
                start = max(0, i - lines_before)
                end = min(len(info.lines), i + lines_after + 1)
                snippet_lines = list(info.lines[start:end])
                bl_idx = i - start
                snippet_lines[bl_idx] = ">>> " + snippet_lines[bl_idx]
                snippets.append({
                    "caller": caller_name,
                    "module": info.module,
                    "file": info.asm_file,
                    "snippet": snippet_lines,
                })

        return snippets

    @property
    def count(self) -> int:
        return len(self.func_index)
