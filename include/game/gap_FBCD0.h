#ifndef __GAME_gap_FBCD0_H_
#define __GAME_gap_FBCD0_H_

#include "mssbTypes.h"

typedef struct {
    u32 unk_00;            /* 0x000 */
    u8 _pad04[0x04];       /* 0x004 */
    u32 unk_08;            /* 0x008 */
    u8 _pad0C[0x08];       /* 0x00C */
    u32 unk_14;            /* 0x014 */
    u32 field_0x18;        /* 0x018 */
    u8 _pad1C[0xFC];       /* 0x01C */
    u32 unk_118;           /* 0x118 */
    u8 _pad11C[0x810];     /* 0x11C */
    u32 unk_92C;           /* 0x92C */
    u8 _pad930[0x14];      /* 0x930 */
    u16 unk_944;           /* 0x944 */
    u8 _pad946[0x65];      /* 0x946 */
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
void fn_3_FBDAC(u32 arg0);
void fn_3_FBE24(void);
void fn_3_FC2FC(void);

#endif // !__GAME_gap_FBCD0_H_
