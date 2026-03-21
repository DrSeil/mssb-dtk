#ifndef __GAME_rep_1E08_H_
#define __GAME_rep_1E08_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"
#include "game/rep_1C0.h"

extern u8 lbl_3_bss_995C;
extern s16 lbl_3_bss_9952;
extern s32 lbl_3_data_17000[];
extern CompressedDiskRead lbl_3_data_11380;

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


 

 
void fn_3_BA538(void);
void fn_3_BA7F4(void);
void fn_3_BB07C(void);
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
void fn_3_BD504(void);
void fn_3_BD6AC(s32 arg0, f32 f1, f32 f2, f32 f3);
void fn_3_BD758(void);
BOOL fn_3_BD7D0(void);
void fn_3_BD7D8(void);
void fn_3_BD7DC(s32 arg1);
void fn_3_BD80C(void);
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
