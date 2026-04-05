#ifndef __GAME_kinoko_H_
#define __GAME_kinoko_H_

#include "mssbTypes.h"

typedef struct KinokoState {
    u8 *ptr0;        // 0x00
    u8 *ptr4;        // 0x04
    u8 _unk08[0xD8]; // 0x08
} KinokoState;       // 0xE0 bytes

typedef struct KinokoData {
    u8 _pad0[0x19D4];  // 0x0000
    u32 unk_19D4;      // 0x19D4
    u8 _pad1[0x4];     // 0x19D8
    s8 unk_19DC;       // 0x19DC
    u8 unk_19DD;       // 0x19DD
    u8 _pad2[0x2];     // 0x19DE
} KinokoData;          // 0x19E0 bytes

typedef struct KinokoBss {
    u8 _pad[0x1BF0];
} KinokoBss;            // 0x1BF0 bytes

extern KinokoData lbl_3_data_28928;
extern KinokoBss lbl_3_bss_BAE0;

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
void fn_3_16C394(s8 arg0);

#endif // !__GAME_kinoko_H_
