#ifndef __GAME_rep_3090_H_
#define __GAME_rep_3090_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "Dolphin/mtx.h"
#include "Dolphin/mtxext.h"


typedef struct Struct_DE94_Inner {
    /* 0x000 */ u8 _pad0[0x14];
    /* 0x014 */ s32 field_0x14;
    /* 0x018 */ u32 field_0x18;
    /* 0x01C */ u8 _pad1[0x04];
    /* 0x020 */ u32 field_0x20;
    /* 0x024 */ u8 _pad2[0xF4];
    /* 0x118 */ u32 field_0x118;   // 0x118
    /* 0x11C */ u32 field_0x11C;   // 0x11C (Replaced _pad3)
    /* 0x120 */ u8 _pad3[0x04];    // (Adjusted padding)
    /* 0x124 */ u32 field_0x124;
    /* 0x128 */ u8 _pad4[0x883];
    /* 0x9AB */ u8 field_0x9AB;
    /* 0x9AC */ u8 _pad5[0x05];
    /* 0x9B1 */ u8 field_0x9B1;    // 0x9B1 (Corrected from 0x9B6)
    /* 0x9B2 */ u8 _pad6[0x4];     // Remaining space to field_0x9B6
    /* 0x9B6 */ u8 field_0x9B6;
} Struct_DE94_Inner;

extern Struct_DE94_Inner* lbl_3_common_bss_DE94;

typedef struct Fixup_105C84 {
    u8 _pad0[4];
    u16 unk4;
    s16 unk6;
    u32 unk8;
    u32 array[1];
} Fixup_105C84;

typedef struct SourceObj_104B20 {
    u8 _pad[0x30];
    VecXYZ pos;
} SourceObj_104B20;

void fn_3_FBDAC(s32 param);
void fn_3_FC448(void);
void fn_3_FC938(void);
void fn_3_FCE38(void);
void fn_3_FCEAC(void);
void fn_3_FCEB0(void);
void fn_3_FCF20(void);
void fn_3_FCF24(void);
void fn_3_FD408(void);
void fn_3_FD4DC(void);
void fn_3_FD51C(void);
void fn_3_FD5A8(void);
void fn_3_FD670(void);
u32 fn_3_FD9FC(void);
void fn_3_FDA1C(void);
void fn_3_FDB30(void);
BOOL fn_3_100018(void);
void fn_3_100038(void);
BOOL fn_3_10007C(void);
void fn_3_1000D8(void);
void fn_3_101CC4(void);
void fn_3_103C30(void);
void fn_3_103E7C(void);
void fn_3_1040D8(void);
void fn_3_104338(void);
void fn_3_1045A8(void);
void fn_3_104740(void);
void fn_3_1048E0(void);
void fn_3_104A3C(Vec *dst, const Mtx44 mtx);
void fn_3_104A88(Vec *dst, Mtx44 mtx);
void fn_3_104AD4(Vec *dst, Mtx44 mtx);
void fn_3_104B20(VecXYZ* dst, SourceObj_104B20* src);
void fn_3_104B3C(void);
void fn_3_1054CC(void);
void fn_3_1054D0(void);
// Struct for fn_3_105BD8
typedef struct Struct_105BD8 {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ u8 _pad1[0x10];
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u8 _pad2[0x10];
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ u8 _pad3[0x10];
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ u8 _pad4[0x44];
} Struct_105BD8; // Size: 0x88 (136 bytes)

void fn_3_105A10(void);
void fn_3_105ACC(void);
void fn_3_105BD8(Struct_105BD8* arg0);
typedef struct {
    u16 key;
    u16 val;
} LookupEntry_105C28;

typedef struct {
    u16 count;
    u16 _pad;
    LookupEntry_105C28 entries[1];
} LookupTable_105C28;

typedef struct {
    u8 _pad0[0x8];
    LookupTable_105C28* table;
} Struct_105C28;

u32 fn_3_105C28(Struct_105C28* arg0, u32 arg1);
void fn_3_105C84(Fixup_105C84* arg0);
void fn_3_105CDC(void);
void fn_3_105E00(void);
void fn_3_106014(void);
void fn_3_1060D8(void);
void fn_3_10617C(void);
void fn_3_106270(void);
u32 fn_3_10698C(u32* list);
void* fn_3_1069B0(void* base, s32 index);
void fn_3_1069C0(void);
void fn_3_106BA0(void);

extern struct {
    u8 _00[0x28];
    u8 _28;
} lbl_80366158;

// No local headers needed.
/* --- The Function --- */
extern Vec lbl_3_data_20FF8;
extern Vec lbl_3_data_20FEC;
extern f32 lbl_3_rodata_30FC;
#endif // !__GAME_rep_3090_H_
