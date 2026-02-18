#ifndef UNKNOWN_HEADERS_H
#define UNKNOWN_HEADERS_H

#include "types.h"

typedef void (*QueueCallback)(void);
void fn_800528AC(void (*callback)(void), s32 *arg1);
typedef struct Struct_3_2281C_Entry {
u8 _pad0[0x68];
s16 field_0x68;
} Struct_3_2281C_Entry;
typedef struct GlobalStruct_8036E548 {
u8 _pad0[0x70];
s32 unk70;              // Offset 0x70
u8 _pad1[0x2C50 - 0x74];
Struct_3_2281C_Entry* entries[1]; // Offset 0x2C50
u8 _pad2[0x3070 - (0x2C50 + 4)];
void (*unk3070)(void);  // Offset 0x3070
void (*unk3074)(s32);   // Offset 0x3074
} GlobalStruct_8036E548;
extern GlobalStruct_8036E548 lbl_8036E548;
void fn_800BD670(s32 fieldVal, s32 arg1, GlobalStruct_8036E548* globalPtr);
extern GlobalStruct_8036E548 lbl_8036E548;
extern f32 lbl_3_rodata_1414;

typedef struct UnknownStruct_350E4 {
    u8 _pad0[0x3C];
    u32* unk3C;         /* 0x3C - Pointer to u32 array */
    u16* unk40;         /* 0x40 - Pointer to u16 array */
    u32* unk44;         /* 0x44 - Pointer to u32 array base */
    u8 _pad1[0x6C-0x48];
    u8 unk6C;           /* 0x6C */
    u8 _pad2[0xC0-0x6D];
    u8 targetByte;      /* 0xC0 */
} UnknownStruct_350E4;

extern UnknownStruct_350E4 lbl_3_common_bss_350E4[];

typedef struct Bss_1910 {
f32 unk0;
f32 unk4;
f32 unk8;
f32 unkC;
f32 unk10;
f32 unk14;
} Bss_1910;
extern Bss_1910 lbl_3_bss_1910;
extern f32 lbl_3_rodata_1DD0;
extern f32 lbl_3_rodata_1DD4;
typedef struct QueueEntry {
u8 _pad0[0x10];
s16 unk10;
} QueueEntry;
/* extern function with proper types */
QueueEntry* fn_800B0A5C_insertQueue(QueueCallback callback, s32 priority);
typedef struct UnknownStruct_EF3D4 {
u8 _pad0[0x74];
s32 unk74;
} UnknownStruct_EF3D4;
/* The global is accessed as an array of 32-bit values (pointers or ints) */
extern s32 lbl_3_bss_B154[];
#endif
