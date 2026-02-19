#ifndef UNKNOWN_HEADERS_H
#define UNKNOWN_HEADERS_H

#include "types.h"
#include "game/UnknownHomes_Game.h"

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
typedef struct {
    u8 pad[0x14];
    f32 unk14;
} UnknownStructB7;
extern UnknownStructB7 lbl_3_data_4444;

typedef struct CommonBss_35154 {
    /* 0x000 */ u32 firstMember; // Previously inside _pad0
    /* 0x004 */ u8 _pad0[0x3AC - 4];
    /* 0x3AC */ u32 bitfield;
    /* 0x3B0 */ u8 _pad_extra[0x54];
    /* 0x404 */ s16 someHalfword;
    /* 0x406 */ u8 _pad2[0x2E];
    /* 0x434 */ f32 unk_434;
    /* 0x438 */ f32 unk_438;
    /* 0x43C */ f32 unk_43C;
    /* 0x440 */ u8 _pad3[0x26];
    /* 0x466 */ u8 someFlag;
    /* 0x467 */ u8 unk_467;
} CommonBss_35154;

extern CommonBss_35154 lbl_3_common_bss_35154;
extern void fn_80034CEC(u32);
// The assembly passes the index in r4
extern VecXZ lbl_3_data_111C8[];
// Extern function prototype based on bl fn_8003A688
extern void fn_8003A688(f32, f32);
extern u8 lbl_3_bss_B894[0xF];
typedef struct CommonBss32A94 {
u8 _pad[0x4];
s16 unk4;
} CommonBss32A94;
extern CommonBss32A94 lbl_3_common_bss_32A94;
extern void fn_3_FBE24(void);
#endif
