import subprocess
import sys
import argparse
import os
import re

def check_return_type_mismatch(location_output, func_name):
    """Warn if the assembly loads a pointer into r3 before blr but the source declares void."""
    # Parse source file and assembly block from decomp_helper output
    source_file = None
    asm_lines = []
    in_asm = False

    for line in location_output.splitlines():
        if line.startswith("Source: "):
            source_file = line[len("Source: "):].strip()
        if "Target ASM:" in line:
            in_asm = True
            continue
        if in_asm:
            asm_lines.append(line)

    if not asm_lines:
        return

    # "lis r3," loading a global address before blr is a strong indicator of a pointer return.
    has_pointer_return = any(re.search(r'\blis\s+r3,', line) for line in asm_lines)
    if not has_pointer_return:
        return

    # Find the .c source file and check for a void declaration.
    if not source_file:
        return
    src_path = os.path.join("src", source_file)
    if not os.path.exists(src_path):
        return

    with open(src_path, 'r') as f:
        content = f.read()

    void_pattern = re.compile(r'\bvoid\s+' + re.escape(func_name) + r'\s*\(')
    if void_pattern.search(content):
        print("=" * 60)
        print("SIGNATURE WARNING: Assembly loads a pointer into r3 before blr,")
        print(f"but '{func_name}' is declared as void in {src_path}.")
        print()
        print("ACTION REQUIRED before writing the function body:")
        print("  1. Determine the actual return type (check in_game.h / Ghidra FUN_ export).")
        print("  2. Update the declaration in the header AND the definition in the .c file.")
        print("  3. Only then implement the function body with 'return <value>;'.")
        print("=" * 60)


def run_command(cmd):
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        return f"Error: {e.stderr}"

def main():
    parser = argparse.ArgumentParser(description='Initialize decompilation for a function')
    parser.add_argument('function', help='Function name')
    parser.add_argument('--slim', action='store_true',
                        help='Skip kappa context (assembly + m2c only). Use for small/simple functions.')
    args = parser.parse_args()

    func = args.function
    slim = args.slim

    print(f"--- Initializing Decompilation for {func} ---")

    # 1. Decomp Helper (Locate and Assembly)
    steps = "1/2" if slim else "1/3"
    print(f"\n[{steps}] Locating source and assembly...")
    location = run_command(["python3", "tools/decomp_helper.py", func])
    print(location)

    # Signature check: warn if assembly returns a pointer but the function is declared void.
    # Heuristic: "lis r3," before blr is a reliable indicator of a pointer return value.
    check_return_type_mismatch(location, func)

    # 2. Kappa Context (Dependencies and Symbols) — skipped in slim mode
    if not slim:
        print("\n[2/3] Gathering context (symbols, dependencies)...")
        context = run_command(["python3", "tools/kappa_context.py", func])
        print(context)

    # 3. M2C (Initial C Implementation)
    m2c_step = "2/2" if slim else "3/3"
    print(f"\n[{m2c_step}] Generating initial C code with m2c...")
    m2c = run_command(["python3", "tools/m2c_helper.py", func])
    print("\n--- Suggested C Implementation ---")
    print(m2c)
    print("\n------------------------------------------------")

if __name__ == "__main__":
    main()
