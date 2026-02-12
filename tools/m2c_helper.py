#!/usr/bin/env python3

import os
import sys
import subprocess
import argparse
import tempfile
from pathlib import Path

# Add tools directory to path so we can import decomp_helper and decompctx
script_dir = Path(__file__).parent.resolve()
root_dir = script_dir.parent
sys.path.append(str(script_dir))

import decomp_helper
import decompctx

def main():
    parser = argparse.ArgumentParser(description='m2c Decompilation Wrapper')
    parser.add_argument('function', help='Function name to decompile')
    parser.add_argument('--target', default='ppc-mwcc-c', help='m2c target (default: ppc-mwcc-c)')
    args = parser.parse_args()

    # 1. Find the assembly for the function using decomp_helper logic
    config_base = root_dir / "config/GYQE01"
    
    found_symbol = None
    module = None
    
    # Check main symbols
    symbols = decomp_helper.parse_symbols(str(config_base / 'symbols.txt'))
    if args.function in symbols:
        found_symbol = symbols[args.function]
        module = ""
    else:
        # Check modules
        for m in ['game', 'menus', 'challenge']:
            sys_file = config_base / m / 'symbols.txt'
            symbols = decomp_helper.parse_symbols(str(sys_file))
            if args.function in symbols:
                found_symbol = symbols[args.function]
                module = m
                break
    
    if not found_symbol:
        print(f"Error: Function {args.function} not found in symbols.", file=sys.stderr)
        sys.exit(1)

    addr = found_symbol['addr']
    found_file = None
    
    if module == "":
        splits = decomp_helper.parse_splits(str(config_base / 'splits.txt'))
        found_file = decomp_helper.find_file_for_address(splits, addr)
    else:
        splits = decomp_helper.parse_splits(str(config_base / module / 'splits.txt'))
        found_file = decomp_helper.find_file_for_address(splits, addr)

    if not found_file:
        print(f"Error: Could not find source file for {args.function} at {hex(addr)}", file=sys.stderr)
        sys.exit(1)

    source_path = root_dir / "src" / found_file
    
    # 2. Extract ASM to a temporary file
    rel_path = os.path.splitext(found_file)[0] + ".s"
    if module:
        asm_path = root_dir / f"build/GYQE01/{module}/asm" / rel_path
    else:
        asm_path = root_dir / "build/GYQE01/asm" / rel_path
        
    asm_content = decomp_helper.extract_asm(str(asm_path), args.function)
    if not asm_content:
        # Fallback search as in decomp_helper
        search_root = root_dir / f"build/GYQE01/{module}/asm" if module else root_dir / "build/GYQE01/asm"
        if search_root.exists():
            for root, dirs, files in os.walk(search_root):
                for file in files:
                    if file.endswith('.s'):
                        test_path = os.path.join(root, file)
                        asm_content = decomp_helper.extract_asm(test_path, args.function)
                        if asm_content:
                            break
                if asm_content: break

    if not asm_content:
        print(f"Error: Could not extract ASM for {args.function}", file=sys.stderr)
        sys.exit(1)

    # 3. Generate context using decompctx
    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as ctx_file:
        ctx_path = ctx_file.name
        
    try:
        # Configure decompctx globals
        decompctx.root_dir = str(root_dir)
        decompctx.include_dirs = [
            str(root_dir / "include"),
            str(root_dir / "include/stl"),
            str(root_dir / "build/GYQE01/include"),
        ]
        
        # Generate context
        raw_context = decompctx.import_c_file(str(source_path))
        
        # Preprocess context using cpp to resolve macros and guards
        cpp_cmd = [
            "cpp",
            "-P",       # Inhibit generation of linemarkers
            "-undef",    # Do not predefine any system-specific or GCC-specific macros
            "-D__extension__=",
            "-D__attribute__(x)=",
            "-D__inline__=inline",
            "-D__const=const",
            "-D__restrict=restrict",
            "-D__powerpc__",
            "-D__BIG_ENDIAN__",
            "-D__PPC__",
            "-DM2C",
        ]
        for inc in [
            str(root_dir / "include"),
            str(root_dir / "include/stl"),
            str(root_dir / "build/GYQE01/include"),
        ]:
            cpp_cmd.extend(["-I", inc])
        
        try:
            cpp_result = subprocess.run(cpp_cmd, input=raw_context, capture_output=True, text=True)
            if cpp_result.returncode == 0:
                clean_context = cpp_result.stdout
            else:
                print(f"Warning: cpp failed, falling back to basic stripping: {cpp_result.stderr}", file=sys.stderr)
                # Fallback stripping logic
                clean_context_lines = []
                in_directive = False
                for line in raw_context.splitlines():
                    stripped = line.strip()
                    if not in_directive:
                        if stripped.startswith("#"):
                            if stripped.endswith("\\"): in_directive = True
                            continue
                        clean_context_lines.append(line)
                    else:
                        if not stripped.endswith("\\"): in_directive = False
                        continue
                clean_context = "\n".join(clean_context_lines)
        except Exception as e:
            print(f"Warning: could not run cpp: {e}", file=sys.stderr)
            clean_context = raw_context

        # Final pass to remove extern "C" which m2c's C parser hates
        final_context = []
        for line in clean_context.splitlines():
            s = line.strip()
            if s == 'extern "C" {' or s == 'extern "C"':
                continue
            final_context.append(line)
        
        with open(ctx_path, 'w') as f:
            f.write("\n".join(final_context))

        # 4. Run m2c
        with tempfile.NamedTemporaryFile(mode='w', suffix='.s', delete=False) as asm_temp:
            asm_temp.write(asm_content)
            asm_temp_path = asm_temp.name

        try:
            cmd = [
                "python3",
                str(root_dir / "tools/m2c_repo/m2c.py"),
                "--target", args.target,
                "--context", ctx_path,
                "--function", args.function,
                asm_temp_path
            ]
            
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode != 0:
                print(f"m2c failed with exit code {result.returncode}", file=sys.stderr)
                print("--- m2c stderr ---", file=sys.stderr)
                print(result.stderr, file=sys.stderr)
                print("--- m2c stdout ---", file=sys.stderr)
                print(result.stdout, file=sys.stderr)
                print(f"DEBUG: Context kept at {ctx_path}", file=sys.stderr)
                print(f"DEBUG: ASM kept at {asm_temp_path}", file=sys.stderr)
                sys.exit(1)
            
            print(result.stdout)
            # Cleanup only on success
            if os.path.exists(asm_temp_path):
                os.remove(asm_temp_path)
            if os.path.exists(ctx_path):
                os.remove(ctx_path)
        except Exception as e:
            print(f"Error running m2c: {e}", file=sys.stderr)
            sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
