
### Learnings from fn_3_B85DC
- The function uses pointer arithmetic with base struct offsets and memcpy operations, suggesting this codebase heavily relies on manual memory management and struct field access patterns.
- Parameter ordering in the final working version differs from earlier attempts, indicating that argument order matters significantly for this compiler/architecture.
- The struct field `unk48` is accessed as a pointer and used for offset calculations, suggesting pointer-to-pointer dereferencing is common in this codebase.
- The compiler accepts pointer casts like `(u8*)base->unk48` for arithmetic operations, indicating this is a standard pattern for memory access in this project.
- The function signature uses `s32` for index parameters, suggesting signed integers are preferred for array indexing in this codebase.

### Learnings from fn_3_B8414
- **Register variable declaration timing**: Global register variables (`register void* r5 asm("r5")`) must be declared before any code generation occurs. Declaring them after code has been generated causes a compiler error, indicating strict C89 compliance requirements.
- **Function parameter order sensitivity**: The compiler appears to be sensitive to the order of `src` and `dest` parameters in `memcpy` calls.
- **Build system behavior**: The build process is highly sensitive to compiler errors; all files must compile successfully.
- **Compiler optimization flags**: The use of `-O4,p` optimization level and specific flags like `-inline auto` and `-str reuse,readonly` suggests the codebase relies on aggressive optimization.

### Learnings from fn_3_2ACD8
- Global register variables must be declared at the very top of the function to avoid "cannot declare global register variables after code has been generated" errors.
- Strides for arrays can be determined by observing `mulli` instructions (e.g., `mulli ..., 0x268` confirms a struct size of `0x268`).

### Learnings from fn_3_104A88
- The compiler enforces strict C89 typing rules, particularly around implicit conversions between pointer types.
- External symbol declarations in shared headers must be properly terminated with a semicolon.

### Session Learnings (March 2026)

#### MWCC 2.6 Syntax & Sanity Checks
- **Mandatory C Syntax**: NEVER attempt to treat arrays as lvalues (e.g., `global.array += 1` is ILLEGAL). If assembly shows a pointer increment, use an explicit local pointer variable (`u8 *p = global.array; p++;`).
- **Array Indexing Priority**: Always prioritize standard array indexing (`array[i]`) over manual pointer increments unless the assembly explicitly shows a separate register being used for the running pointer.
- **Pre-Execution Sanity Check**: Before applying code, verify that it follows standard C syntax. If a "learning" suggests a pattern that violates C rules, IGNORE it.

#### Handling Compiler Errors
- **Stop-and-Think on Syntax Errors**: If the compiler reports `not an lvalue`, `illegal operand`, or `syntax error`, DO NOT repeat the same code pattern. These errors indicate a fundamental C syntax violation, not a logic mismatch.
- **Error Priority**: Treat build errors as higher priority than assembly mismatches. A non-compiling solution is never a valid step toward a match.

#### Instruction Scheduling vs. Logic
- Logically correct C code may still fail to match 100% due to MWCC 2.6's aggressive instruction interleaving. The compiler often moves loads earlier to hide pipeline stalls. If logical attempts fail, focus on reordering C statements or using temporary variables to guide the scheduler.
- **Register Swapping**: Even with perfect logic, the compiler may choose to swap the registers assigned to local variables. This can sometimes be resolved by changing the order of variable declarations or initialization.
- **Handling Identical Functions (Folding)**: The compiler may "fold" identical functions. To verify matching, temporarily change a constant or add a unique global access.
- **Function Address Gaps**: Gaps between matched functions in the `.text` section must be filled with stubs to maintain correct address offsets and ordering.
- **Pointer Arithmetic vs. Array Indexing**: The target assembly's register reloads often depend on whether a value was accessed via `array[i]` or `*ptr++`.
- **Mandatory Prototypes**: MWCC 2.6 requires prototypes for all functions (due to `-requireprotos`).

### Running Pointer Auto-Generation vs. Explicit Pointer Variables (MWCC 2.6)
- **Index-based access beats explicit running pointer**: When a loop iterates over repeated struct members, using `global.array[i].field` (no named pointer variable) lets the compiler auto-generate the running pointer with optimal prologue scheduling. An explicit `Type *p = &global; p->field` produces an extra `addi r0 + mr rN`.
- **Root cause of extra `mr`**: With a named pointer variable initialized to a global address, MWCC computes the address into a scratch register (r0) first, then `mr` copies it to the callee-saved register.
- **Relocation addend stays 0**: Index-based access `global.array[i].field` causes the compiler to keep the running pointer base at `global` (addend 0 in the relocation).
- **Recognizing repeated struct fields as arrays**: Consolidate identically-typed fields at uniform stride into arrays.

### Struct Alignment & Explicit Padding (MWCC 2.6)
- **Verify field offsets against Ghidra**: If a struct member's offset comment doesn't match where the compiler actually places it, the alignment of an earlier member is wrong.
- **Unaligned floats stored as `u8[4]`**: Use `u8 _A[4]` for unaligned floats to prevent the compiler from adding implicit padding.
- **Explicit padding is fragile**: 4-byte-aligned types will skip past explicit padding to the next 4-byte boundary.

### Register Allocation & Address Load Ordering (MWCC 2.6)
- **Symbol Encounter Order**: Register assignment for global addresses is driven by the order in which the compiler's parser/optimizer first encounters each symbol's address being used.
- **Strategic Interleaving**: Separate declarations from assignments and interleave them to control register assignment.
- **Register/ASM Tag Ineffectiveness**: The `register` keyword and `asm("rN")` tags are often ignored or overridden.
- **OR-ing Condition Sensitivity**: The choice of `bne` vs `beq` in `||` conditions affects register reuse and instruction hoisting.

### Learnings from fn_3_119854
- When indexing into a global array, the compiler may truncate the index to a `u8` if it can prove the upper bound of the index is small.

### Learnings from fn_3_107078
- The `minigameControlStruct` field within `MiniGameStruct` is likely an array.
- Direct pointer arithmetic on `g_Minigame` with an offset and cast to `u8*` is sometimes used, implying dynamic access or less structured data blocks.
- Offsets in global structs must account for padding fields, which are frequently used as flags or state markers.
- Pointer arithmetic requires explicit casting to specific byte-sized types (`u8*`/`s8*`) to read/write individual bytes.
- Padding fields within structs are writable and commonly used for runtime state flags.
