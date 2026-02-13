#ifndef __GAME_rep_F80_H_
#define __GAME_rep_F80_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0xD4];
    u8 someFlag; // 0xD4
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

#endif // !__GAME_rep_F80_H_
