#ifndef __GAME_rep_F80_H_
#define __GAME_rep_F80_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad0[0x90];
    s16 unk_90; // Offset 0x90
    s16 unk_92; // Offset 0x92
    u8 _pad1[0x15];
    u8 unk_A9;  // Offset 0xA9
    u8 _pad2[0xB];
    u8 unk_B5;  // Offset 0xB5
    u8 _pad3[0x1D];
    u8 unk_D3;  // Offset 0xD3
    u8 someFlag; // Offset 0xD4
} CommonBss_32724;

extern CommonBss_32724 lbl_3_common_bss_32724;

void fn_3_6A2A4(void);
void fn_3_6A300(void);
void fn_3_6A400(void);
void fn_3_6A414(void);
void fn_3_6A83C(void);
void fn_3_6A9B0(void);
void fn_3_6AA98(void);
void fn_3_6AB30(void);
void fn_3_6AB58(void);
void fn_3_CABB4(void);
void fn_80011578(void);


#endif // !__GAME_rep_F80_H_
