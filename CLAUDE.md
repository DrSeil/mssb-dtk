# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a matching decompilation project for Mario Superstar Baseball (GameCube, GYQE01). The goal is to create C code that, when compiled with Metrowerks CodeWarrior GC/1.3.2, produces the exact same binary as the original game.

Progress is tracked at: https://decomp.dev/roeming/mssb-dtk

## Project Structure

- `src/` decompiled (or partially decompiled) C code
  - `src/game/` main game logic (~95 files, primary decompilation focus)
  - `src/Dolphin/` Dolphin SDK framework code
  - `src/C3/` C3 audio library
  - `src/Musyx/` Musyx music system
- `include/` headers for decompiled C code
  - `include/game/UnknownHomes_Game.h` major game structs (Batter, Pitcher, Runners, etc.)
  - `include/mssbTypes.h` core type definitions (BOOL, Vec3D, etc.)
- `config/GYQE01/` symbol addresses, code/data splits, and build configuration
  - `symbols.txt` and `splits.txt` for the main DOL
  - `game/`, `menus/`, `challenge/` subdirectories for REL modules
- `build/GYQE01/` generated build artifacts (asm, obj files)
- `orig/GYQE01/` original game files (main.dol, REL files)
- `tools/` custom Python decompilation and build tools
- `.agent/` AI agent workflow files and knowledge base

## Build System

The project uses **Ninja** with Python-based configuration:

```bash
# Configure (generates build.ninja)
python3 configure.py

# Build
ninja

# Build and check progress
ninja report
```

The toolchain consists of:
- **Metrowerks CodeWarrior** (GC/1.3.2) via **WiBo** wrapper on Linux
- **decomp-toolkit (dtk)** for binary manipulation and linking
- **objdiff-cli** for binary diffing and match verification
- **PowerPC binutils** for assembly

## Tools

- `./decomp.sh <function_name>` find the source file, module, and target assembly for a function.
- `./decomp.sh --m2c <function_name>` auto-decompile a function from assembly to C using m2c.
- `python3 tools/feedback_diff.py <function_name>` compare compiled output against target assembly. Reports `MATCH!` on success or line-by-line instruction differences. Can also take explicit unit: `python3 tools/feedback_diff.py <unit_name> <function_name>`.
- `python3 tools/kappa_context.py <function_name>` gather decompilation context: assembly, dependencies (bl calls), and referenced symbols.
- `python3 tools/decomp_helper.py <function_name>` locate a function's source file, address, module, and extract its assembly.
- `python3 tools/m2c_helper.py <function_name>` run m2c decompiler with full compilation context to generate initial C code.
- `python3 tools/score_functions.py [--module MODULE] [--max-size N] [--min-size N]` find the easiest unmatched functions to decompile, ranked by size.
- `python3 tools/check_pointer_arithmetic.py <file_or_directory>` detect pointer arithmetic with casts that should be replaced with struct field access. Use `--strict` to fail on violations.
- `python3 tools/find_matches.py report.json` list functions that are already 100% matched.

## Code Quality Standards

### Struct Usage

**NEVER use pointer arithmetic with manual offsets.** Always define and use proper structs.

**NEVER use void* as an argument or return type.** Always define structs to represent needed arguments.  Name the structs based off their usage.

**BAD - Pointer Arithmetic:**

```c
f32 getField(void* arg0) {
    return *(f32*)((u8*)arg0 + 0x94);
}
```

**GOOD - Proper Structs:**

```c
// In the appropriate header file
typedef struct {
    u8 _pad[0x94];
    f32 someField;  // 0x94
} SomeStruct;

f32 getField(SomeStruct* arg0) {
    return arg0->someField;
}
```

### Struct Definition Guidelines

When you see pointer arithmetic patterns like `*(type*)((u8*)ptr + offset)`:

1. **Identify the access pattern:**
   - What offset is being accessed? (e.g., `0x94` means field at offset 148)
   - Is it accessing an array element? (e.g., `idx * 36` means 36-byte elements)
   - What field within the element?

