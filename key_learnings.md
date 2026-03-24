# MWCC 2.6 Decompilation Key Learnings

## Core Mandates & Syntax
- **Mandatory C Syntax**: NEVER treat arrays as lvalues (e.g., `array += 1` is ILLEGAL). Use an explicit local pointer (`u8 *p = array; p++;`) or array indexing.
- **Stop-and-Think on Syntax Errors**: If the compiler reports `not an lvalue`, `illegal operand`, or `syntax error`, it is a fundamental C violation. Stop and reconsider the structure; do not attempt to "tweak" a broken pattern.
- **Mandatory Prototypes**: MWCC 2.6 requires prototypes for all functions (due to `-requireprotos`). Always update headers.
- **Strict C89 Compliance**: Declare all variables (including global register variables) at the very top of the function scope. Implicit conversions between pointer types are often disallowed.

## Loop & Instruction Scheduling
- **Do-While Loops for Pointer Matching**: Prefer `do-while` loops over `for` loops when the assembly shows a running pointer incrementing (e.g., `stb ... (r4)` followed by `addi r4, r4, 1`). `for` loops often trigger unrolling or induction variables that don't match simple patterns.
- **Instruction Interleaving**: MWCC 2.6 moves loads/address calculations early to hide pipeline stalls. If logic is correct but instructions are out of order, try reordering C statements or using temporary variables to guide the scheduler.
- **Register Swapping/Steering**: Register allocation is driven by the order the compiler encounters symbols. Interleave declarations and assignments (e.g., `decl v1; init v2; assign v1;`) to control which global gets which register.
- **Register/ASM Tags**: The `register` keyword and `asm("rN")` tags are frequently ignored or overridden by the optimizer. Focus on code structure instead.

## Pointers & Array Access
- **Index-based Access vs. Explicit Pointers**: Using `global.array[i].field` often lets the compiler auto-generate a running pointer with optimal prologue scheduling. An explicit `Type *p = &global; p->field` may produce an extra `mr` (move register) instruction.
- **Array Indexing Priority**: Always prioritize standard array indexing (`array[i]`) over manual pointer increments unless the assembly explicitly shows a separate register being used for the running pointer.
- **Pointer Arithmetic**: If the assembly shows a register reload after an increment, mirror the C access pattern exactly (e.g., `*ptr++` vs `ptr[i]`).

## Structs & Alignment
- **Struct Alignment & Padding**: Verify field offsets against Ghidra. If offsets drift, an earlier member's alignment is wrong.
- **Unaligned Floats**: If a `float` exists at an unaligned offset (e.g., 0x0A), define it as `u8 _A[4]` to prevent the compiler from adding implicit padding.
- **Array Fields**: Always provide explicit sizes for array fields in structs.
- **Consolidation**: Group identically-typed fields at uniform strides into arrays to enable cleaner indexing patterns.

## Specialized Patterns
- **Identical Function Folding**: The compiler may fold identical functions. To verify a match, temporarily add a unique constant or global access to force distinct implementation.
- **Function Address Gaps**: Fill gaps between matched functions in `.text` with stubs to maintain correct alignment and ordering.
- **Boolean/OR Sensitivity**: In `||` conditions, the compiler's choice of `bne` vs `beq` can affect register reuse and instruction hoisting.

### Learnings from fn_3_169150
- **Function prototype requirement**: The compiler enforces strict C89 rules where functions must be declared before use. Any call to an undefined function (like `fn_800B0A5C_insertQueue` or `fn_3_1690C0`) will cause a build failure unless a prototype is provided earlier in the file or via a header.

- **Encoding sensitivity**: Files containing Shift JIS encoded text (common in Japanese source) can cause build failures if the compiler or build tools encounter encoding errors, even if the code itself is valid.

- **Build system behavior**: The build process halts on the first critical error (e.g., missing prototype) and does not proceed to compile remaining files, making it essential to resolve all prototype and syntax issues before successful compilation.

- **No generalizable architectural patterns**: The failures observed are due to standard C89 compliance and encoding issues rather than project-specific quirks or unconventional code patterns.
