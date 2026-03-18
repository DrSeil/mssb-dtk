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
    args = parser.parse_args()
    
    func = args.function
    
    print(f"--- Initializing Decompilation for {func} ---")
    
    # 1. Decomp Helper (Locate and Assembly)
    print("\n[1/3] Locating source and assembly...")
    location = run_command(["python3", "tools/decomp_helper.py", func])
    print(location)
    
    # 2. Kappa Context (Dependencies and Symbols)
    print("\n[2/3] Gathering context (symbols, dependencies)...")
    context = run_command(["python3", "tools/kappa_context.py", func])
    print(context)
    
    # 3. M2C (Initial C Implementation)
    print("\n[3/3] Generating initial C code with m2c...")
    m2c = run_command(["python3", "tools/m2c_helper.py", func])
    print("\n--- Suggested C Implementation ---")
    print(m2c)
    print("\n------------------------------------------------")

if __name__ == "__main__":
    main()
