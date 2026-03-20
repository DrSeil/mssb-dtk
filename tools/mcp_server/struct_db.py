"""
struct_db.py — Parses C header files to build a struct/enum/union type registry.

Parses:
  - in_game.h (Ghidra export, 28K lines)
  - include/**/*.h (project headers)

Computes field offsets and struct sizes using Metrowerks CodeWarrior GC/1.3.2
alignment rules.

Also indexes `extern` declarations for global symbol type lookup.
"""

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


# ---------------------------------------------------------------------------
# Primitive sizes (Metrowerks/GC, 32-bit)
# ---------------------------------------------------------------------------
PRIMITIVE_SIZES: dict[str, int] = {
    "u8": 1, "s8": 1, "char": 1, "byte": 1, "bool": 1, "BOOL": 1,
    "undefined": 1, "undefined1": 1,
    "u16": 2, "s16": 2, "short": 2, "ushort": 2, "uint16_t": 2, "undefined2": 2,
    "u32": 4, "s32": 4, "int": 4, "uint": 4, "uint32_t": 4, "long": 4,
    "float": 4, "f32": 4, "undefined4": 4,
    "u64": 8, "s64": 8, "double": 8, "f64": 8, "undefined8": 8,
    "pointer": 4,   # Ghidra typedef for void*
    "void": 0,
}

# All enums are int-sized in Ghidra exports unless otherwise specified
_ENUM_SIZE = 4


@dataclass
class FieldDef:
    name: str
    type_str: str           # raw type string, e.g. "struct ACTBone *"
    base_type: str          # stripped base type name, e.g. "ACTBone"
    is_pointer: bool
    is_array: bool
    array_count: int        # 1 if not array
    offset: int             # byte offset within struct (computed)
    size: int               # byte size (computed)
    comment: str = ""       # original comment if any

    def to_dict(self) -> dict:
        d = {
            "name": self.name,
            "type": self.type_str,
            "offset": self.offset,
            "offset_hex": f"0x{self.offset:X}",
            "size": self.size,
        }
        if self.is_array and self.array_count != 1:
            d["array_count"] = self.array_count
        if self.comment:
            d["comment"] = self.comment
        return d


@dataclass
class StructDef:
    name: str
    source_file: str
    source_line: int
    fields: list[FieldDef] = field(default_factory=list)
    computed_size: int = 0
    alignment: int = 1
    is_union: bool = False

    def to_dict(self, include_fields: bool = True) -> dict:
        d = {
            "name": self.name,
            "computed_size": self.computed_size,
            "computed_size_hex": f"0x{self.computed_size:X}",
            "alignment": self.alignment,
            "source": f"{self.source_file}:{self.source_line}",
        }
        if self.is_union:
            d["is_union"] = True
        if include_fields:
            d["fields"] = [f.to_dict() for f in self.fields]
        return d


@dataclass
class ExternDecl:
    symbol_name: str
    declared_type: str
    is_pointer: bool
    declaration_text: str
    source_file: str


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _strip_qualifiers(t: str) -> str:
    """Remove const, volatile, static, extern from type string."""
    for kw in ("const ", "volatile ", "static ", "extern ", "register ", "signed ", "unsigned "):
        t = t.replace(kw, "")
    return t.strip()


def _extract_base_type(type_str: str) -> tuple[str, bool]:
    """
    Given a raw type string like "struct Foo *" or "int" or "u8[4]",
    return (base_type, is_pointer).
    """
    t = _strip_qualifiers(type_str)
    is_ptr = "*" in t
    t = t.replace("*", "").strip()
    # Remove struct/union/enum keywords
    for kw in ("struct ", "union ", "enum "):
        t = t.replace(kw, "")
    # Remove array dimensions
    t = re.sub(r"\s*\[.*", "", t)
    return t.strip(), is_ptr


_ARRAY_DIM_RE = re.compile(r"\[([^\]]+)\]")
# Only allow safe characters in array dimension expressions
_SAFE_DIM_RE = re.compile(r"^[0-9a-fA-FxX \+\-\*\(\)]+$")


def _eval_dim(expr: str) -> int:
    """Evaluate an array dimension expression. Handles hex and simple arithmetic."""
    expr = expr.strip()
    if not expr:
        return 1
    if not _SAFE_DIM_RE.match(expr):
        return 1
    try:
        result = eval(expr, {"__builtins__": {}})  # noqa: S307
        return max(1, int(result))
    except Exception:
        return 1


def _extract_array_count(field_text: str) -> int:
    """Extract array count from field text. Returns 1 if not array.
    Handles: [16], [0xA50], [0xA82 - 0xA54], [2][4] (product).
    """
    dims = _ARRAY_DIM_RE.findall(field_text)
    if not dims:
        return 1
    result = 1
    for d in dims:
        result *= _eval_dim(d)
    return result


