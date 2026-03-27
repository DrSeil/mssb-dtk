# MWCC 2.6 Decompilation Key Learnings

## Syntax & Compliance
- **C89 strict**: Declare all variables at the top of each scope. Arrays are never lvalues (`array += 1` is illegal — use `u8 *p = array; p++;`). Implicit pointer conversions are often disallowed.
- **-requireprotos**: All functions need prototypes before use. Update headers before building.
- **Encoding**: Shift JIS bytes in source files cause build failures. Check for non-ASCII if the compiler chokes on a clean-looking file.
- **On syntax errors**: `not an lvalue`, `illegal operand`, `syntax error` are fundamental violations. Stop and reconsider — don't tweak a broken pattern.

## Codegen & Scheduling
- **No temp variables by default**: Write clean direct expressions first. Only add temps if the diff reveals a specific register allocation problem — unnecessary temps cause extra registers and wrong scheduling.
- **Instruction interleaving**: MWCC moves loads and address calculations early to hide pipeline stalls. If logic is correct but ordering differs, reorder C statements or use a targeted temp to guide the scheduler.
- **`register` and `asm()` ignored**: The optimizer overrides these. Focus on code structure.

## Loops & Register Allocation
- **Do-while for running pointers**: Prefer `do-while` when the assembly shows a running pointer (`stb (r4); addi r4, r4, 1`). `for` loops often trigger unrolling or induction variables that don't match.
- **Declaration order steers registers**: Register allocation follows symbol encounter order. Interleave declarations and assignments (`decl v1; init v2; assign v1;`) to steer register assignment.

## Pointers & Array Access
- **Index over explicit pointer**: `global.array[i].field` lets the compiler auto-generate a running pointer with optimal prologue scheduling. `Type *p = &global; p->field` often produces an extra `mr`.
- **Mirror the assembly pattern**: Match `*ptr++` vs `ptr[i]` exactly. A register reload after an increment means your access pattern differs.

## Structs & Alignment
- **Verify offsets against Ghidra**: Drifting offsets mean an earlier member's alignment is wrong.
- **Unaligned floats**: A `float` at an unaligned offset (e.g., `0x0A`) must be defined as `u8 _A[4]` — implicit padding would shift everything after it.
- **Repeated fields → arrays**: Group identically-typed fields at uniform strides into arrays.
- **Explicit array sizes**: Always provide explicit sizes for array fields in structs.

## Patterns & Edge Cases
- **Identical function folding**: The compiler may fold identical functions. Add a unique constant or global access temporarily to force a distinct implementation for verification.
- **Text gaps**: Fill gaps between matched functions in `.text` with stubs to maintain correct alignment.
- **Boolean/OR sensitivity**: In `||` conditions, `bne` vs `beq` choice affects register reuse and instruction hoisting — match the branch direction exactly.
- **2-arg call of 3-arg fn**: Under `-requireprotos`, calling a globally-declared 3-arg function with 2 args is impossible without breaking all other callers. Accept as NONMATCHING (see `rep_3448.c`, `fn_3_1580AC`).
- **s32 vs int type mismatch**: In MWCC, `s32 = signed long` ≠ `int`. Passing `s32*` to an `int*` parameter is an error. Declare the variable as `int` when the callee signature uses `int*`.
