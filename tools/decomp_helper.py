import os
import re
import subprocess
import sys
import argparse

def parse_symbols(symbol_file):
    symbols = {}
    if not os.path.exists(symbol_file):
        return symbols
    with open(symbol_file, 'r') as f:
        for line in f:
            # example: __init_hardware = .init:0x80003400; // type:function size:0x24 scope:global
            match = re.match(r'(\w+)\s*=\s*(.*?):0x([0-9a-fA-F]+);', line)
            if match:
                name, section, addr = match.groups()
                symbols[name] = {'section': section, 'addr': int(addr, 16)}
    return symbols

def parse_splits(splits_file):
    splits = []
    current_file = None
    if not os.path.exists(splits_file):
        return splits
    with open(splits_file, 'r') as f:
        for line in f:
            line = line.strip()
            if line.endswith(':'):
                current_file = line[:-1]
            elif line.startswith('.') or 'start:' in line:
                # example: .text       start:0x8006D138 end:0x8006DC04
                match = re.match(r'(\.?\w+)\s+start:0x([0-9a-fA-F]+)\s+end:0x([0-9a-fA-F]+)', line)
                if match and current_file:
                    section, start, end = match.groups()
                    splits.append({
                        'file': current_file,
                        'section': section,
                        'start': int(start, 16),
                        'end': int(end, 16)
                    })
    return splits

def find_file_for_address(splits, addr):
    for entry in splits:
        if entry['start'] <= addr < entry['end']:
            return entry['file']
    return None

def extract_asm(asm_file, func_name):
    if not os.path.exists(asm_file):
        return None
    
    output = []
    recording = False
    try:
        with open(asm_file, 'r') as f:
            for line in f:
                if line.strip().startswith(f".fn {func_name}"):
                    recording = True
                if recording:
                    output.append(line)
                if line.strip().startswith(f".endfn {func_name}"):
                    recording = False
                    break
    except Exception:
        return None
    return "".join(output) if output else None

# Known absolute offsets for modules to bridge DTK names to Ghidra names
# These are used to calculate the absolute address for FUN_XXXXXXXX fallback
MODULE_OFFSETS = {
    "game": 0x8063F094,
}

def extract_ghidra_decomp(func_name, in_game_c_path, addr=None, module=None):
    if not os.path.exists(in_game_c_path):
        return None
    
    try:
        # 1. First try searching for the exact function name
        # Search for the function name followed by '(' to find the definition line
        grep_cmd = ["grep", "-n", f" {func_name}(", in_game_c_path]
        result = subprocess.run(grep_cmd, capture_output=True, text=True)
        if result.returncode != 0:
            grep_cmd = ["grep", "-n", f"^{func_name}(", in_game_c_path]
            result = subprocess.run(grep_cmd, capture_output=True, text=True)
            
        # 2. If not found and we have address info, try FUN_XXXXXXXX
        if result.returncode != 0 and addr is not None and module in MODULE_OFFSETS:
            abs_addr = MODULE_OFFSETS[module] + addr
            ghidra_name = f"FUN_{abs_addr:08x}"
            # Ghidra often uses lowercase in hex names but sometimes uppercase
            grep_cmd = ["grep", "-n", f" {ghidra_name}(", in_game_c_path]
            result = subprocess.run(grep_cmd, capture_output=True, text=True)
            if result.returncode != 0:
                grep_cmd = ["grep", "-n", f"^{ghidra_name}(", in_game_c_path]
                result = subprocess.run(grep_cmd, capture_output=True, text=True)
            
            # Try lowercase hex if still not found
            if result.returncode != 0:
                ghidra_name_lower = f"FUN_{abs_addr:08x}".lower()
                grep_cmd = ["grep", "-n", f" {ghidra_name_lower}(", in_game_c_path]
                result = subprocess.run(grep_cmd, capture_output=True, text=True)
                if result.returncode != 0:
                    grep_cmd = ["grep", "-n", f"^{ghidra_name_lower}(", in_game_c_path]
                    result = subprocess.run(grep_cmd, capture_output=True, text=True)

        if result.returncode != 0:
            return None
        
        # Often there are multiple hits (declarations, calls). 
        # We want the one without a semicolon at the end of the signature line (or next line)
        hits = result.stdout.splitlines()
        start_line = -1
        for hit in hits:
            parts = hit.split(':', 1)
            if len(parts) < 2: continue
            l_num = int(parts[0])
            l_text = parts[1].strip()
            
            # Skip declarations/calls (have semicolons)
            if l_text.endswith(';'): continue
            
            # Found potential definition start
            start_line = l_num
            break
            
        if start_line == -1: return None
        
        # Read from start_line onwards and match braces
        output = []
        brace_count = 0
        found_start_brace = False
        
        with open(in_game_c_path, 'r') as f:
            # Skip to start_line
            for i, line in enumerate(f):
                if i + 1 < start_line: continue
                
                output.append(line)
                brace_count += line.count('{')
                brace_count -= line.count('}')
                
                if '{' in line:
                    found_start_brace = True
                
                if found_start_brace and brace_count <= 0:
                    break
                    
        return "".join(output) if output else None
            
    except Exception as e:
        print(f"Error extracting Ghidra decomp: {e}")
        return None