def _field_alignment(size: int) -> int:
    """Metrowerks default alignment: min(field_size, 4)."""
    if size == 0:
        return 1
    return min(size, 4)


def _align_up(offset: int, alignment: int) -> int:
    if alignment <= 1:
        return offset
    return (offset + alignment - 1) & ~(alignment - 1)


# ---------------------------------------------------------------------------
# Raw struct/union text parser
# ---------------------------------------------------------------------------

_STRUCT_START_RE = re.compile(
    r"^\s*(?:typedef\s+)?(?P<kw>struct|union)\s+(?P<name>\w+)?\s*\{"
)
_TYPEDEF_END_RE = re.compile(r"^\s*\}\s*(\w+(?:\s*,\s*\*?\w+)*)\s*;")
_FORWARD_TYPEDEF_RE = re.compile(
    r"^\s*typedef\s+(?:struct|union|enum)\s+\w+\s+(\w+)(?:\s*,\s*\*?(\w+))?\s*;"
)
_ENUM_START_RE = re.compile(r"^\s*(?:typedef\s+)?enum\s+(?:\w+\s*)?\{")
_EXTERN_RE = re.compile(
    r"^\s*extern\s+(?:struct\s+|union\s+|enum\s+)?(\w+)\s*(\*?)\s*(\w+)\s*;"
)

# Handle the E(base_type, ENUM_NAME) macro pattern
_E_MACRO_RE = re.compile(r"\bE\s*\(\s*(\w+)\s*,\s*\w+\s*\)")

# Handle artificial_padding(from, to, type) macro
_ARTPAD_RE = re.compile(r"artificial_padding\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(\w+)\s*\)")


def _expand_macros(line: str) -> str:
    """Expand known macros in a field declaration line."""
    # E(base_type, ENUM_NAME) → base_type
    line = _E_MACRO_RE.sub(lambda m: m.group(1), line)
    return line


def _parse_artificial_padding(line: str) -> Optional[tuple[int, int, str]]:
    """Parse artificial_padding(from, to, type) → (from_val, to_val, type_name)."""
    m = _ARTPAD_RE.search(line)
    if not m:
        return None
    from_s, to_s, type_name = m.group(1), m.group(2), m.group(3)
    try:
        from_v = int(from_s, 16) if from_s.startswith("0x") else int(from_s)
        to_v = int(to_s, 16) if to_s.startswith("0x") else int(to_s)
        return from_v, to_v, type_name
    except ValueError:
        return None


def _extract_comment(line: str) -> str:
    m = re.search(r"/\*(.+?)\*/", line)
    if m:
        return m.group(1).strip()
    m = re.search(r"//(.+)$", line)
    if m:
        return m.group(1).strip()
    return ""


# ---------------------------------------------------------------------------
# Field-line parser
# ---------------------------------------------------------------------------

_FIELD_RE = re.compile(
    r"^\s*"
    r"(?:(?:struct|union|enum)\s+)?"    # optional keyword
    r"(\w+)"                            # base type
    r"(\s*\*+\s*)?"                     # optional pointer stars
    r"\s+(\w+)"                         # field name
    r"(\s*\[[^\]]+\])?"                 # optional array dimension(s)
    r"\s*(?::\s*\d+)?"                  # optional bitfield
    r"\s*;"
)


def _parse_field_line(line: str, type_registry: dict) -> Optional[tuple[str, str, bool, int]]:
    """
    Parse a field declaration line.
    Returns (field_name, base_type, is_pointer, array_count) or None.
    """
    line = _expand_macros(line)

    # Strip comments
    line_no_comment = re.sub(r"/\*.*?\*/|//.*$", "", line).strip()
    if not line_no_comment or line_no_comment in ("{", "}", "};"):
        return None

    m = _FIELD_RE.match(line_no_comment)
    if not m:
        return None

    base_type = m.group(1)
    pointer_part = m.group(2) or ""
    field_name = m.group(3)
    array_part = m.group(4) or ""

    is_pointer = "*" in pointer_part
    array_count = _extract_array_count(array_part) if array_part else 1

    # Skip if base_type looks like a keyword
    if base_type in ("return", "if", "else", "for", "while", "do", "switch", "case",
                     "break", "continue", "typedef", "extern", "static"):
        return None

    return field_name, base_type, is_pointer, array_count


# ---------------------------------------------------------------------------
# StructDB
# ---------------------------------------------------------------------------

