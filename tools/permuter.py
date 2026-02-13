#!/usr/bin/env python3
import os
import sys
import subprocess
import argparse
import shutil
from pathlib import Path

# Add project tools to path so we can import decomp_helper
sys.path.append(os.path.join(os.path.dirname(__file__)))
from decomp_helper import parse_symbols, parse_splits, find_file_for_address, extract_asm

def get_function_info(func_name):
    base_dir = "."
    config_base = os.path.join(base_dir, "config/GYQE01")
    
    # Try to find symbol
    symbol_files = [
        os.path.join(config_base, 'symbols.txt'),
        os.path.join(config_base, 'game/symbols.txt'),
        os.path.join(config_base, 'menus/symbols.txt'),
        os.path.join(config_base, 'challenge/symbols.txt')
    ]
    
    found_symbol = None
    module = None
    
    for s_file in symbol_files:
        if not os.path.exists(s_file): continue
        symbols = parse_symbols(s_file)
        if func_name in symbols:
            found_symbol = symbols[func_name]
            module = os.path.basename(os.path.dirname(s_file))
            if module == 'GYQE01': module = ""
            break
            
    if not found_symbol:
        return None

    addr = found_symbol['addr']
    split_file = os.path.join(config_base, module, 'splits.txt') if module else os.path.join(config_base, 'splits.txt')
    splits = parse_splits(split_file)
    found_file = find_file_for_address(splits, addr)
    
    if not found_file:
        return None

    # Map to ASM path
    rel_path = os.path.splitext(found_file)[0] + ".s"
    if module:
        asm_path = os.path.join(base_dir, f"build/GYQE01/{module}/asm", rel_path)
    else:
        asm_path = os.path.join(base_dir, "build/GYQE01/asm", rel_path)
        
    return {
        "source": found_file,
        "asm_path": asm_path,
        "module": module,
        "addr": addr
    }

def extract_c_func(source_file, func_name):
    if not os.path.exists(source_file):
        return None
    
    with open(source_file, 'r') as f:
        content = f.read()
        
    search_str = f"{func_name}("
    idx = content.find(search_str)
    while idx != -1:
        # Check if it's a definition: preceded by a type and not followed by a semicolon before {
        # We'll just look for the first occurrence that has a { before a ;
        brace_pos = content.find('{', idx)
        semi_pos = content.find(';', idx)
        
        if brace_pos != -1 and (semi_pos == -1 or brace_pos < semi_pos):
            # Found it! Now find where the declaration starts (backtrack to newline)
            start_pos = content.rfind('\n', 0, idx)
            if start_pos == -1: start_pos = 0
            else: start_pos += 1
            
            # Find matching brace
            brace_count = 0
            for i in range(brace_pos, len(content)):
                if content[i] == '{':
                    brace_count += 1
                elif content[i] == '}':
                    brace_count -= 1
                    if brace_count == 0:
                        return content[start_pos:i+1]
        
        idx = content.find(search_str, idx + 1)
        
    return None

