import json
import subprocess
import sys
import argparse
import os

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

    left_insts = left_sym.get("instructions", [])
    right_insts = right_sym.get("instructions", [])
    
    feedback = []
    
    max_len = max(len(left_insts), len(right_insts))
    
    for i in range(max_len):
        l = left_insts[i] if i < len(left_insts) else None
        r = right_insts[i] if i < len(right_insts) else None
        
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

def main():
    parser = argparse.ArgumentParser(description="Analyze objdiff output for feedback.")
    parser.add_argument("unit", help="objdiff unit name")
    parser.add_argument("symbol", help="Symbol name to diff")
    
    args = parser.parse_args()
    
    data = run_objdiff(args.unit, args.symbol)
    if not data:
        sys.exit(1)
        
    left_sym, right_sym = get_symbol_data(data, args.symbol)
    
    print(f"Feedback for {args.symbol} in {args.unit}:")
    print("-" * 40)
    print(analyze_diff(left_sym, right_sym))

if __name__ == "__main__":
    main()