class StructDB:
    def __init__(self):
        self.structs: dict[str, StructDef] = {}      # name → StructDef
        self.externs: dict[str, ExternDecl] = {}     # symbol_name → ExternDecl
        # Resolved sizes (cached to avoid repeated recursion)
        self._size_cache: dict[str, int] = {}
        # Set of names currently being resolved (cycle detection)
        self._resolving: set[str] = set()

    def build(self, project_root: Path) -> None:
        self.structs.clear()
        self.externs.clear()
        self._size_cache.clear()
        self._resolving.clear()

        paths: list[Path] = []

        in_game = project_root / "in_game.h"
        if in_game.exists():
            paths.append(in_game)

        include_dir = project_root / "include"
        if include_dir.exists():
            paths.extend(sorted(include_dir.rglob("*.h")))

        for path in paths:
            self._parse_file(path)

        # Compute sizes for all structs
        for name in list(self.structs.keys()):
            self._compute_struct_size(name)

    def _parse_file(self, path: Path) -> None:
        try:
            text = path.read_text(errors="replace")
        except Exception:
            return

        rel_path = str(path)
        lines = text.splitlines()
        i = 0
        n = len(lines)

        while i < n:
            line = lines[i]

            # extern declarations
            ext_m = _EXTERN_RE.match(line)
            if ext_m:
                type_name, star, sym_name = ext_m.group(1), ext_m.group(2), ext_m.group(3)
                # Skip primitive types and void
                if type_name not in ("void",):
                    self.externs[sym_name] = ExternDecl(
                        symbol_name=sym_name,
                        declared_type=type_name,
                        is_pointer=bool(star),
                        declaration_text=line.strip(),
                        source_file=rel_path,
                    )
                i += 1
                continue

            # Forward typedef: typedef struct Foo Foo, *PFoo;
            fwd_m = _FORWARD_TYPEDEF_RE.match(line)
            if fwd_m:
                i += 1
                continue

            # Struct/union start
            su_m = _STRUCT_START_RE.match(line)
            if su_m:
                kw = su_m.group("kw")
                struct_name = su_m.group("name") or ""
                is_union = kw == "union"
                body_lines = []
                start_line = i + 1
                depth = line.count("{") - line.count("}")
                i += 1

                while i < n and depth > 0:
                    body_lines.append(lines[i])
                    depth += lines[i].count("{") - lines[i].count("}")
                    i += 1

                # After body, check for typedef alias
                typedef_name = struct_name
                if i < n:
                    end_m = _TYPEDEF_END_RE.match(lines[i])
                    if end_m:
                        # e.g. "} Foo, *PFoo;" → take first name
                        names = [x.strip().lstrip("*") for x in end_m.group(1).split(",")]
                        for nm in names:
                            if nm and not nm.startswith("P") or nm == names[0]:
                                typedef_name = nm
                                break
                        i += 1

                if not typedef_name or typedef_name in ("struct", "union", "enum"):
                    continue

                # Skip "struct struct" Ghidra errors
                if typedef_name == "struct":
                    continue

                sdef = StructDef(
                    name=typedef_name,
                    source_file=rel_path,
                    source_line=start_line,
                    is_union=is_union,
                )
                self._parse_struct_body(sdef, body_lines)
                self.structs[typedef_name] = sdef
                continue

            # Skip enum blocks (just count braces to advance past them)
            if _ENUM_START_RE.match(line):
                depth = line.count("{") - line.count("}")
                i += 1
                while i < n and depth > 0:
                    depth += lines[i].count("{") - lines[i].count("}")
                    i += 1
                if i < n and "}" in lines[i]:
                    i += 1
                continue

            i += 1

    def _parse_struct_body(self, sdef: StructDef, body_lines: list[str]) -> None:
        """
        Parse field declarations from struct body lines.
        Stores raw field info; sizes computed later by _compute_struct_size.
        """
        for line in body_lines:
            # artificial_padding macro
            pad = _parse_artificial_padding(line)
            if pad is not None:
                from_v, to_v, type_name = pad
                pad_bytes = to_v - from_v
                if pad_bytes > 0:
                    base_sz = PRIMITIVE_SIZES.get(type_name, 1)
                    count = pad_bytes // base_sz if base_sz > 0 else pad_bytes
                    # Represent as a padding field
                    f = FieldDef(
                        name=f"_pad_{from_v:x}",
                        type_str=f"{type_name}[{count}]" if count != 1 else type_name,
                        base_type=type_name,
                        is_pointer=False,
                        is_array=count != 1,
                        array_count=count,
                        offset=0,   # filled later
                        size=0,     # filled later
                        comment=f"padding {from_v:#x}–{to_v:#x}",
                    )
                    sdef.fields.append(f)
                continue

            result = _parse_field_line(line, self.structs)
            if result is None:
                continue
            field_name, base_type, is_pointer, array_count = result
            comment = _extract_comment(line)

            # Build type string
            type_str = base_type
            if is_pointer:
                type_str += " *"
            if array_count != 1:
                type_str += f"[{array_count}]"

            f = FieldDef(
                name=field_name,
                type_str=type_str,
                base_type=base_type,
                is_pointer=is_pointer,
                is_array=array_count != 1,
                array_count=array_count,
                offset=0,   # filled by _compute_struct_size
                size=0,     # filled by _compute_struct_size
                comment=comment,
            )
            sdef.fields.append(f)

    def _get_type_size(self, type_name: str) -> int:
        """Return the byte size of a named type. Recurse into struct definitions."""
        if type_name in PRIMITIVE_SIZES:
            return PRIMITIVE_SIZES[type_name]

        # Check if it's a known struct
        if type_name in self.structs:
            return self._compute_struct_size(type_name)

        # Unknown type — fall back to 4 bytes (pointer-sized guess)
        return 4

    def _compute_struct_size(self, name: str) -> int:
        """Compute and cache the byte size of a struct, including tail padding."""
        if name in self._size_cache:
            return self._size_cache[name]

        sdef = self.structs.get(name)
        if sdef is None:
            return 0

        # Cycle detection
        if name in self._resolving:
            return 0
        self._resolving.add(name)

        try:
            if sdef.is_union:
                size, align = self._layout_union(sdef)
            else:
                size, align = self._layout_struct(sdef)

            # Apply tail padding
            size = _align_up(size, align)
            sdef.computed_size = size
            sdef.alignment = align
            self._size_cache[name] = size
            return size
        finally:
            self._resolving.discard(name)

    def _field_size_and_align(self, f: FieldDef) -> tuple[int, int]:
        """Return (field_total_size, field_alignment) for layout purposes."""
        if f.is_pointer:
            elem_size = 4
            elem_align = 4
        else:
            elem_size = self._get_type_size(f.base_type)
            # Alignment of the base type itself
            if f.base_type in PRIMITIVE_SIZES:
                elem_align = _field_alignment(elem_size)
            else:
                sub = self.structs.get(f.base_type)
                if sub:
                    self._compute_struct_size(f.base_type)
                    elem_align = sub.alignment
                else:
                    elem_align = _field_alignment(elem_size)

        total_size = elem_size * f.array_count
        return total_size, elem_align

    def _layout_struct(self, sdef: StructDef) -> tuple[int, int]:
        """Compute layout for a regular struct. Returns (total_size, struct_alignment)."""
        offset = 0
        struct_align = 1

        for f in sdef.fields:
            total_size, elem_align = self._field_size_and_align(f)
            struct_align = max(struct_align, elem_align)

            offset = _align_up(offset, elem_align)
            f.offset = offset
            f.size = total_size
            offset += total_size

        return offset, struct_align

    def _layout_union(self, sdef: StructDef) -> tuple[int, int]:
        """Compute layout for a union. Returns (total_size, union_alignment)."""
        max_size = 0
        union_align = 1

        for f in sdef.fields:
            total_size, elem_align = self._field_size_and_align(f)
            union_align = max(union_align, elem_align)
            f.offset = 0
            f.size = total_size
            max_size = max(max_size, total_size)

        return max_size, union_align

    # --- Queries ---

    def lookup(self, name: str) -> Optional[StructDef]:
        return self.structs.get(name)

    def lookup_extern(self, symbol_name: str) -> Optional[ExternDecl]:
        return self.externs.get(symbol_name)

    def find_by_size(self, size: int) -> list[StructDef]:
        return [s for s in self.structs.values() if s.computed_size == size]

    def find_field_at_offset(self, offset: int, size_hint: Optional[int] = None) -> list[dict]:
        """Find all struct fields that start at exactly `offset` bytes."""
        results = []
        for sdef in self.structs.values():
            if size_hint is not None and sdef.computed_size != size_hint:
                continue
            for f in sdef.fields:
                if f.offset == offset:
                    results.append({
                        "struct": sdef.name,
                        "field_name": f.name,
                        "field_type": f.type_str,
                        "offset": f.offset,
                        "struct_size": sdef.computed_size,
                        "source": f"{sdef.source_file}:{sdef.source_line}",
                    })
        return results

    def lookup_recursive(self, name: str, max_depth: int = 3) -> Optional[dict]:
        """Return struct dict with pointer types recursively resolved."""
        sdef = self.lookup(name)
        if sdef is None:
            return None

        seen: set[str] = set()
        return self._recurse(sdef, seen, max_depth)

    def _recurse(self, sdef: StructDef, seen: set[str], depth: int) -> dict:
        seen.add(sdef.name)
        result = sdef.to_dict(include_fields=True)
        if depth <= 0:
            return result

        pointer_types: dict[str, dict] = {}
        for f in sdef.fields:
            if f.is_pointer and f.base_type not in seen:
                sub = self.lookup(f.base_type)
                if sub:
                    pointer_types[f.base_type] = self._recurse(sub, seen, depth - 1)

        if pointer_types:
            result["pointer_types"] = pointer_types
        return result

    @property
    def count(self) -> int:
        return len(self.structs)

    @property
    def extern_count(self) -> int:
        return len(self.externs)