2. **Create appropriate structs:**
   - Define the struct with correct field offsets
   - Use meaningful names or `unk_0x[Offset]` naming convention
   - Always check `include/game/UnknownHomes_Game.h` for existing struct layouts

3. **Verify struct sizes:**
   - Calculate total size to ensure it matches any array multiplier
   - Ensure offsets align with the assembly's load/store offsets

### When Decompiling

If you write code with pointer arithmetic:
- **STOP immediately**
- Create proper struct definitions first
- Then write the function using struct access
- This applies even if the pointer arithmetic "works" - it's always wrong in a decompilation project

## Tasks

### Decompile a function to C code

Follow the iterative decompilation workflow:

1. **Initialize**: Run `./decomp.sh <function_name>` to identify the source file, module, and target assembly.
2. **Gather context**: Run `python3 tools/kappa_context.py <function_name>` for dependencies and symbols.
3. **Optionally get m2c output**: Run `./decomp.sh --m2c <function_name>` for an initial C approximation.
4. **Implement**: Open the identified source file and write the C implementation.
5. **Iterate** (max 10 attempts):
   a. Compile: `ninja`
   b. Diff: `python3 tools/feedback_diff.py <function_name>`
   c. If `MATCH!` → done. Otherwise analyze feedback and refine.
6. **If matched**, commit with message `matched <function_name>`.
7. **If not matched** after attempts, document the remaining differences and best attempt.

### Find easy functions to decompile

Use `python3 tools/score_functions.py` to find small, simple unmatched functions. Start with the smallest ones as they are typically easiest to match.

### Decompile a directory of functions

1. Use `python3 tools/score_functions.py --module <module>` to find the easiest function in a module.
2. Follow the "Decompile a function" workflow above.
3. If matched, commit and return to step 1.
4. If not matched after several attempts, move on to the next function.

## Compiler Behavior Patterns (Metrowerks CodeWarrior / PowerPC)

### Global Address Re-loading
The compiler often re-loads the base address of a global variable (using `lis`/`addi`) even if already in a register.
- **Trap**: Local pointers (`Batter* b = &g_Batter;`) cause the compiler to hold the address in one register, mismatching the original.
- **Fix**: Use direct global access (`g_Batter.field = ...;`) in distinct logical blocks.

### Structure Assignments and Update-Form Instructions
The binary uses "load/store with update" instructions (`lfsu`, `stfsu`).
- **Pattern**: Generated by structure-level assignments: `g_Runners[0].position = g_Batter.batterPos;`
- **Trap**: Point-by-point field assignment (`p.x = b.x; p.z = b.z;`) won't trigger update-form instructions.

### Register Steering ("Anchoring")
Registers `r31` and `r30` are typically used for major object base pointers.
- **Pattern**: Define a local pointer at the exact line where assembly first allocates the base register.
- **Trap**: Defining it at the top of a function causes the compiler to use a temporary register instead.

### ROData Ordering
Float constants are stored in `.rodata` in the order first encountered in C code.
- **Problem**: If your code uses `1.0f` before `0.0f` but the original did the reverse, rodata labels mismatch.
- **Fix**: Order your references to match the assembly's order.

### Casting and Sign Extension
- Use explicit casts to match `extsh` (sign-extend halfword) and `extsb` (sign-extend byte) instructions.
- Ensure `lwz` vs `lhz` vs `lbz` matches your struct field types.

## Verification

**IMPORTANT**: Always verify with a full build before declaring success.

```bash
ninja
python3 tools/feedback_diff.py <function_name>
```

The ONLY acceptable success condition is `MATCH!` from `feedback_diff.py`. A function that compiles without errors is NOT necessarily matched.

## Self-Review Checklist

Before declaring a decompilation complete, verify:

- [ ] No pointer arithmetic with manual offset calculations
- [ ] All struct field accesses use `->` or `.` operators
- [ ] No `void*` parameters that should be typed structs
- [ ] Struct sizes match the assembly access patterns
- [ ] `ninja` builds successfully
- [ ] `python3 tools/feedback_diff.py <function_name>` reports `MATCH!`
