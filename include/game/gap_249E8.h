#ifndef __GAME_gap_249E8_H_
#define __GAME_gap_249E8_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad0[0x34];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} Data2398Entry; // size 0x40

extern Data2398Entry lbl_3_data_2398[];

void fn_3_249E8(s32 arg0);
void fn_3_24ADC(s32 arg0, s32 arg1);
void fn_3_24DBC(s32 arg0);
void fn_3_24EA0(void);
void fn_3_24F20(void);
void fn_3_24F24(void);
void fn_3_250FC(void);

#endif // !__GAME_gap_249E8_H_
