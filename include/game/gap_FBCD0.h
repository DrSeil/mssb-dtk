#ifndef __GAME_gap_FBCD0_H_
#define __GAME_gap_FBCD0_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad0[0x18];        /* 0x00 */
    u32 field_0x18;        /* 0x18 */
    u8 _pad1[0x98F];       /* 0x1C */
    u8 field_0x9AB;        /* 0x9AB */
    u8 field_0x9AC;        /* 0x9AC */
    u8 _pad2;              /* 0x9AD */
    u8 field_0x9AE;        /* 0x9AE */
    u8 field_0x9AF;        /* 0x9AF */
    u8 field_0x9B0;        /* 0x9B0 */
    u8 _pad3[5];           /* 0x9B1 */
    u8 field_0x9B6;        /* 0x9B6 */
} BssDE94Item;

extern BssDE94Item* lbl_3_common_bss_DE94;

void fn_3_FBCD0(void);
void fn_3_FBD58(void);
void fn_3_FBD70(void);
void fn_3_FBDAC(void);
void fn_3_FBE24(void);
void fn_3_FC2FC(void);

#endif // !__GAME_gap_FBCD0_H_