def main():
    parser = argparse.ArgumentParser(description='Decompilation Helper')
    parser.add_argument('function', help='Function name to look up')
    args = parser.parse_args()

    # Base directory of the project
    base_dir = "."
    
    config_base = os.path.join(base_dir, "config/GYQE01")
    asm_base = os.path.join(base_dir, "build/GYQE01/asm")

    symbol_files = [
        os.path.join(config_base, 'symbols.txt'),
        os.path.join(config_base, 'game/symbols.txt'),
        os.path.join(config_base, 'menus/symbols.txt'),
        os.path.join(config_base, 'challenge/symbols.txt')
    ]
    
    split_files = [
        os.path.join(config_base, 'splits.txt'),
        os.path.join(config_base, 'game/splits.txt'),
        os.path.join(config_base, 'menus/splits.txt'),
        os.path.join(config_base, 'challenge/splits.txt')
    ]

    found_symbol = None
    module = None
    
    # Check main symbols first
    symbols = parse_symbols(os.path.join(config_base, 'symbols.txt'))
    if args.function in symbols:
        found_symbol = symbols[args.function]
        module = ""
    else:
        # Check modules
        for m in ['game', 'menus', 'challenge']:
            sys_file = os.path.join(config_base, m, 'symbols.txt')
            symbols = parse_symbols(sys_file)
            if args.function in symbols:
                found_symbol = symbols[args.function]
                module = m
                break
    
    if not found_symbol:
        # Fallback: search build/GYQE01/asm and REL asm dirs directly
        print(f"Function {args.function} not found in symbols. Searching ASM files directly...")
        search_paths = [asm_base]
        for m in ['game', 'menus', 'challenge']:
            search_paths.append(os.path.join(base_dir, f"build/GYQE01/{m}/asm"))
            
        asm_content = None
        for s_path in search_paths:
            if not os.path.exists(s_path): continue
            for root, dirs, files in os.walk(s_path):
                for file in files:
                    if file.endswith('.s'):
                        test_path = os.path.join(root, file)
                        asm_content = extract_asm(test_path, args.function)
                        if asm_content:
                            print(f"Found function in: {test_path}")
                            print("\nTarget ASM:")
                            print("-" * 20)
                            print(asm_content)
                            return
        print(f"Could not find function {args.function} anywhere.")
        return

    addr = found_symbol['addr']
    found_file = None
    
    if module == "":
        splits = parse_splits(os.path.join(config_base, 'splits.txt'))
        found_file = find_file_for_address(splits, addr)
    else:
        splits = parse_splits(os.path.join(config_base, module, 'splits.txt'))
        found_file = find_file_for_address(splits, addr)

    if not found_file:
        print(f"Could not find source file for address {hex(addr)} in module '{module}'")
        return

    print(f"Function: {args.function}")
    print(f"Address: {hex(addr)}")
    print(f"Module: {module if module else 'main'}")
    print(f"Source: {found_file}")

    # Map to build/GYQE01/<module>/asm/
    rel_path = os.path.splitext(found_file)[0] + ".s"
    if module:
        asm_path = os.path.join(base_dir, f"build/GYQE01/{module}/asm", rel_path)
    else:
        asm_path = os.path.join(asm_base, rel_path)

    print(f"Expected ASM Path: {asm_path}")
    asm_content = extract_asm(asm_path, args.function)
    
    if not asm_content:
        # Try searching all files in appropriate asm dir
        print("Direct extraction failed. Searching appropriate ASM directory...")
        search_root = os.path.join(base_dir, f"build/GYQE01/{module}/asm") if module else asm_base
        if os.path.exists(search_root):
            for root, dirs, files in os.walk(search_root):
                for file in files:
                    if file.endswith('.s'):
                        test_path = os.path.join(root, file)
                        asm_content = extract_asm(test_path, args.function)
                        if asm_content:
                            print(f"Found function in: {test_path}")
                            break
                if asm_content:
                    break

    if asm_content:
        print("\nTarget ASM:")
        print("-" * 20)
        print(asm_content)
        
        # Suggest objdiff command
        obj_path = None
        if module:
            obj_path = os.path.join(f"build/GYQE01/{module}/obj", os.path.splitext(found_file)[0] + ".o")
        else:
            obj_path = os.path.join("build/GYQE01/obj", os.path.splitext(found_file)[0] + ".o")
        
        # Look for unit name in objdiff.json
        unit_name = None
        if os.path.exists("objdiff.json"):
            try:
                import json
                with open("objdiff.json", 'r') as f:
                    config = json.load(f)
                    for unit in config.get('units', []):
                        if unit.get('target_path') == obj_path:
                            unit_name = unit.get('name')
                            break
            except Exception:
                pass
        
        print("-" * 20)
        if unit_name:
            print(f"Suggested objdiff command: objdiff diff -u {unit_name}")
        else:
            print(f"Suggested objdiff unit (guess): {obj_path}")

    else:
        print("Could not extract ASM.")

if __name__ == "__main__":
    main()
