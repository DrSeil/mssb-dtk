#ifndef __GAME_gap_6D4A0_H_
#define __GAME_gap_6D4A0_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_Static.h"

typedef struct {
    u8 _pad0[0x3B]; // 0x00
    u8 stats[0x65]; // 0x3B
} Struct_8034E9A0;  // 0xA0 bytes

typedef struct {
    u8 _pad0[0x28]; // 0x00
    f32 unk_28;     // 0x28
    u8 _pad1[0x4];  // 0x2C
} Struct_5FC4;      // 0x30 bytes

extern Struct_8034E9A0 lbl_8034E9A0[];
extern Struct_5FC4 lbl_3_data_5FC4;

void fn_3_6D4A0(void);
void fn_3_6D564(void);
s32 fn_3_6D658(s32 arg0, s32 arg1, s32 arg2);

#endif // !__GAME_gap_6D4A0_H_
