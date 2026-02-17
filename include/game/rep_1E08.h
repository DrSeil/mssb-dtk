#ifndef __GAME_rep_1E08_H_
#define __GAME_rep_1E08_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"

extern u8 lbl_3_bss_995C;
extern s16 lbl_3_bss_9952;

void pitchingMachinePitching(s32 param);

void fn_8006C3F0(s32 arg);
void fn_8006C43C(s32 arg);

typedef struct {
    u8 _pad0[0x3AC];
    u32 bitfield; // Offset 0x3AC
    u8 _pad_extra[0x54];
    s16 someHalfword; // 0x404
    u8 _pad2[0x2E];
    f32 unk_434;      // 0x434
    f32 unk_438;      // 0x438
    f32 unk_43C;      // 0x43C
    u8 _pad3[0x26];
    u8 someFlag; // 0x466
    u8 unk_467; // 0x467
} CommonBss_35154;

extern CommonBss_35154 lbl_3_common_bss_35154;
 

 
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
void fn_3_BC274(void);
void fn_3_BC2DC(void);
void fn_3_BC6D8(void);
void fn_3_BC850(void);
void fn_3_BC888(void);
void fn_3_BCA20(void);
void fn_3_BD1D4(void);
void fn_3_BD1D8(void);
void fn_3_BD434(void);
void fn_3_BD4F0(void);
void fn_3_BD504(void);
void fn_3_BD6AC(void);
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
void fn_3_BE174(void);
void fn_3_BE1D4(void);
void fn_3_BEFF8(void);
void fn_3_BF070(void);
void fn_3_BF158(void);
void fn_3_BF1AC(void);
void fn_3_BF20C(void);
void fn_3_BF238(void);
void fn_3_BF6C0(void);
void fn_3_BF878(void);
void fn_3_BF8F8(void);
void fn_3_BFB3C(void);
void fn_3_BFDA4(void);
void fn_3_C0134(void);
void fn_3_C0770(void);
void fn_3_C07A0(void);

#endif // !__GAME_rep_1E08_H_
