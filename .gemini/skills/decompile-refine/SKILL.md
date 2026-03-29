---
name: decompile-refine
description: Iteratively refine decompiled C code to match assembly by fixing build errors, register mismatches, and structural differences.
---

# Decompilation Iterative Refinement

This skill guides the iterative process of building, diffing, and fixing C code to achieve a 100% binary match with Metrowerks GC/1.3.2.

## Refinement Loop

Repeat up to 10 times:

1. **Build**: Run `ninja build/GYQE01/src/<module>/<file>.o` for faster verification of your changes.
2. **Fix Build Errors/Warnings**: If compilation fails or reports "missing prototype," update the source and header.
3. **Diff**: Run `python3 tools/feedback_diff.py <function_name>`. This is the primary verification tool.
4. **Check Result**: If `MATCH!`, proceed to `decompile-commit`.
5. **Analyze Feedback**: Use the line-by-line assembly diff to identify:
   - Register mismatches (e.g., using `r31` instead of `r30`).
   - Instruction mismatches (e.g., `extsh` vs `extsb`).
   - Constant mismatches (e.g., float ordering in `.rodata`).
   - Offset mismatches (incorrect struct field access).
6. **Apply Refinements**: Update C code based on feedback.

### If Stuck (After 10 Iterations)

If you cannot achieve a match after 10 iterations, skip the function to move on to easier targets:
1. **Run Skip Script**: `./tools/skip_function.py <function_name>`
   - This will save your current progress to a branch `skip/<function_name>` and add the function to `config/skip_list.txt`.
2. **Pick a New Function**: Run `python3 tools/score_functions.py` to find the next easiest function. The skipped function will be filtered out.

## Key Refinement Principles

### 1. MWCC 2.6 Patterns & Constraints
- **CRITICAL**: NEVER use anonymous structs for pointer casting (e.g., `(struct { ... }*)`). MWCC 2.6 treats every anonymous struct definition as a unique type, leading to 'illegal implicit conversion' errors. Always use a named `typedef`.
- **Header Ordering**: When defining new types, ensure the `typedef` precedes any `extern` declarations or function prototypes that use that type. Failure to do so causes 'Shared Header Corruption' in `UnknownHeaders.h`.
- **Register Anchoring**: If the assembly uses a specific register (like `r6`) as a base for multiple struct accesses, use a local pointer variable (`StructType *p = &GlobalSymbol;`) to force the compiler to allocate a register for the base address and match the lis/addi sequence.
- **C89 strict**: Declare all variables at the top of each scope. Arrays are never lvalues (e.g., `array += 1` is illegal).

### 2. Struct Access Over Pointer Arithmetic
- **RULE**: NEVER use manual offsets like `*(u32*)((u8*)ptr + 0x94)`.
- **FIX**: Define a struct in `include/game/UnknownHomes_Game.h` and use `ptr->field`.

### 2. Register Steering ("Anchoring")
- If `r31` or `r30` are expected but not used, move the local pointer definition to the exact line where the assembly first allocates the base register.

### 3. Global Address Re-loading
- The compiler re-loads globals often. Use direct global access (`g_Batter.field`) in distinct blocks to trigger re-loads.

### 4. Load/Store with Update (`lfsu`, `stfsu`)
- Triggered by structure-level assignments: `target.vec = source.vec;`.
- Individual field assignments (`v.x = s.x; v.y = s.y;`) will FAIL to match these instructions.

### 5. ROData Ordering
- Floating-point literals are stored in the order they appear in C. Order your code's float references to match the assembly's order.

### 7. Permuter for Register Steering
- If manual anchoring fails, use the permuter: `python3 tools/permuter.py <function_name>`.
- **Iteration Limit**: By default, it runs for 100 attempts. You can override this with `-n <number>` (e.g., `-n 500`).
- The permuter will generate context automatically and try various code transformations.
- Check `nonmatching/<function_name>/output-<score>-<count>/source.c` for successful attempts.
