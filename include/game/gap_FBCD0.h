#ifndef __GAME_gap_FBCD0_H_
#define __GAME_gap_FBCD0_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0x9AC];
    u8 field_0x9AC;
} BssDE94Item;

extern BssDE94Item* lbl_3_common_bss_DE94;

void fn_3_FBCD0(void);
void fn_3_FBD58(void);
void fn_3_FBD70(void);
void fn_3_FBDAC(void);
void fn_3_FBE24(void);
void fn_3_FC2FC(void);

#endif // !__GAME_gap_FBCD0_H_
