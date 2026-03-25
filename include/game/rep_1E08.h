#ifndef __GAME_rep_1E08_H_
#define __GAME_rep_1E08_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"
#include "game/rep_1C0.h"

extern u8 lbl_3_bss_995C;
extern s16 lbl_3_bss_9952;
extern s32 lbl_3_data_17000[];
extern CompressedDiskRead lbl_3_data_11380;
extern f32 lbl_3_rodata_1E64;
extern f32 lbl_3_rodata_1E84;
extern f32 lbl_3_rodata_1E68;
extern f32 lbl_3_rodata_1EC0;
extern f32 lbl_3_rodata_1EC4;
extern f64 lbl_3_rodata_1ED0;

typedef struct Data170D8 {
    u8 _pad[4];
    s32 unk4;
} Data170D8;
extern Data170D8 lbl_3_data_170D8;
extern u8 lbl_80366158; // Size and exact type unknown, accessing as byte array
extern f32 lbl_3_rodata_1EB0;
extern f32 lbl_3_rodata_1F18;
extern f32 lbl_3_rodata_1F1C;
extern f32 lbl_3_rodata_1F20;
extern f32 lbl_3_rodata_1F24;
extern f32 lbl_3_rodata_1F38;
extern f64 lbl_3_rodata_1F40;

extern u8 lbl_3_data_A3C[];

typedef struct {
    f32 x;
    f32 z;
} CosSinPair;

typedef struct {
    u8 _pad0[0x4C];
    CosSinPair a[0x60];   /* 0x4C */
    u8 _pad1[0x4C];       /* 0x34C */
    CosSinPair b[0x60];   /* 0x398 */
    u8 _rest[0x698];      /* 0x698 */
} Data11620;
extern Data11620 lbl_3_data_11620;


typedef struct DataOuter {
    DataArrayElement elements[10]; // 10 * 0x10 = 0xa0
} DataOuter;

extern DataOuter lbl_3_data_12354[];
extern s32 lbl_3_data_1146C[];

typedef struct Rep1E08Obj {
    u8 _pad[0x252];
    u8 field_0x252; // 0x252
} Rep1E08Obj;

void pitchingMachinePitching(s32 param);

void fn_8006C3F0(s32 arg);
void fn_8006C43C(s32 arg);

typedef struct {
    u8 arr0[8];     /* 0x00 - indexed by StadiumID */
    u8 arr8[24];    /* 0x08 - flat, indexed by StadiumID*3 */
} Data111A8;
extern Data111A8 lbl_3_data_111A8;

typedef struct {
    u32 prefix; /* 4-byte prefix before texture data */
} TexDataInfo;

typedef struct {
    u8 _pad[0x18];
    TexDataInfo* texData; /* 0x18 */
} TexBundleHeader;

extern void fn_8003A85C(u8 val);
extern void fn_8003A848(u8 b0, u8 b1, u8 b2);
extern void fn_8003A6B0(s32 idx, TexDataInfo* texData, f32 f1, f32 f2);

typedef struct {
    u8 _pad0[0x08];
    Mtx mtx;         /* 0x08 */
    f32 unk38;       /* 0x38 */
    f32 unk3C;       /* 0x3C */
    f32 unk40;       /* 0x40 */
    s32 unk44;       /* 0x44 */
    f32 unk48;       /* 0x48 */
    u8 _rest[0x300]; /* 0x4C */
} BdcArrayElement; /* size 0x34C */

typedef struct {
    u8 _pad0[0x40];
    Mtx mtx;         /* 0x40 */
    u8 _pad1[0x34];
    f32 unkA4;       /* 0xa4 */
} Fn52768Result;

Fn52768Result* fn_80052768(s32 arg0);
void fn_800A7D4C(s32 arg0, BdcArrayElement* elem);

void fn_3_BA538(void);
void fn_3_BA7F4(void);
void fn_3_BB07C(BssTransRotData* arg0, f32 f1);
void fn_3_BB15C(void);
void fn_3_BB454(void);
void fn_3_BB7F4(void);
void fn_3_BBBC4(void);
void fn_3_BBF94(void);
void fn_3_BC224(void);
void fn_3_BC25C(void);
s32 fn_3_BC274(Rep1E08Obj* arg0, VecXZ* arg1, VecXZ* arg2);
void fn_3_BC2DC(void);
void fn_3_BC6D8(void);
void fn_3_BC850(s32 dummy, s32 index);
void fn_3_BC888(void);
void fn_3_BCA20(void);
void fn_3_BD1D4(void);
void fn_3_BD1D8(void);
void fn_3_BD434(s32 arg0, s32 arg1);
void fn_3_BD4F0(void);
void fn_3_BD504(s32 flag, f32 f1, f32 f2, f32 f3);
void fn_3_BD6AC(s32 arg0, f32 f1, f32 f2, f32 f3);
void fn_3_BD758(void);
BOOL fn_3_BD7D0(void);
void fn_3_BD7D8(void);
void fn_3_BD7DC(s32 arg1);
void fn_3_BD80C(s32 arg0);
void fn_3_BD8D8(void);
void fn_3_BD8FC(void);
void fn_3_BDCA4(void);
void fn_3_BDE14(void);
void fn_3_BDF74(void);
void fn_3_BE140(void);
void fn_3_BE174(s32 arg, f32 v1, f32 v2, f32 v3);
void fn_3_BE1D4(void);
void fn_3_BEFF8(void);
void fn_3_BF070(void);
void fn_3_BF158(void);
void fn_3_BF1AC(void);
void fn_3_BF20C(void);
void fn_3_BF238(void);
void fn_3_BF6C0(void);
s32 fn_3_BF878(void);
void fn_3_BF8F8(void);
void fn_3_BFB3C(void);
void fn_3_BFDA4(void);
void fn_3_C0134(void);
void fn_3_C0770(void);
void fn_3_C07A0(void);
void fn_3_C07B0(void);

// No local headers needed.
#endif // !__GAME_rep_1E08_H_
