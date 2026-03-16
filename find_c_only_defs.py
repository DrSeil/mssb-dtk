import os
import re
from pathlib import Path
import argparse

def main():
    parser = argparse.ArgumentParser(description="Find struct and extern definitions in .c files that are missing from headers.")
    parser.add_argument('--dir', default='.', help='Directory to search (default: .)')
    args = parser.parse_args()

    root_dir = Path(args.dir)
    
    # Store names found in .h files
    h_structs = set()
    h_externs = set()
    
    # Regex to capture struct names, e.g. "struct Foo {"
    struct_rx = re.compile(r'struct\s+([a-zA-Z0-9_]+)\s*\{')
    
    # Regex to capture extern declarations
    extern_rx = re.compile(r'^\s*extern\s+[^;]+;', re.MULTILINE)
    
    print("Parsing headers...")
    for h_file in root_dir.rglob('*.h'):
        try:
            content = h_file.read_text(encoding='utf-8')
            for match in struct_rx.finditer(content):
                h_structs.add(match.group(1))
            
            for match in extern_rx.finditer(content):
                line = match.group(0)
                # Primitive way to get var name: the last words before ;
                parts = re.findall(r'[a-zA-Z0-9_]+', line.split(';')[0])
                if parts:
                    h_externs.add(parts[-1])
        except Exception:
            pass
            
    print("Searching .c files for exclusive definitions...\n")
    found_any = False
    
    for c_file in root_dir.rglob('*.c'):
        try:
            content = c_file.read_text(encoding='utf-8')
            found_structs = []
            found_externs = []
            
            for match in struct_rx.finditer(content):
                struct_name = match.group(1)
                if struct_name not in h_structs:
                    found_structs.append(struct_name)
                    
            for match in extern_rx.finditer(content):
                line = match.group(0).strip()
                parts = re.findall(r'[a-zA-Z0-9_]+', line.split(';')[0])
                if parts:
                    var_name = parts[-1]
                    # In a typical codebase, an extern in a .c file is bad practice,
                    # but we specifically filter those not found in headers.
                    if var_name not in h_externs:
                        found_externs.append(line)
                        
            if found_structs or found_externs:
                found_any = True
                print(f"--- {c_file} ---")
                for s in found_structs:
                    print(f"  [Struct] struct {s} {{ ... }}")
                for e in found_externs:
                    print(f"  [Extern] {e}")
        except Exception:
            pass

    if not found_any:
        print("No C-only structs or externs found!")

if __name__ == '__main__':
    main()
