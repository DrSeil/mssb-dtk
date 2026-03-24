#ifndef __GAME_gap_AC9F8_H_
#define __GAME_gap_AC9F8_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "game/gap_B3B70.h"

typedef struct {
    u8 _pad0[0x04];      /* 0x00 */
    s16 field_0x04;      /* 0x04 */
    s16 field_0x06;      /* 0x06 */
    s16 field_0x08;      /* 0x08 */
    u8 _pad1[0x02];      /* 0x0A */
    s16 field_0x0C;      /* 0x0C */
    s16 field_0x0E;      /* 0x0E */
    s16 field_0x10;      /* 0x10 */
    u8 _pad2[0x1BF];     /* 0x12 to 0x1D0 */
    u8 field_0x1D1;      /* 0x1D1 */
    u8 field_0x1D2;      /* 0x1D2 */
    u8 _pad3[0x2];       /* 0x1D3-0x1D4 */
    u8 field_0x1D5;      /* 0x1D5 */
    u8 field_0x1D6;      /* 0x1D6 */
    u8 field_0x1D7;      /* 0x1D7 */
    u8 _pad4[0x2];       /* 0x1D8-0x1D9 */
    u8 field_0x1DA;      /* 0x1DA */
    u8 _pad5[0x2B];      /* 0x1DB-0x205 */
    s8 field_0x206;      /* 0x206 */
} Bss34C90Item; // size: 0x264

extern Bss34C90Item lbl_3_common_bss_34C90;

void fn_3_AC9F8(void);
void fn_3_ACAF8(void);
void fn_3_ACD38(void);
void fn_3_AD164(void);
void fn_3_AD2A0(void);
void fn_3_AD3BC(void);
void fn_3_AD8CC(void);
void fn_3_ADA3C(void);
void fn_3_ADEDC(void);
void fn_3_AE334(void);
void fn_3_AE770(void);
void fn_3_AE900(void);
void fn_3_AEC50(void);
void fn_3_AEFF8(void);
void fn_3_AF10C(void);
void fn_3_AF428(void);
void fn_3_AF5A4(void);
void fn_3_AFA64(void);
void fn_3_AFB64(void);
s32 fn_3_AFD48(s16 arg0);
void fn_3_AFD80(u8 arg0);
void fn_3_AFDA4(void);
void fn_3_AFDC0(void);
void fn_3_AFE0C(void);
void fn_3_B003C(void);
void fn_3_B01E0(void);
void fn_3_B025C(void);
void fn_3_B02A8(void);
void fn_3_B03F0(void);
void fn_3_B0464(void);
void fn_3_B056C(void);
void fn_3_B0874(void);
void fn_3_B0A88(void);
void fn_3_B0AAC(void);
void fn_3_B0B5C(void);
void fn_3_B0CF4(void);
void fn_3_B0D2C(void);
void fn_3_B0D78(void);
void fn_3_B0D7C(void);
void fn_3_B0DB0(void);
void fn_3_B0E00(void);
void fn_3_B1120(void);
void fn_3_B116C(void);
void fn_3_B11D0(void);
void fn_3_B12F8(void);
void fn_3_B1470(void);
void fn_3_B1578(void);
void fn_3_B1A30(void);
void fn_3_B1BCC(void);
void fn_3_B1C14(void);
void fn_3_B1CB0(void);
void fn_3_B1DA4(u8 arg0, u8 arg1);
void fn_3_B1DD0(void);
void fn_3_B254C(void);
void fn_3_B2630(void);
void fn_3_B274C(void);
void fn_3_B27A4(void);
void fn_3_B28A8(void);
void fn_3_B2AA0(void);
void fn_3_B2E20(void);
void fn_3_B3288(void);
void fn_3_B32B8(void);
void fn_3_B3448(void);
void fn_3_B3620(void);
void fn_3_B3A28(void);

#endif // !__GAME_gap_AC9F8_H_
