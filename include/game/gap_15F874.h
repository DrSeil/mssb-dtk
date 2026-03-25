#ifndef __GAME_gap_15F874_H_
#define __GAME_gap_15F874_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad0[0x4];
    s32 unk4;           // 0x4
    u8 _pad[0x468];     // 0x8
    u8 field_0x470;
    u8 field_0x471;
    u8 _pad2[0x7];
    u8 field_0x479;
} Bss35154Item;

extern Bss35154Item lbl_3_common_bss_35154;

void fn_3_15F874(void);
void fn_3_15F998(void);
void fn_3_15F9AC(void);
void fn_3_15F9C0(void);
void fn_3_15FA58(void);

#endif // !__GAME_gap_15F874_H_
