#ifndef __GAME_rep_E08_H_
#define __GAME_rep_E08_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad0[0x04];
    u16 field_0x04; // 0x04
    u8 _pad1[0x02];
    u8 field_0x08;  // 0x08
    u8 _pad2[0x01];
    u8 field_0x0a;  // 0x0a
    u8 field_0x0B;  // 0x0b
    u8 _pad3[0x04];
} Struct_32220; // size: 0x10

extern Struct_32220 lbl_3_common_bss_32220;

typedef struct {
    u8 unk0; // 0x00
    u8 unk1; // 0x01
    u8 unk2; // 0x02
    u8 unk3; // 0x03
} Struct_32230; // size: 0x04

extern Struct_32230 lbl_3_common_bss_32230;

typedef struct {
    u8 unk0; // 0x00
    u8 unk1; // 0x01
    u8 _pad[0x06];
} Struct_32234; // size: 0x08

extern Struct_32234 lbl_3_common_bss_32234;

typedef struct {
    u8 _pad0[0x38];
    s16 unk38; // Offset 0x38
    s16 unk3A; // Offset 0x3A
    u8 _pad1[0x18]; // Total size 0x54
} AnimationStruct_31EAC;

extern AnimationStruct_31EAC g_UnkAnimation_31EAC[];

void fn_3_60768(void);
void fn_3_60804(void);
void fn_3_60A98(void);
void fn_3_60D80(void);
void fn_3_60E90(void);
void fn_3_61148(void);
void fn_3_61228(void);
void fn_3_61544(void);
void fn_3_61B64(void);
void fn_3_62904(void);
void fn_3_62B50(void);
void fn_3_62CA8(void);
void fn_3_62D44(void);
void fn_3_62E04(s32 index);
void fn_3_62E28(void);
void fn_3_62E70(void);
void fn_3_631AC(void);
void fn_3_63874(void);
void fn_3_63A38(void);
void fn_3_63AF8(void);
void fn_3_64BDC(void);
void fn_3_657E4(void);
void fn_3_65FE0(void);
void fn_3_66140(void);
void fn_3_664FC(void);
void fn_3_668BC(void);
void fn_3_67130(void);
void fn_3_6714C(void);

#endif // !__GAME_rep_E08_H_
