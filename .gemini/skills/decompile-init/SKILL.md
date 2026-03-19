---
name: decompile-init
description: Initialize decompilation for a specific function by gathering context, assembly, and initial C implementation using m2c.
---

# Decompilation Initialization

This skill guides the initial steps of decompiling a function in the `mssb-dtk` project.

## Workflow

### 0. Choose a Function
If you don't have a function to decompile, run:
`python3 tools/score_functions.py --limit 1000 | grep <module>`
- This will show the easiest unmatched functions in a specific module (e.g., `game`).
- Functions in `config/skip_list.txt` are automatically filtered out.

### 1. Run Comprehensive Initialization
Run the initialization script from the root to gather ALL context in one call:
`python3 skills/decompile-init/scripts/initialize_function.py <function_name>`

This will automatically:
- Identify the source file and target assembly.
- Gathers dependencies (bl calls) and symbols (labels).
- Generates an initial C implementation using `m2c`.

**Note on Address Gaps**: If the function's address (e.g., `0x8B2E4`) is before the first function already in the source file, check `config/GYQE01/<module>/splits.txt` to confirm the file's start address. You may need to add stubs or other functions at the top of the file to maintain correct ordering.

### 2. Setup Environment
1. Identify the target source file from the script output (e.g., `src/game/game_batter.c`).
2. Add the function prototype to the header (e.g., `include/game/m_sound.h`) and the `m2c` generated code to the source file.
3. Ensure necessary headers are included.
4. Verify symbol visibility with `nm build/GYQE01/src/<module>/<file>.o | grep <func_name>`.

## Tools Reference

- `./decomp.sh <function_name>`: Main entry point for finding and decompiling.
- `python3 tools/kappa_context.py <function_name>`: Gathers detailed context.
- `python3 tools/decomp_helper.py <function_name>`: Locate source file and address.
- `python3 tools/m2c_helper.py <function_name>`: Runs m2c decompiler.
