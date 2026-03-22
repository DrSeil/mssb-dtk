import subprocess
import sys
import argparse
import os

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
