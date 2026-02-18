#ifndef UNKNOWN_HEADERS_H
#define UNKNOWN_HEADERS_H

#include "types.h"

void fn_800528AC(void (*)(), s32 *);                   /* extern */
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
#endif
