---
name: decompile-refine
description: Iteratively refine decompiled C code to match assembly by fixing build errors, register mismatches, and structural differences.
---

# Decompilation Iterative Refinement

This skill guides the iterative process of building, diffing, and fixing C code to achieve a 100% binary match with Metrowerks GC/1.3.2.

## Refinement Loop

Repeat up to 10 times:

1. **Build**: Run `ninja`.
2. **Fix Build Errors**: If compilation fails, fix the C code.
3. **Diff**: Run `python3 tools/feedback_diff.py <function_name>`.
4. **Check Result**: If `MATCH!`, proceed to `decompile-commit`.
5. **Analyze Feedback**: Use the line-by-line assembly diff to identify:
   - Register mismatches (e.g., using `r31` instead of `r30`).
   - Instruction mismatches (e.g., `extsh` vs `extsb`).
   - Constant mismatches (e.g., float ordering in `.rodata`).
   - Offset mismatches (incorrect struct field access).
6. **Apply Refinements**: Update C code based on feedback.

## Key Refinement Principles

### 1. Struct Access Over Pointer Arithmetic
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

### 6. Casting and Type Matching
- Use explicit casts to match sign-extension instructions like `extsh` (sign-extend halfword).
- **Match Register Loads**: Use `u16` to trigger `lhz` (load halfword zero) or `s16` to trigger `lha` (load halfword algebraic/sign-extended).

### 8. Signed Division by Constant
- The compiler optimizes signed division by a constant using `mulhw` (multiply high word) followed by shifts and adds.
- If you see `mulhw` followed by `add` and `srawi`, it's likely a signed division in C.
- Example: `(arg + 6) / 7` matches a specific `mulhw` sequence for division by 7.
