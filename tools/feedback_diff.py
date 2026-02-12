import json
import subprocess
import sys
import argparse
import os
import re

def run_objdiff(unit, symbol):
    cmd = [
        "objdiff", "diff",
        "--format", "json",
        "-u", unit,
        "-o", "-",
        symbol
    ]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return json.loads(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error running objdiff: {e.stderr}", file=sys.stderr)
        return None
    except json.JSONDecodeError:
        print("Error decoding objdiff JSON output", file=sys.stderr)
        return None

def get_symbol_data(data, symbol_name):
    # Search in left and right for the symbol
    left_sym = None
    right_sym = None
    
    if "left" in data and "symbols" in data["left"]:
        for sym in data["left"]["symbols"]:
            if sym["name"] == symbol_name:
                left_sym = sym
                break
    
    if "right" in data and "symbols" in data["right"]:
        for sym in data["right"]["symbols"]:
            if sym["name"] == symbol_name or (sym.get("demangled_name") == symbol_name):
                right_sym = sym
                break
                
    return left_sym, right_sym

def analyze_diff(left_sym, right_sym):
    if not left_sym:
        return "Target symbol not found in original object."
    if not right_sym:
        return "Symbol not found in your compiled object. Check naming and visibility."

    left_inst_entries = left_sym.get("instructions", [])
    right_inst_entries = right_sym.get("instructions", [])
    
    feedback = []
    
    max_len = max(len(left_inst_entries), len(right_inst_entries))
    
    for i in range(max_len):
        l_entry = left_inst_entries[i] if i < len(left_inst_entries) else None
        r_entry = right_inst_entries[i] if i < len(right_inst_entries) else None
        
        l = l_entry.get("instruction", {}) if l_entry else None
        r = r_entry.get("instruction", {}) if r_entry else None
        
        if l and r:
            l_fmt = l.get("formatted", "")
            r_fmt = r.get("formatted", "")
            
            if l_fmt != r_fmt:
                # Basic check for diff
                feedback.append(f"Line {i}: Mismatch")
                feedback.append(f"  Target: {l_fmt}")
                feedback.append(f"  Current: {r_fmt}")
                
                # Try to be smarter about the mismatch
                l_parts = l.get("parts", [])
                r_parts = r.get("parts", [])
                
                if l.get("opcode") != r.get("opcode"):
                    feedback.append("  -> Instruction mnemonic mismatch.")
                else:
                    # Check for register swaps etc in parts
                    l_args = [p.get("opaque", p.get("signed", p.get("unsigned", ""))) for p in l_parts if not p.get("opcode") and not p.get("separator")]
                    r_args = [p.get("opaque", p.get("signed", p.get("unsigned", ""))) for p in r_parts if not p.get("opcode") and not p.get("separator")]
                    
                    if l_args != r_args:
                        feedback.append(f"  -> Operand mismatch. Target args: {l_args}, Your args: {r_args}")
        elif l:
            feedback.append(f"Line {i}: Missing instruction in your code.")
            feedback.append(f"  Target: {l.get('formatted', '')}")
        elif r:
            feedback.append(f"Line {i}: Extra instruction in your code.")
            feedback.append(f"  Current: {r.get('formatted', '')}")

    if not feedback:
        return "MATCH! Standardized assembly is identical."
    
    return "\n".join(feedback)

def parse_symbols(symbol_file):
    symbols = {}
    if not os.path.exists(symbol_file):
        return symbols
    with open(symbol_file, 'r') as f:
        for line in f:
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

def find_unit_for_symbol(symbol_name):
    base_dir = "."
    config_base = os.path.join(base_dir, "config/GYQE01")
    
    found_symbol = None
    module = None
    
    # Check main symbols first
    symbols = parse_symbols(os.path.join(config_base, 'symbols.txt'))
    if symbol_name in symbols:
        found_symbol = symbols[symbol_name]
        module = ""
    else:
        # Check modules
        for m in ['game', 'menus', 'challenge']:
            sys_file = os.path.join(config_base, m, 'symbols.txt')
            symbols = parse_symbols(sys_file)
            if symbol_name in symbols:
                found_symbol = symbols[symbol_name]
                module = m
                break
                
    if not found_symbol:
        return None

    addr = found_symbol['addr']
    found_file = None
    
    if module == "":
        splits = parse_splits(os.path.join(config_base, 'splits.txt'))
        found_file = find_file_for_address(splits, addr)
    else:
        splits = parse_splits(os.path.join(config_base, module, 'splits.txt'))
        found_file = find_file_for_address(splits, addr)

    if not found_file:
        return None

    # Map to build/GYQE01/<module>/obj/
    if module:
        obj_path = os.path.join(f"build/GYQE01/{module}/obj", os.path.splitext(found_file)[0] + ".o")
    else:
        obj_path = os.path.join("build/GYQE01/obj", os.path.splitext(found_file)[0] + ".o")
        
    # Look for unit name in objdiff.json
    unit_name = None
    if os.path.exists("objdiff.json"):
        try:
            with open("objdiff.json", 'r') as f:
                config = json.load(f)
                for unit in config.get('units', []):
                    if unit.get('target_path') == obj_path:
                        return unit.get('name')
        except Exception:
            pass
    return None

def main():
    parser = argparse.ArgumentParser(description="Analyze objdiff output for feedback.")
    parser.add_argument("args", nargs="+", help="Symbol name (and optionally unit name)")
    
    parsed_args = parser.parse_args()
    
    if len(parsed_args.args) == 1:
        symbol = parsed_args.args[0]
        unit = find_unit_for_symbol(symbol)
        if not unit:
            print(f"Error: Could not find unit for symbol {symbol}", file=sys.stderr)
            sys.exit(1)
    elif len(parsed_args.args) == 2:
        unit = parsed_args.args[0]
        symbol = parsed_args.args[1]
    else:
        parser.print_help()
        sys.exit(1)
        
    data = run_objdiff(unit, symbol)
    if not data:
        sys.exit(1)
        
    left_sym, right_sym = get_symbol_data(data, symbol)
    
    print(f"Feedback for {symbol} in {unit}:")
    print("-" * 40)
    print(analyze_diff(left_sym, right_sym))

if __name__ == "__main__":
    main()
