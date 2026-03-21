
### Learnings from fn_3_B85DC
- The function uses pointer arithmetic with base struct offsets and memcpy operations, suggesting this codebase heavily relies on manual memory management and struct field access patterns
- Parameter ordering in the final working version differs from earlier attempts, indicating that argument order matters significantly for this compiler/architecture
- The struct field `unk48` is accessed as a pointer and used for offset calculations, suggesting pointer-to-pointer dereferencing is common in this codebase
- The compiler accepts pointer casts like `(u8*)base->unk48` for arithmetic operations, indicating this is a standard pattern for memory access in this project
- The function signature uses `s32` for index parameters, suggesting signed integers are preferred for array indexing in this codebase
- The final working version has different parameter names (`src2, src`) compared to earlier attempts, indicating parameter naming consistency may affect compilation or is required for matching calling conventions

### Learnings from fn_3_B8414
- **Register variable declaration timing**: Global register variables (`register void* r5 asm("r5")`) must be declared before any code generation occurs. Declaring them after code has been generated causes a compiler error, indicating strict C89 compliance requirements.

- **Function parameter order sensitivity**: The compiler appears to be sensitive to the order of `src` and `dest` parameters in `memcpy` calls, as different orderings produced different similarity scores (65.0% vs 62.7%), suggesting potential type or calling convention mismatches.

- **Build system behavior**: The build process is highly sensitive to compiler errors, with a single error causing the entire build to abort. This indicates that all files must compile successfully, and partial builds are not supported.

- **Compiler optimization flags**: The use of `-O4,p` optimization level and specific flags like `-inline auto` and `-str reuse,readonly` suggests the codebase relies on aggressive optimization and specific memory handling behaviors that must be preserved in decompilation.

- **Platform-specific constraints**: The use of PowerPC-specific features (Gekko processor, register constraints) indicates that any decompiled code must maintain platform-specific calling conventions and register usage patterns.

### Learnings from fn_3_2ACD8
- Global register variables must be declared at the very top of the function to avoid "cannot declare global register variables after code has been generated" errors.
- Strides for arrays can be determined by observing `mulli` instructions (e.g., `mulli ..., 0x268` confirms a struct size of `0x268`).

### Learnings from fn_3_104A88
- The compiler enforces strict C89 typing rules, particularly around implicit conversions between pointer types.
- External symbol declarations in shared headers must be properly terminated with a semicolon.
- Shared headers like `UnknownHeaders.h` are foundational; errors here block the entire project build.

### Session Learnings (March 2026)
- **Instruction Scheduling vs. Logic**: Logically correct C code (verified by data flow analysis) may still fail to match 100% due to MWCC 2.6's aggressive instruction interleaving. The compiler often moves loads earlier to hide pipeline stalls. If logical attempts fail, focus on reordering C statements or using temporary variables to guide the scheduler.
- **Register Swapping**: Even with perfect logic, the compiler may choose to swap the registers assigned to local variables (e.g., `r5` vs `r6`). This can sometimes be resolved by changing the order of variable declarations or initialization.
- **Handling Identical Functions (Folding)**: The compiler may "fold" identical functions into a single implementation to save space. To verify matching for one of several identical functions, temporarily change a constant or add a unique global access to force the compiler to treat them as distinct entities.
- **Function Address Gaps**: When a function is missing from a source file, always check `symbols.txt` for its address and `splits.txt` for its expected file. Gaps between matched functions in the `.text` section must be filled with stubs to maintain correct address offsets and ordering.
- **Pointer Arithmetic vs. Array Indexing**: The target assembly's register reloads often depend on whether a value was accessed via `array[i]` or `*ptr++`. Matching reloads (e.g., re-loading `z1` from an array after a pointer increment) requires mirroring the assembly's access pattern exactly in C.
- **Mandatory Prototypes**: MWCC 2.6 requires prototypes for all functions (due to `-requireprotos`). Adding or updating headers is a required step for every matched function.

### Running Pointer Auto-Generation vs. Explicit Pointer Variables (MWCC 2.6)

- **Index-based access beats explicit running pointer**: When a loop iterates over repeated struct members, using `global.array[i].field` (no named pointer variable) lets the compiler auto-generate the running pointer with optimal prologue scheduling. An explicit `Type *p = &global; p->field` produces an extra `addi r0 + mr rN` instead of a direct `addi rN`.
- **Root cause of extra `mr`**: With a named pointer variable initialized to a global address, MWCC computes the address into a scratch register (r0) first, then `mr` copies it to the callee-saved register. The auto-generated running pointer avoids this by letting the compiler schedule the `stw rN` (save old value) before `addi rN` (write new value) directly.
- **Relocation addend stays 0**: Index-based access `global.array[i].field` causes the compiler to keep the running pointer base at `global` (addend 0 in the relocation), folding all field offsets (e.g., 0xCA + 4 = 0xCE) into the load instruction immediate. This matches targets that show `addi rN, rX, global@l` + `lbz r0, 0xCE(rN)`.
- **Recognizing repeated struct fields as arrays**: When a struct has multiple identically-typed fields at uniform stride (e.g., `_CA`, `_F2`, `_11A`... at 0x28 intervals), they should be consolidated into an array. This enables the index-based pattern above and produces cleaner, more correct struct definitions.

### Struct Alignment & Explicit Padding (MWCC 2.6)

- **Verify field offsets against Ghidra**: If a struct member's offset comment doesn't match where the compiler actually places it, the alignment of an earlier member is wrong. Cross-check with Ghidra's `in_game.h` export.
- **Unaligned floats stored as `u8[4]`**: Ghidra sometimes reports a `float` at an address that's not 4-byte aligned (e.g., offset 0x0A in a struct). This means the field has no natural alignment, and the struct should define it as `u8 _A[4]` to prevent the compiler from adding implicit padding that displaces subsequent fields. The struct's max alignment then comes from its `s16` member (2 bytes), not from f32 (4 bytes).
- **Explicit padding is fragile**: Adding `u8 _padXX[N]` between fields forces the next field to start at a specific offset — but only if that field's type alignment is ≤ the padding's end address. A 4-byte-aligned type will skip past explicit padding to the next 4-byte boundary.

### Register Allocation & Address Load Ordering (MWCC 2.6)
- **Symbol Encounter Order**: Register assignment for global addresses (`lis/addi ha/l`) is driven by the order in which the compiler's parser/optimizer first encounters each symbol's address being used.
- **Strategic Interleaving**: To control which global gets which register (e.g., `r3` vs `r4`), it's often necessary to separate declarations from assignments and interleave them (e.g., `void* v1; u8 v2 = global1; v1 = global2;`). This forces the compiler to see `global1`'s address first, assigning it to a lower register like `r3` before it sees `global2`.
- **Register/ASM Tag Ineffectiveness**: The `register` keyword and `asm("rN")` tags are often ignored or overridden by MWCC 2.6's optimizer and scheduler. They should not be used as a primary solution for register mismatches; instead, focus on restructuring the C code's variable initialization and usage patterns.
- **OR-ing Condition Sensitivity**: In functions with multiple `if` conditions joined by `||`, the compiler's choice of `bne` (if any condition is true) vs `beq` (if all conditions are false) can drastically affect register reuse and instruction hoisting.
