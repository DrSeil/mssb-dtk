import json
import subprocess
import sys
import argparse
import os
import re

def summarize_objdiff_json(data, symbol_name):
    """Summarize objdiff JSON data into a token-efficient text summary."""
    left_sym, right_sym = get_symbol_data(data, symbol_name)
    if not left_sym:
        return "Target symbol not found in original object."
    if not right_sym:
        return "Symbol not found in your compiled object."

    left_insts = left_sym.get("instructions", [])
    right_insts = right_sym.get("instructions", [])
    
    l_base = int(left_sym.get("address", 0))
    r_base = int(right_sym.get("address", 0))

    mismatches = []
    # Count total mismatches for the header
    total_mismatches = 0
    for inst in right_insts:
        if inst.get("diff_kind") != "None":
            total_mismatches += 1

    if total_mismatches == 0:
        return "MATCH!"

    # Iterate through instructions and find differences
    # We use a simple index-based comparison for the summary
    max_idx = max(len(left_insts), len(right_insts))
    
    for i in range(max_idx):
        l_inst = left_insts[i] if i < len(left_insts) else None
        r_inst = right_insts[i] if i < len(right_insts) else None
        
        if not l_inst or not r_inst or r_inst.get("diff_kind") != "None":
            addr = r_inst.get("address", 0) if r_inst else (l_inst.get("address", 0) if l_inst else 0)
            rel_addr = addr - r_base if r_inst else (addr - l_base if l_inst else 0)
            
            l_op, _ = get_opcode(l_inst) if l_inst else ("<missing>", 0)
            r_op, _ = get_opcode(r_inst) if r_inst else ("<missing>", 0)
            
            l_args = get_normalized_args(l_inst.get("parts", []), l_base, i) if l_inst else []
            r_args = get_normalized_args(r_inst.get("parts", []), r_base, i) if r_inst else []
            
            mismatches.append(
                f"0x{rel_addr:X}: expected {l_op} {', '.join(l_args)}, "
                f"got {r_op} {', '.join(r_args)}"
            )
            
            # Limit to 20 mismatches in summary for token efficiency
            if len(mismatches) >= 20:
                mismatches.append(f"... and {total_mismatches - 20} more")
                break
                
    return f"{total_mismatches} mismatches:\n" + "\n".join(mismatches)

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

def normalize_arg(part, sym_base, inst_index):
    """Extract a normalized argument from an instruction part.

    For branch destinations, convert absolute object-file addresses to
    instruction-index targets so that functions at different positions
    in their object files compare correctly.
    """
    arg = part.get("arg")
    if not arg:
        return None
    if "branch_dest" in arg:
        dest = int(arg["branch_dest"])
        # Convert absolute address to instruction index within the function
        target_index = (dest - sym_base) // 4
        return f"branch_idx:{target_index}"
    if "opaque" in arg:
        return arg["opaque"]
    if "signed" in arg:
        return arg["signed"]
    if "unsigned" in arg:
        return arg["unsigned"]
    return str(arg)

def get_normalized_args(parts, sym_base, inst_index):
    """Extract normalized args from instruction parts, handling branch targets."""
    args = []
    for p in parts:
        if p.get("opcode") or p.get("separator") or p.get("basic"):
            continue
        norm = normalize_arg(p, sym_base, inst_index)
        if norm is not None:
            args.append(norm)
    return args

def get_opcode(inst):
    """Extract the opcode/mnemonic from instruction parts."""
    for p in inst.get("parts", []):
        op = p.get("opcode")
        if op:
            return op.get("mnemonic", ""), op.get("opcode", 0)
    return "", 0

def analyze_diff(left_sym, right_sym):
    if not left_sym:
        return "Target symbol not found in original object."
    if not right_sym:
        return "Symbol not found in your compiled object. Check naming and visibility."

    left_inst_entries = left_sym.get("instructions", [])
    right_inst_entries = right_sym.get("instructions", [])

    # Stub detection: compiled function is significantly shorter than target.
    # This usually means the return type or function body is wrong/incomplete.
    left_count = len(left_inst_entries)
    right_count = len(right_inst_entries)
    stub_warning = ""
    if left_count >= 8 and right_count <= left_count // 3:
        stub_warning = (
            f"\nNOTE: Your compiled function has {right_count} instruction(s) "
            f"but the target has {left_count}. This usually means:\n"
            f"  - The function is declared void but should return a value (update the header)\n"
            f"  - The function body is incomplete / most logic is missing\n"
            f"  Check the return type in the header and assembly before iterating further.\n"
        )

    # Get symbol base addresses for branch target normalization
    l_base = int(left_sym.get("address", 0))
    r_base = int(right_sym.get("address", 0))

    feedback = []

    max_len = max(len(left_inst_entries), len(right_inst_entries))

    for i in range(max_len):
        l_entry = left_inst_entries[i] if i < len(left_inst_entries) else None
        r_entry = right_inst_entries[i] if i < len(right_inst_entries) else None

        l = l_entry.get("instruction", {}) if l_entry else None
        r = r_entry.get("instruction", {}) if r_entry else None

        if l and r:
            l_mnemonic, l_opcode = get_opcode(l)
            r_mnemonic, r_opcode = get_opcode(r)

            l_parts = l.get("parts", [])
            r_parts = r.get("parts", [])

            l_args = get_normalized_args(l_parts, l_base, i)
            r_args = get_normalized_args(r_parts, r_base, i)

            if l_mnemonic != r_mnemonic:
                feedback.append(f"Line {i}: Mismatch")
                feedback.append(f"  Target: {l.get('formatted', '')}")
                feedback.append(f"  Current: {r.get('formatted', '')}")
                feedback.append("  -> Instruction mnemonic mismatch.")
            elif l_args != r_args:
                feedback.append(f"Line {i}: Mismatch")
                feedback.append(f"  Target: {l.get('formatted', '')}")
                feedback.append(f"  Current: {r.get('formatted', '')}")
                feedback.append(f"  -> Operand mismatch. Target args: {l_args}, Your args: {r_args}")
        elif l:
            feedback.append(f"Line {i}: Missing instruction in your code.")
            feedback.append(f"  Target: {l.get('formatted', '')}")
        elif r:
            feedback.append(f"Line {i}: Extra instruction in your code.")
            feedback.append(f"  Current: {r.get('formatted', '')}")

    if not feedback:
        return "MATCH! Standardized assembly is identical."

    return stub_warning + "\n".join(feedback)

def get_formatted_asm(sym):
    if not sym:
        return ""
    lines = []
    for entry in sym.get("instructions", []):
        inst = entry.get("instruction")
        if inst:
            lines.append(inst.get("formatted", ""))
    return "\n".join(lines)

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
    parser.add_argument("--current-asm", action="store_true", help="Output only the compiled assembly of the current code")
    
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
    
    if parsed_args.current_asm:
        print(get_formatted_asm(right_sym))
    else:
        print(f"Feedback for {symbol} in {unit}:")
        print("-" * 40)
        print(analyze_diff(left_sym, right_sym))

if __name__ == "__main__":
    main()
