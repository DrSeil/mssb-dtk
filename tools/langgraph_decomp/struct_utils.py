"""Utilities for parsing and modifying C struct definitions."""

import re
from typing import List, Dict, Optional, Tuple

class StructField:
    def __init__(self, type_str: str, name: str, offset: int, size: int, is_padding: bool = False):
        self.type_str = type_str
        self.name = name
        self.offset = offset
        self.size = size
        self.is_padding = is_padding

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
    # Match: /* 0xOFFSET */ type name[optional_size];
    # Also handles lines without comments if needed, but primarily relies on them for robustness.
    pattern = r'/\*\s*(0x[0-9a-fA-F]+)\s*\*/\s*([\w\s\*]+?)\s+(\w+)(?:\[(0x[0-9a-fA-F]+|\d+)\])?\s*;'
    
    # Try to find the size of basic types
    type_sizes = {
        'u8': 1, 's8': 1, 'char': 1, 'bool': 1,
        'u16': 2, 's16': 2, 'short': 2,
        'u32': 4, 's32': 4, 'int': 4, 'f32': 4, 'long': 4,
        'u64': 8, 's64': 8, 'f64': 8, 'double': 8,
        'Mtx': 48, 'Vec': 12, 'VecXYZ': 12, 'VecXZ': 8,
    }

    for match in re.finditer(pattern, struct_def):
        offset_str, type_str, name, array_size_str = match.groups()
        offset = int(offset_str, 16)
        type_str = type_str.strip()
        
        # Calculate size
        if '*' in type_str:
            base_size = 4 # 32-bit pointers
        else:
            # Check for known types
            base_size = type_sizes.get(type_str.split()[-1], 4)
            
        if array_size_str:
            if array_size_str.startswith('0x'):
                count = int(array_size_str, 16)
            else:
                count = int(array_size_str)
            size = base_size * count
        else:
            size = base_size
            
        # Consider _0, _1, _1A3F, _0x1A3F etc. as padding/placeholders
        is_placeholder = bool(re.match(r'^(_[0-9A-Fa-f]+|_[0-9]+)$', name))
        is_padding = name.startswith('_pad') or name.startswith('pad') or is_placeholder
        
        fields.append(StructField(type_str + ("*" if '*' in type_str and not type_str.endswith('*') else ""), 
                                 name, offset, size, is_padding))
    
    return sorted(fields, key=lambda x: x.offset)

def reconcile_struct(struct_def: str, modifications: List[Dict]) -> str:
    """Apply modifications to a struct and return the new definition.
    
    modifications: List of {'action': 'add_field', 'type': str, 'name': str, 'offset': int}
    """
    # 1. Parse current fields
    fields = parse_struct_fields(struct_def)
    if not fields:
        return struct_def

    # Extract struct wrapper
    header_match = re.search(r'(typedef\s+struct\s+\w+\s*\{)', struct_def)
    footer_match = re.search(r'(\}\s*\w+\s*;)', struct_def)
    if not header_match or not footer_match:
        return struct_def
        
    header = header_match.group(1)
    footer = footer_match.group(1)
    
    # 2. Collect all named fields (original non-padding + new additions)
    named = {f.offset: f for f in fields if not f.is_padding}
    
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
            
            # If we already have a named field at this offset, check if it's the same
            if m_offset in named:
                existing = named[m_offset]
                # If it's a real name (not a placeholder), and it matches, skip
                if existing.name == m_name and existing.type_str == m_type:
                    continue
                # If we are here, we are potentially overriding or it was a placeholder
                # We'll allow the override
                
            named[m_offset] = StructField(m_type, m_name, m_offset, m_size, False)

    # 3. Sort named fields and generate the final list with padding
    sorted_named = sorted(named.values(), key=lambda x: x.offset)
    final_fields = []
    
    # We assume the struct starts at the offset of the first field found
    # In UnknownHeaders.h they usually start at 0x0.
    current_offset = sorted_named[0].offset if sorted_named else 0
    
    # If the first field isn't at 0, should we add padding? 
    # Usually we just preserve the structure from the first field.
    
    for f in sorted_named:
        # Gap?
        if f.offset > current_offset:
            gap_size = f.offset - current_offset
            final_fields.append(StructField("u8", f"_pad_{hex(current_offset)}", current_offset, gap_size, True))
            current_offset = f.offset
            
        # Overlap?
        if f.offset < current_offset:
            # Skip field if it overlaps a higher-priority named field already added
            print(f"[struct_utils] Skipping overlapping field {f.name} at {hex(f.offset)}")
            continue
            
        final_fields.append(f)
        current_offset += f.size

    # 4. Format the output
    lines = [header]
    for f in final_fields:
        offset_str = f"0x{f.offset:03X}" if f.offset < 0x1000 else f"0x{f.offset:X}"
        if f.is_padding:
            # For padding, we use a slightly more readable name if possible, or preservation
            size_str = f"0x{f.size:X}"
            lines.append(f"    /* {offset_str} */ {f.type_str} {f.name}[{size_str}];")
        else:
            lines.append(f"    /* {offset_str} */ {f.type_str} {f.name};")
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
