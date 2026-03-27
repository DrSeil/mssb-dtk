# MWCC 2.6 Decompilation Key Learnings

## Core Mandates & Syntax
- **Mandatory C Syntax**: NEVER treat arrays as lvalues (e.g., `array += 1` is ILLEGAL). Use an explicit local pointer (`u8 *p = array; p++;`) or array indexing.
- **Stop-and-Think on Syntax Errors**: If the compiler reports `not an lvalue`, `illegal operand`, or `syntax error`, it is a fundamental C violation. Stop and reconsider the structure; do not attempt to "tweak" a broken pattern.
- **Mandatory Prototypes**: MWCC 2.6 requires prototypes for all functions (due to `-requireprotos`). Always update headers before building.
- **Strict C89 Compliance**: Declare all variables at the very top of the function scope. Implicit conversions between pointer types are often disallowed.
- **Encoding Sensitivity**: Files containing Shift JIS text can cause build failures. If the compiler chokes on a file with no obvious syntax errors, check for non-ASCII source bytes.

## Expression & Instruction Scheduling
- **No Temp Variables by Default**: Write clean direct expressions first (e.g., `dest->x = a->y * b->z - a->z * b->y`). Only introduce temp variables if the diff reveals a specific register allocation problem they would fix — adding temps when they're not needed causes extra registers and wrong scheduling.
- **Instruction Interleaving**: MWCC 2.6 moves loads and address calculations early to hide pipeline stalls. If logic is correct but instructions are out of order, try reordering C statements or using a targeted temporary to guide the scheduler.
- **Register/ASM Tags Ignored**: The `register` keyword and `asm("rN")` tags are frequently overridden by the optimizer. Focus on code structure instead.

## Loop Structure
- **Do-While for Running Pointers**: Prefer `do-while` over `for` when the assembly shows a running pointer incrementing (`stb (r4); addi r4, r4, 1`). `for` loops often trigger unrolling or induction variables that don't match simple patterns.
- **Register Steering via Declaration Order**: Register allocation is driven by the order the compiler encounters symbols. Interleave declarations and assignments (`decl v1; init v2; assign v1;`) to steer which global gets which register.

## Pointers & Array Access
- **Index-based Access**: Using `global.array[i].field` often lets the compiler auto-generate a running pointer with optimal prologue scheduling. An explicit `Type *p = &global; p->field` may produce an extra `mr` instruction.
- **Mirror the Assembly Pattern**: If the assembly shows `*ptr++` vs `ptr[i]`, match it exactly. If it shows a register reload after an increment, your access pattern differs from the original.

## Structs & Alignment
- **Verify Field Offsets Against Ghidra**: If offsets drift, an earlier member's alignment is wrong.
- **Unaligned Floats**: If a `float` exists at an unaligned offset (e.g., `0x0A`), define it as `u8 _A[4]` to prevent implicit padding.
- **Consolidate Repeated Fields**: Group identically-typed fields at uniform strides into arrays to enable cleaner indexing patterns.
- **Explicit Array Sizes**: Always provide explicit sizes for array fields in structs.

## Specialized Patterns
- **Identical Function Folding**: The compiler may fold identical functions. To verify a match, temporarily add a unique constant or global access to force a distinct implementation.
- **Function Address Gaps**: Fill gaps between matched functions in `.text` with stubs to maintain correct alignment and ordering.
- **Boolean/OR Sensitivity**: In `||` conditions, the compiler's choice of `bne` vs `beq` can affect register reuse and instruction hoisting.

### Learnings from fn_3_EB684
- **Padding Field Access:** When accessing specific bytes within a struct's padding array (e.g., `_pad[1]`), the compiler expects explicit indexing rather than pointer arithmetic or casting, as these fields are often treated as distinct, addressable members in the symbol map.
- **Global State Pattern:** The codebase frequently uses a centralized status structure where multiple disparate conditions (e.g., status flags and game state enums) are checked in sequence to determine a boolean "ready" or "active" state.
- **Enum/Constant Comparison:** The compiler strictly enforces type consistency for global state enums; comparisons against magic numbers (like `0xB`) are common, but they must align with the underlying integer type defined in the global header to avoid implicit conversion warnings that break strict builds.
