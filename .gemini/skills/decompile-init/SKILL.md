---
name: decompile-init
description: Initialize decompilation for a specific function by gathering context, assembly, and initial C implementation using m2c.
---

# Decompilation Initialization

This skill guides the initial steps of decompiling a function in the `mssb-dtk` project.

## Workflow

### 1. Locate and Extract Assembly
Run `./decomp.sh <function_name>` to identify the source file, module, and target assembly.

### 2. Gather Context
Run `python3 tools/kappa_context.py <function_name>` to gather:
- Function signatures
- Symbols (labels and functions)
- Assembly listing
- External dependencies (branch calls)

### 3. Generate Initial C Implementation
Use the `m2c` helper to get a starting point:
`./decomp.sh --m2c <function_name>`

### 4. Setup Environment
1. Identify the target source file (e.g., `src/game/game_batter.c`).
2. Add the function prototype and the `m2c` generated code to the file.
3. Ensure necessary headers are included (e.g., `include/game/UnknownHomes_Game.h`).

## Tools Reference

- `./decomp.sh <function_name>`: Main entry point for finding and decompiling.
- `python3 tools/kappa_context.py <function_name>`: Gathers detailed context.
- `python3 tools/decomp_helper.py <function_name>`: Locate source file and address.
- `python3 tools/m2c_helper.py <function_name>`: Runs m2c decompiler.
