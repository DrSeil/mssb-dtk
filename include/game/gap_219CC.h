#ifndef __GAME_gap_219CC_H_
#define __GAME_gap_219CC_H_

#include "mssbTypes.h"

typedef struct {
    u8 _data[0x2C];
} Bss1323CSubItem; // size: 0x2C

typedef struct {
    Bss1323CSubItem unk0[13]; // 0x000 - 13 x 0x2C = 0x23C bytes
    s16 unk23C;        // 0x23C
    u8 _pad23E[0x2];
    s16 unk240[13];    // 0x240
    s16 unk25A;        // 0x25A
    u8 unk25C;         // 0x25C
    u8 unk25D;         // 0x25D
    u8 _pad25E;
    u8 unk25F;         // 0x25F
    u8 _pad260;
    u8 unk261[13];     // 0x261
    u8 _pad26E[0xE];
    u8 unk27C;         // 0x27C
    u8 unk27D;         // 0x27D
    u8 unk27E;         // 0x27E
    u8 _pad27F;
    u8 unk280;         // 0x280
} Bss1323CItem;

extern Bss1323CItem* lbl_3_common_bss_1323C;

void fn_3_219CC(void);
void fn_3_21AA8(void);
void fn_3_21BDC(void);
void fn_3_21C7C(s32 arg0, u8 arg1);

#endif // !__GAME_gap_219CC_H_
