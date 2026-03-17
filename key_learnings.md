
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
30");
    register void* r29 asm("r29");
    register void* r0 asm("r0");
    register void* r1 asm("r1");
    
    r30 = (void*)arg1;
    r5 = (void*)(arg0 * 0x268);
    r4 = (void*)g_Fielders;
    r0 = (void*)((u8*)r4 + (u32)r5);
    r31 = r0;
    if ((s32)r30 < 1) {
        return;
    }
    fn_3_2BB04();
    if ((s32)r3 != 0) {
        return;
    }
    r3 = (void*)g_Ball;
    r4 = (void*)((u8*)r3 + ((u32)r30 << 4));
    r0 = (void*)0x1;
    r3 = (void*)((u8*)r4 + 0x354);
    r4 = (void*)((u8*)r4 + 0x35C);
    r31->unk14 = *(f32*)r3;
    r31->unk1C = *(f32*)r4;
    r31->unk216 = (u8)r0;
}
Struct Updates: [{"type_name": "Fielder", "actions": [{"action": "add_field", "type": "f32", "name": "unk14", "offset": "0x14"}, {"action": "add_field", "type": "f32", "name": "unk1C", "offset": "0x1C"}, {"action": "add_field", "type": "u8", "name": "unk216", "offset": "0x216"}]}]

--- Attempt 4 ---
Score: 0.0%
Build Error:
[1/31] MWCC build/GYQE01/src/game/rep_720.o
[2/31] MWCC build/GYQE01/src/game/rep_1038.o
[3/31] MWCC build/GYQE01/src/game/rep_1200.o
[4/31] MWCC build/GYQE01/src/game/rep_13B8.o
[5/31] MWCC build/GYQE01/src/game/rep_16B8.o
[6/31] MWCC build/GYQE01/src/game/rep_18E8.o
[7/31] MWCC build/GYQE01/src/game/rep_1CB8.o
[8/31] MWCC build/GYQE01/src/game/rep_1E08.o
[9/31] MWCC build/GYQE01/src/game/rep_1F58.o
[10/31] MWCC build/GYQE01/src/game/rep_23E8.o
[11/31] MWCC build/GYQE01/src/game/rep_2528.o
[12/31] MWCC build/GYQE01/src/game/rep_2A28.o
[13/31] MWCC build/GYQE01/src/game/rep_2B48.o
[14/31] MWCC build/GYQE01/src/game/rep_2C88.o
[15/31] MWCC build/GYQE01/src/game/rep_2D88.o
[16/31] MWCC build/GYQE01/src/game/rep_2F08.o
[17/31] MWCC build/GYQE01/src/game/rep_3008.o
[18/31] MWCC build/GYQE01/src/game/rep_3108.o
[19/31] MWCC build/GYQE01/src/game/rep_3208.o
[20/31] MWCC build/GYQE01/src/game/rep_3308.o
[21/31] MWCC build/GYQE01/src/game/rep_3408.o
[22/31] MWCC build/GYQE01/src/game/rep_3508.o
[23/31] MWCC build/GYQE01/src/game/rep_3608.o
[24/31] MWCC build/GYQE01/src/game/rep_3708.o
[25/31] MWCC build/GYQE01/src/game/rep_3808.o
[26/31] MWCC build/GYQE01/src/game/rep_3908.o
[27/31] MWCC build/GYQE01/src/game/rep_3A08.o
[28/31] MWCC build/GYQE01/src/game/rep_3B08.o
[29/31] MWCC build/GYQE01/src/game/rep_3C08.o
[30/31] MWCC build/GYQE01/src/game/rep_3D08.o
[31/31] MWCC build/GYQE01/src/game/rep_3E08.o
[1/31] MWCC build/GYQE01/src/game/rep_EA0.o
[2/31] MWCC build/GYQE01/src/game/rep_A00.o
[3/31] MWCC build/GYQE01/src/game/rep_E08.o
[4/31] MWCC build/GYQE01/src/game/rep_F80.o
[5/31] MWCC build/GYQE01/src/game/rep_AC8.o
FAILED: build/GYQE01/src/game/rep_AC8.o 
build/tools/wibo build/tools/sjiswrap.exe build/compilers/GC/2.6/mwcceppc.exe -proc gekko -nodefaults -Cpp_exceptions off -RTTI off -fp hard -fp_contract on -O4,p -maxerrors 1 -enum int -inline auto -str reuse,readonly -nosyspath -use_lmw_stmw on -sdata 8 -sdata2 8 -DVERNUM={version_num} -i include -i include/stl -DNDEBUG=1 -sdata 0 -sdata2 0 -O4,p -sym on -inline noauto -pool off -requireprotos -lang=c -MMD -c src/game/rep_AC8.c -o build/GYQE01/src/game && "/usr/bin/python3" tools/transform_dep.py build/GYQE01/src/game/rep_AC8.d build/GYQE01/src/game/rep_AC8.d
### mwcceppc.exe Compiler:
#    File: src\game\rep_AC8.c
# ---------------------------
#     118: register void* r5 asm("r5");
#   Error:                       ^
#   cannot declare global register variables after code has been generated.
#   Too many errors printed, aborting program

User break, cancelled...
[6/31] MWCC build/GYQE01/src/game/m_sound.o
ninja: build stopped: subcommand failed.

Code Snippet:
void fn_3_2ACD8(s32 arg0, s32 arg1) {
    register void* r5 asm("r5");
    register void* r3 asm("r3");
    register void* r4 asm("r4");
    register void* r31 asm("r31");
    register void* r30 asm("r30");
    register void* r29 asm("r29");
    register void* r0 asm("r0");
    register void* r1 asm("r1");
    
    r30 = (void*)arg1;
    r5 = (void*)(arg0 * 0x268);
    r4 = (void*)g_Fielders;
    r0 = (void*)((u8*)r4 + (u32)r5);
    r31 = r0;
    if ((s32)r30 < 1) {
        return;
    }
    fn_3_2BB04();
    if ((s32)r3 != 0) {
        return;
    }
    r3 = (void*)g_Ball;
    r4 = (void*)((u8*)r3 + ((u32)r30 << 4));
    r0 = (void*)0x1;
    r3 = (void*)((u8*)r4 + 0x354);
    r4 = (void*)((u8*)r4 + 0x35C);
    r31->unk14 = *(f32*)r3;
    r31->unk1C = *(f32*)r4;
    r31->unk216 = (u8)r0;
}
Struct

### Learnings from fn_3_104A88
- The compiler enforces strict C89 typing rules, particularly around implicit conversions between pointer types. Passing a `const float (*)[4][4]` where a `const float (*)[4]` is expected will cause a build failure, requiring explicit casting or restructuring of data layouts.

- External symbol declarations in shared headers (e.g., `extern const Vec3f lbl_3_data_20FF8;`) must be properly terminated with a semicolon. Missing semicolons in header files will cause cascading compilation failures across multiple translation units.

- The build system appears to use a shared header (`UnknownHeaders.h`) that is included by many source files. Errors in this header (such as syntax issues) will block compilation of all dependent files, indicating the importance of header stability.

- The codebase likely uses a custom memory layout or data section (e.g., `lbl_3_data_20FF8`) that must be defined elsewhere in the project. References to undefined external data symbols will fail unless the corresponding data is properly declared and linked.

- The compiler's error handling is strict and will abort on the first fatal error, preventing partial builds. This suggests that fixing one error may not immediately resolve build issues if other dependent errors exist.
