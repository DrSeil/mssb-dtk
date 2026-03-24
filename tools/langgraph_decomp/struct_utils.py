"""Utilities for parsing and modifying C struct definitions."""

import re
from typing import List, Dict, Optional, Tuple

class StructField:
    def __init__(self, type_str: str, name: str, offset: int, size: int, is_padding: bool = False, count: int = 1, base_size: int = 4):
        self.type_str = type_str
        self.name = name
        self.offset = offset
        self.size = size
        self.is_padding = is_padding
        self.count = count
        self.base_size = base_size

    def __repr__(self):
        return f"StructField({self.type_str}, {self.name}, {hex(self.offset)}, {hex(self.size)})"

def parse_symbols_txt(file_path: str) -> List[Dict]:
    """Parse a project symbols.txt file.
    
    Format: name = section:address; // type:type size:size scope:scope
    """
    symbols = []
    import os
    if not os.path.exists(file_path):
        return symbols
        
    pattern = re.compile(r'^(\w+)\s*=\s*(\w+):(0x[0-9A-Fa-f]+);\s*//\s*type:(\w+)(?:\s+size:(0x[0-9A-Fa-f]+))?')
    
    with open(file_path, 'r') as f:
        for line in f:
            match = pattern.match(line.strip())
            if match:
                name, section, addr_str, sym_type, size_str = match.groups()
                addr = int(addr_str, 16)
                size = int(size_str, 16) if size_str else 0
                symbols.append({
                    'name': name,
                    'section': section,
                    'addr': addr,
                    'type': sym_type,
                    'size': size
                })
    return symbols

def get_sda_mapping(symbols: List[Dict]) -> Dict[str, List[Dict]]:
    """Group symbols by their SDA base register (r13 or r2).
    
    In GameCube (GYQE01):
    r13 (SDA) typically points to .sdata / .sbss (around 0x804D76C0)
    r2 (SDA2) typically points to .sdata2 / .sbss2 (around 0x804D2AC0)
    """
    sda_r13_base = 0x804D76C0
    sda_r2_base = 0x804D2AC0
    
    mapping = {'r13': [], 'r2': []}
    
    # Sections associated with r13
    r13_sections = {'.sdata', '.sbss', 'sdata', 'sbss'}
    # Sections associated with r2
    r2_sections = {'.sdata2', '.sbss2', 'sdata2', 'sbss2'}
    
    for sym in symbols:
        if sym['section'] in r13_sections:
            offset = sym['addr'] - sda_r13_base
            sym['sda_offset'] = offset
            mapping['r13'].append(sym)
        elif sym['section'] in r2_sections:
            offset = sym['addr'] - sda_r2_base
            sym['sda_offset'] = offset
            mapping['r2'].append(sym)
            
    return mapping

