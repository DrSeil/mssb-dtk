---
name: decompile-init
description: Initialize decompilation for a specific function by gathering context, assembly, and initial C implementation using m2c.
---

# Decompilation Initialization

This skill guides the initial steps of decompiling a function in the `mssb-dtk` project.

## Workflow

### 1. Run Comprehensive Initialization
Run the initialization script from the root to gather ALL context in one call:
`python3 skills/decompile-init/scripts/initialize_function.py <function_name>`

This will automatically:
- Identify the source file and target assembly.
- Gathers dependencies (bl calls) and symbols (labels).
- Generates an initial C implementation using `m2c`.

### 2. Setup Environment
1. Identify the target source file from the script output (e.g., `src/game/game_batter.c`).
2. Add the function prototype and the `m2c` generated code to the file.
3. Ensure necessary headers are included (e.g., `include/game/UnknownHomes_Game.h`).

## Tools Reference

- `./decomp.sh <function_name>`: Main entry point for finding and decompiling.
- `python3 tools/kappa_context.py <function_name>`: Gathers detailed context.
- `python3 tools/decomp_helper.py <function_name>`: Locate source file and address.
- `python3 tools/m2c_helper.py <function_name>`: Runs m2c decompiler.
