import subprocess
import sys
import os
import re

def get_function_asm(func_name):
    try:
        result = subprocess.run(["python3", "tools/decomp_helper.py", func_name], capture_output=True, text=True, check=True)
        # Find the line that starts with .fn <func_name> and ends with .endfn <func_name>
        lines = result.stdout.splitlines()
        asm_lines = []
        in_fn = False
        for line in lines:
            if line.startswith(f".fn {func_name}"):
                in_fn = True
                continue
            if line.startswith(f".endfn {func_name}"):
                in_fn = False
                break
            if in_fn:
                asm_lines.append(line)
        return asm_lines
    except subprocess.CalledProcessError as e:
        print(f"Error running decomp_helper: {e.stderr}", file=sys.stderr)
        return []

def extract_dependencies(asm_lines):
    deps = set()
    for line in asm_lines:
        match = re.search(r"bl\s+([a-zA-Z0-9_]+)", line)
        if match:
            deps.add(match.group(1))
    return sorted(list(deps))

def find_signature(symbol, include_dir):
    # Search for the symbol in all headers in include_dir
    # We look for lines containing the symbol followed by ( - likely a function signature
    try:
        # Use grep to find potential signatures
        # -r: recursive, -E: regex, -h: don't print filenames
        cmd = ["grep", "-rE", rf"\b{symbol}\s*\(", include_dir]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            lines = result.stdout.splitlines()
            # Clean up: take the first match that looks like a prototype
            for line in lines:
                if ";" in line or "{" in line:
                    # Strip filename and path if grep -r output format is used
                    # But since we might want the filename for context...
                    return line.strip()
        return f"// Signature for {symbol} not found"
    except Exception as e:
        return f"// Error searching for {symbol}: {str(e)}"

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 kappa_context.py <function_name>")
        sys.exit(1)

    func_name = sys.argv[1]
    include_dir = "include"

    print(f"--- Kappa Context for {func_name} ---")
    
    asm_lines = get_function_asm(func_name)
    if not asm_lines:
        print("Could not retrieve ASM lines.")
        sys.exit(1)

    deps = extract_dependencies(asm_lines)
    print(f"\nDependencies found: {len(deps)}")
    for dep in deps:
        sig = find_signature(dep, include_dir)
        print(f"  {sig}")

    # Also suggest constants/lbls?
    lbls = set()
    for line in asm_lines:
        match = re.search(r"\b(lbl_[0-9A-Fa-f_]+|g_[a-zA-Z0-9_]+)\b", line)
        if match:
            lbls.add(match.group(1))
    
    print(f"\nSymbols found: {len(lbls)}")
    for lbl in sorted(list(lbls)):
        print(f"  {lbl}")

    print("\n--- Assembly ---")
    for line in asm_lines:
        print(line)

if __name__ == "__main__":
    main()
