#ifndef __GAME_gap_219CC_H_
#define __GAME_gap_219CC_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0x261];
    u8 field_0x261;
} Bss1323CItem;

extern Bss1323CItem* lbl_3_common_bss_1323C;

void fn_3_219CC(void);
void fn_3_21AA8(void);
void fn_3_21BDC(void);
void fn_3_21C7C(s32 arg0, u8 arg1);

#endif // !__GAME_gap_219CC_H_