def parse_struct_fields(struct_def: str) -> List[StructField]:
    """Parse a struct definition into a list of StructField objects.
    
    Expects format with offset comments: /* 0xXXX */ type name;
    """
    fields = []
    # Match: /* 0xOFFSET */ type name[optional_size]; // size: 0xXX
    # Improved regex to handle E(storage, enum) macros and other decorations
    pattern = r'/\*\s*(0x[0-9a-fA-F]+)\s*\*/\s*([\w\s\*\(\),]+?)\s+(\w+)(?:\[(0x[0-9a-fA-F]+|\d+)\])?\s*;(?:\s*//\s*size:\s*(0x[0-9a-fA-F]+))?'
    
    # Try to find the size of basic types
    type_sizes = {
        'u8': 1, 's8': 1, 'char': 1, 'bool': 1,
        'u16': 2, 's16': 2, 'short': 2,
        'u32': 4, 's32': 4, 'int': 4, 'f32': 4, 'long': 4,
        'u64': 8, 's64': 8, 'f64': 8, 'double': 8,
        'Mtx': 48, 'Vec': 12, 'VecXYZ': 12, 'VecXZ': 8,
        'MiniGameControlStruct': 22,
    }

    for match in re.finditer(pattern, struct_def):
        offset_str, type_str, name, array_size_str, comment_size_str = match.groups()
        offset = int(offset_str, 16)
        type_str = type_str.strip()
        
        # Calculate base size
        if comment_size_str:
            base_size = int(comment_size_str, 16)
        elif '*' in type_str:
            base_size = 4 # 32-bit pointers
        else:
            # Check for known types, handle E(storage, type) macro by taking the first word of storage or the storage itself
            clean_type = type_str
            if type_str.startswith('E('):
                m = re.match(r'E\s*\(\s*(\w+)', type_str)
                if m:
                    clean_type = m.group(1)
            
            base_size = type_sizes.get(clean_type.split()[-1], 4)
            
        count = 1
        if array_size_str:
            if array_size_str.startswith('0x'):
                count = int(array_size_str, 16)
            else:
                count = int(array_size_str)
        
        size = base_size * count
            
        # Treat things as padding if they explicitly have "pad" in the name OR start with "_"
        is_padding = name.lower().startswith('pad') or name.lower().startswith('_pad') or name.startswith('_') or name.lower().startswith('__padding')
        
        fields.append(StructField(type_str + ("*" if '*' in type_str and not type_str.endswith('*') else ""), 
                                 name, offset, size, is_padding, count, base_size))
    
    return sorted(fields, key=lambda x: x.offset)

