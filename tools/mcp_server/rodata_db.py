"""
rodata_db.py — Indexes rodata (and data) labels from .s asm files.

Handles both `.rodata` and `.data` sections (which both contain constants).
Labels appear as:
    .obj lbl_3_rodata_28F8, local
        .float 999.9
    .endobj lbl_3_rodata_28F8

Values can be: .float, .double, .4byte, .8byte, .2byte, .byte
"""

import re
import struct
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class RotadaEntry:
    label: str
    values: list           # list of (directive, raw_bytes)
    source_file: str
    source_line: int

    @property
    def first_raw_hex(self) -> str:
        if self.values:
            return self.values[0][1].hex().upper()
        return ""

    @property
    def as_float(self) -> Optional[float]:
        """Interpret first value as 32-bit float."""
        if self.values:
            directive, raw = self.values[0]
            if len(raw) == 4:
                try:
                    return struct.unpack(">f", raw)[0]
                except Exception:
                    pass
        return None

    @property
    def as_double(self) -> Optional[float]:
        """Interpret first value as 64-bit double."""
        if self.values:
            directive, raw = self.values[0]
            if len(raw) == 8:
                try:
                    return struct.unpack(">d", raw)[0]
                except Exception:
                    pass
        return None

    @property
    def as_int(self) -> Optional[int]:
        """Interpret first value as unsigned integer."""
        if self.values:
            directive, raw = self.values[0]
            return int.from_bytes(raw, "big")
        return None

    def to_dict(self) -> dict:
        result = {
            "label": self.label,
            "source": f"{self.source_file}:{self.source_line}",
            "values": [],
        }
        for directive, raw in self.values:
            entry = {"directive": directive, "raw_hex": raw.hex().upper()}
            if len(raw) == 4:
                try:
                    entry["as_float"] = struct.unpack(">f", raw)[0]
                except Exception:
                    pass
                entry["as_int"] = int.from_bytes(raw, "big")
            elif len(raw) == 8:
                try:
                    entry["as_double"] = struct.unpack(">d", raw)[0]
                except Exception:
                    pass
                entry["as_int"] = int.from_bytes(raw, "big")
            result["values"].append(entry)
        return result


# Regex to parse value directives
_FLOAT_RE = re.compile(r"^\s*\.float\s+(.+)$")
_DOUBLE_RE = re.compile(r"^\s*\.double\s+(.+)$")
_4BYTE_RE = re.compile(r"^\s*\.4byte\s+(0x[0-9a-fA-F]+|\d+)$")
_8BYTE_RE = re.compile(r"^\s*\.8byte\s+(0x[0-9a-fA-F]+|\d+)$")
_2BYTE_RE = re.compile(r"^\s*\.2byte\s+(0x[0-9a-fA-F]+|\d+)$")
_BYTE_RE = re.compile(r"^\s*\.byte\s+(0x[0-9a-fA-F]+|\d+)$")
_OBJ_RE = re.compile(r"^\s*\.obj\s+(\S+?),?\s")
_ENDOBJ_RE = re.compile(r"^\s*\.endobj\s+(\S+)")

# Repeat prefix like "[x16]" before the directive
_REPEAT_RE = re.compile(r"^\s*\[x\d+\]\s*")


def _parse_float_str(s: str) -> Optional[bytes]:
    """Parse a float literal string to 4 big-endian bytes."""
    s = s.strip()
    try:
        val = float(s)
        return struct.pack(">f", val)
    except ValueError:
        return None


def _parse_double_str(s: str) -> Optional[bytes]:
    """Parse a double literal string to 8 big-endian bytes."""
    s = s.strip()
    try:
        val = float(s)
        return struct.pack(">d", val)
    except ValueError:
        return None


def _parse_int_str(s: str, width: int) -> Optional[bytes]:
    """Parse a hex or decimal integer literal to big-endian bytes of given width."""
    s = s.strip()
    try:
        if s.startswith("0x") or s.startswith("0X"):
            val = int(s, 16)
        else:
            val = int(s)
        return val.to_bytes(width, "big")
    except (ValueError, OverflowError):
        return None


def _parse_line(line: str) -> Optional[tuple]:
    """Try to parse a value directive line. Returns (directive, bytes) or None."""
    # Strip repeat prefix like "[x16]"
    line = _REPEAT_RE.sub("", line)

    m = _FLOAT_RE.match(line)
    if m:
        raw = _parse_float_str(m.group(1))
        if raw:
            return (".float", raw)

    m = _DOUBLE_RE.match(line)
    if m:
        raw = _parse_double_str(m.group(1))
        if raw:
            return (".double", raw)

    m = _4BYTE_RE.match(line)
    if m:
        raw = _parse_int_str(m.group(1), 4)
        if raw:
            return (".4byte", raw)

    m = _8BYTE_RE.match(line)
    if m:
        raw = _parse_int_str(m.group(1), 8)
        if raw:
            return (".8byte", raw)

    m = _2BYTE_RE.match(line)
    if m:
        raw = _parse_int_str(m.group(1), 2)
        if raw:
            return (".2byte", raw)

    m = _BYTE_RE.match(line)
    if m:
        raw = _parse_int_str(m.group(1), 1)
        if raw:
            return (".byte", raw)

    return None


class RotadaDB:
    def __init__(self):
        # keyed by label name
        self.by_label: dict[str, RotadaEntry] = {}

    def build(self, project_root: Path) -> None:
        self.by_label.clear()
        asm_root = project_root / "build" / "GYQE01"
        for asm_file in asm_root.rglob("*.s"):
            self._parse_file(asm_file)

    def _parse_file(self, asm_file: Path) -> None:
        try:
            lines = asm_file.read_text(errors="replace").splitlines()
        except Exception:
            return

        rel_path = str(asm_file)
        current_label: Optional[str] = None
        current_values: list = []
        current_start_line: int = 0

        for lineno, line in enumerate(lines, 1):
            obj_m = _OBJ_RE.match(line)
            endobj_m = _ENDOBJ_RE.match(line)

            if obj_m:
                label = obj_m.group(1).rstrip(",")
                current_label = label
                current_values = []
                current_start_line = lineno
                continue

            if endobj_m:
                if current_label and current_values:
                    entry = RotadaEntry(
                        label=current_label,
                        values=current_values,
                        source_file=rel_path,
                        source_line=current_start_line,
                    )
                    self.by_label[current_label] = entry
                current_label = None
                current_values = []
                continue

            if current_label is not None:
                parsed = _parse_line(line)
                if parsed:
                    current_values.append(parsed)

    # --- Queries ---

    def lookup(self, label: str) -> Optional[RotadaEntry]:
        return self.by_label.get(label)

    @property
    def count(self) -> int:
        return len(self.by_label)
