#ifndef __GAME_kinoko_H_
#define __GAME_kinoko_H_

#include "mssbTypes.h"

typedef struct KinokoState {
    u8 *ptr0;        // 0x00
    u8 *ptr4;        // 0x04
    u8 _unk08[0xD8]; // 0x08
} KinokoState;       // 0xE0 bytes

void fn_3_16917C(void);
void fn_3_1695A4(s8 arg1, u8 arg2);
void fn_3_169600(void);
void fn_3_169804(void);
void fn_3_169984(void);
void fn_3_169D00(void);
void fn_3_169E70(void);
void fn_3_16A07C(void);
void fn_3_16B488(void);
void fn_3_16B5B4(void);
void fn_3_16B884(void);

#endif // !__GAME_kinoko_H_