def reconcile_struct(struct_def: str, modifications: List[Dict], log=None) -> str:
    """Apply modifications to a struct and return the new definition.
    
    modifications: List of {'action': 'add_field', 'type': str, 'name': str, 'offset': int}
    """
    def _log(msg):
        if log is not None:
            log(msg)
        else:
            print(msg)

    if not modifications:
        return struct_def

    # 1. Parse current fields
    fields = parse_struct_fields(struct_def)
    if not fields:
        _log("[struct_utils] WARNING: No fields parsed from struct definition!")
        return struct_def

    # Extract struct wrapper
    header_match = re.search(r'(typedef\s+struct\s+\w+\s*\{)', struct_def)
    footer_match = re.search(r'(\}\s*\w+\s*;)', struct_def)
    if not header_match or not footer_match:
        _log("[struct_utils] WARNING: Could not find struct header/footer!")
        return struct_def
        
    header = header_match.group(1)
    footer = footer_match.group(1)
    
    # 2. Collect ALL fields. 
    named = {f.offset: f for f in fields}
    has_changes = False
    
    for mod in modifications:
        action = mod.get('action', 'add_field')
        m_type = mod.get('type', 'u8')
        m_name = mod.get('name', 'unk')
        m_offset_raw = mod.get('offset', 0)
        
        if isinstance(m_offset_raw, str):
            m_offset = int(m_offset_raw, 16) if '0x' in m_offset_raw else int(m_offset_raw)
        else:
            m_offset = m_offset_raw

        if action == 'add_field':
            # Size mapping
            size_map = {'u8': 1, 's8': 1, 'u16': 2, 's16': 2, 'u32': 4, 's32': 4, 'f32': 4, 'int': 4}
            m_size = size_map.get(m_type, 4)
            if '*' in m_type: m_size = 4
            
            # 1. Check if a field with this EXACT name already exists
            existing_by_name = next((f for f in named.values() if f.name == m_name), None)
            if existing_by_name:
                if existing_by_name.offset == m_offset:
                    _log(f"[struct_utils] Field '{m_name}' already exists at {hex(m_offset)}. Skipping.")
                    continue
                else:
                    _log(f"[struct_utils] REFUSING to add '{m_name}' at {hex(m_offset)}: name already exists at {hex(existing_by_name.offset)}")
                    return struct_def # Return original on refusal

            # 2. Check for overlaps/matches
            overlap_field = None
            for off, f in named.items():
                if off <= m_offset < off + f.size:
                    overlap_field = f
                    break
            
            if overlap_field:
                # If it's an exact match for a padding/placeholder, replace it
                if overlap_field.offset == m_offset and overlap_field.is_padding:
                     _log(f"[struct_utils] Replacing placeholder '{overlap_field.name}' with '{m_name}' at {hex(m_offset)}")
                     # If the placeholder was larger than the new field, we should split it
                     if overlap_field.size > m_size:
                         remaining_size = overlap_field.size - m_size
                         new_pad_offset = m_offset + m_size
                         named[new_pad_offset] = StructField("u8", f"_pad_{hex(new_pad_offset)}", new_pad_offset, remaining_size, True)
                     named[m_offset] = StructField(m_type, m_name, m_offset, m_size, False)
                     has_changes = True
                     continue
                
                # If it overlaps a padding field (even an array), split the padding
                if overlap_field.is_padding:
                    _log(f"[struct_utils] Splitting padding '{overlap_field.name}' at {hex(m_offset)} for '{m_name}'")
                    # Part before the new field
                    before_size = m_offset - overlap_field.offset
                    if before_size > 0:
                        named[overlap_field.offset] = StructField("u8", f"_pad_{hex(overlap_field.offset)}", overlap_field.offset, before_size, True)
                    else:
                        del named[overlap_field.offset]
                    
                    # The new field
                    named[m_offset] = StructField(m_type, m_name, m_offset, m_size, False)
                    
                    # Part after the new field
                    after_offset = m_offset + m_size
                    after_size = (overlap_field.offset + overlap_field.size) - after_offset
                    if after_size > 0:
                        named[after_offset] = StructField("u8", f"_pad_{hex(after_offset)}", after_offset, after_size, True)
                    has_changes = True
                    continue
                else:
                    _log(f"[struct_utils] REFUSING to add '{m_name}' at {hex(m_offset)}: overlaps field '{overlap_field.name}' ({hex(overlap_field.offset)}-{hex(overlap_field.offset+overlap_field.size)})")
                    return struct_def # Return original on refusal
                
            named[m_offset] = StructField(m_type, m_name, m_offset, m_size, False)
            has_changes = True

    if not has_changes:
        return struct_def

    # 3. Sort fields and generate the final list. 
    sorted_fields = sorted(named.values(), key=lambda x: x.offset)
    
    # 4. Format the output
    lines = [header]
    for f in sorted_fields:
        offset_str = f"0x{f.offset:03X}" if f.offset < 0x1000 else f"0x{f.offset:X}"
        size_suffix = f" // size: {hex(f.base_size)}" if f.base_size not in (1, 2, 4, 8, 12, 48) else ""
        
        if f.count > 1 and not f.is_padding:
            name_with_array = f"{f.name}[{hex(f.count) if f.count > 10 else f.count}]"
            lines.append(f"    /* {offset_str} */ {f.type_str} {name_with_array};{size_suffix}")
        elif f.is_padding:
            if f.size > 1:
                lines.append(f"    /* {offset_str} */ {f.type_str} {f.name}[{hex(f.size)}];")
            else:
                lines.append(f"    /* {offset_str} */ {f.type_str} {f.name};")
        else:
            lines.append(f"    /* {offset_str} */ {f.type_str} {f.name};{size_suffix}")
    lines.append(footer)
    
    return "\n".join(lines)

if __name__ == "__main__":
    # Test
    test_struct = """
typedef struct CommonBss_35154 {
    /* 0x000 */ u32 firstMember;
    /* 0x004 */ u8 _pad0[0x3AC - 4];
    /* 0x3AC */ u32 bitfield;
} CommonBss_35154;
"""
    mods = [
        {'action': 'add_field', 'type': 'f32', 'name': 'unk3EC', 'offset': '0x3EC'},
        {'action': 'add_field', 'type': 'u8', 'name': 'unk479', 'offset': '0x479'}
    ]
    print(reconcile_struct(test_struct, mods))