def main():
    parser = argparse.ArgumentParser(description='Run decomp-permuter for a function')
    parser.add_argument('function', help='Function name to permute')
    parser.add_argument('-j', '--jobs', type=int, default=1, help='Number of jobs')
    args, unknown = parser.parse_known_args()

    info = get_function_info(args.function)
    if not info:
        print(f"Error: Could not find function {args.function}")
        sys.exit(1)

    # Adjust source path if it doesn't exist
    if not os.path.exists(info['source']):
        # Maybe it's src/ + source?
        test_path = os.path.join("src", info['source'])
        if os.path.exists(test_path):
            info['source'] = test_path
        else:
            print(f"Error: Source file {info['source']} does not exist")
            sys.exit(1)

    print(f"Found function in {info['source']}")
    
    # Create permuter directory
    perm_dir = Path(f"nonmatching/{args.function}")
    perm_dir.mkdir(parents=True, exist_ok=True)
    
    # 1. Extract ASM
    asm = extract_asm(info['asm_path'], args.function)
    if not asm:
        # Try search
        print("Direct ASM extraction failed, searching...")
        search_root = os.path.dirname(info['asm_path'])
        if os.path.exists(search_root):
            for root, dirs, files in os.walk(search_root):
                for file in files:
                    if file.endswith('.s'):
                        asm = extract_asm(os.path.join(root, file), args.function)
                        if asm: break
                if asm: break
    
    if not asm:
        print(f"Error: Could not extract ASM for {args.function}")
        sys.exit(1)
        
    # Clean up ASM for GAS (powerpc-eabi-as)
    # We strip .fn/.endfn but keep the label and logic
    cleaned_asm = [f".global {args.function}", f"{args.function}:"]
    for line in asm.splitlines():
        line = line.strip()
        if not line or line.startswith(".fn") or line.startswith(".endfn"):
            continue
        cleaned_asm.append(line)
        
    # Assemble target.s to target.o
    # We might need to handle unknown identifiers by declaring them extern
    max_retries = 5
    externs = set()
    
    as_path = str(Path("build/binutils/powerpc-eabi-as").absolute())
    as_cmd = [
        as_path,
        "-mgekko",
        "-mregnames",
        "-o", str(perm_dir / "target.o"),
        str(perm_dir / "target.s")
    ]
    
    for attempt in range(max_retries):
        with open(perm_dir / "target.s", 'w') as f:
            for ext in sorted(externs):
                f.write(f".extern {ext}\n")
            f.write("\n".join(cleaned_asm) + "\n")
            
        print(f"Assembling target.s (attempt {attempt + 1})...")
        result = subprocess.run(as_cmd, capture_output=True, text=True)
        if result.returncode == 0:
            break
            
        # Parse errors for "undefined symbol"
        new_externs = False
        import re
        # GAS errors look like: target.s:123: Error: undefined symbol `g_Batter' used as an immediate
        for line in result.stderr.splitlines():
            match = re.search(r"undefined symbol `(\w+)'", line)
            if match:
                sym = match.group(1)
                if sym not in externs:
                    externs.add(sym)
                    new_externs = True
        
        if not new_externs:
            print(result.stderr)
            print("Could not resolve more symbols.")
            sys.exit(1)
    else:
        print("Reached max retries for assembly.")
        sys.exit(1)
        
    # 2. Extract C
    c_source = extract_c_func(info['source'], args.function)
    if not c_source:
        print(f"Error: Could not extract C source for {args.function}")
        sys.exit(1)
        
    with open(perm_dir / "base.c", 'w') as f:
        f.write(f'#include "ctx.c"\n\n')
        f.write(c_source)
        
    # 3. Generate context
    print("Generating context...")
    ctx_path = perm_dir / "ctx.c"
    
    # We use decompctx.py which is the project's standard context tool
    # It requires include paths and the source file
    ctx_cmd = [
        "python3", "tools/decompctx.py",
        info['source'],
        "-o", str(ctx_path),
        "-I", "include",
        "-I", "include/stl",
        "-D", "NDEBUG=1"
    ]
    subprocess.run(ctx_cmd, check=True)

    # To avoid redefinition errors, we should comment out the function body in ctx.c
    # since decompctx.py inlines the whole source file.
    with open(ctx_path, 'r') as f:
        ctx_lines = f.readlines()
    
    with open(ctx_path, 'w') as f:
        in_fn = False
        brace_count = 0
        search_str = f"{args.function}("
        for line in ctx_lines:
            if not in_fn and search_str in line and "{" in line and ";" not in line:
                in_fn = True
                f.write(f"// [PERMUTER] Commented out redefinition of {args.function}\n")
                f.write("// " + line)
                brace_count = line.count('{') - line.count('}')
                if brace_count == 0: in_fn = False
                continue
            
            if in_fn:
                f.write("// " + line)
                brace_count += line.count('{') - line.count('}')
                if brace_count <= 0:
                    in_fn = False
                continue
            
            f.write(line)

    # 4. Create .sh file for permuter
    # We need the exact flags used for this file.
    # For now, let's use a generic set from permuter_settings.toml
    # but we could parse build.ninja for more accuracy.
    
    wibo_path = str(Path("build/tools/wibo").absolute())
    compiler_path = str(Path("build/compilers/GC/1.3.2/mwcceppc.exe").absolute())
    include_path = str(Path("include").absolute())
    stl_include_path = str(Path("include/stl").absolute())
    
    with open(perm_dir / "compile.sh", 'w') as f:
        f.write("#!/bin/bash\n")
        # Handle cases where $2 is '-o'
        f.write('if [ "$2" == "-o" ]; then\n')
        f.write(f'    {wibo_path} {compiler_path} -proc gekko -nodefaults -Cpp_exceptions off -RTTI off -fp hard -fp_contract on -O4,p -maxerrors 1 -enum int -inline auto -str reuse,readonly -nosyspath -use_lmw_stmw on -sdata 8 -sdata2 8 -i {include_path} -i {stl_include_path} -DNDEBUG=1 -c "$1" -o "$3"\n')
        f.write('else\n')
        f.write(f'    {wibo_path} {compiler_path} -proc gekko -nodefaults -Cpp_exceptions off -RTTI off -fp hard -fp_contract on -O4,p -maxerrors 1 -enum int -inline auto -str reuse,readonly -nosyspath -use_lmw_stmw on -sdata 8 -sdata2 8 -i {include_path} -i {stl_include_path} -DNDEBUG=1 -c "$1" -o "$2"\n')
        f.write('fi\n')
    
    os.chmod(perm_dir / "compile.sh", 0o755)

    objdump_path = str(Path("build/binutils/powerpc-eabi-objdump").absolute())
    # 5. Create .toml file
    with open(perm_dir / "settings.toml", 'w') as f:
        f.write(f'compiler_type = "mwcc"\n')
        f.write(f'target_o = "target.o"\n')
        f.write(f'source_c = "base.c"\n')
        f.write(f'context_file = "ctx.c"\n')
        f.write(f'compile_script = "./compile.sh"\n')
        f.write(f'objdump_command = "{objdump_path} -Drz"\n')

    # 6. Run permuter
    print(f"Starting permuter in {perm_dir}...")
    permuter_path = Path("tools/decomp-permuter/permuter.py").absolute()
    
    cmd = [sys.executable, str(permuter_path), ".", "-j", str(args.jobs)] + unknown
    subprocess.run(cmd, cwd=perm_dir)

if __name__ == "__main__":
    main()
