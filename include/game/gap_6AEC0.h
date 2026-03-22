#ifndef __GAME_gap_6AEC0_H_
#define __GAME_gap_6AEC0_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0xC8];
    u8 field_0xC8;
    u8 field_0xC9;
    u8 field_0xCA;
    u8 _pad2[0x9];
    u8 field_0xD4;
} Bss32724Item;

extern Bss32724Item lbl_3_common_bss_32724;

void fn_3_6AEC0(void);
void fn_3_6AEE0(void);

#endif // !__GAME_gap_6AEC0_H_
