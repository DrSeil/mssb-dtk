#ifndef __GAME_rep_EA0_H_
#define __GAME_rep_EA0_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"

typedef struct {
    u8 _pad0[0xC];          // 0x00-0x0B
    s32 unkC;               // 0x0C
    u8 _padC[0x14A4 - 0x10]; // 0x10-0x14A3
    f32 unk14A4;            // 0x14A4
    f32 unk14A8;            // 0x14A8
} Bss200Struct;
extern Bss200Struct lbl_3_bss_200;

extern f32 lbl_3_rodata_F20;

void fn_3_6750C(void);
void fn_3_675B8(u16 arg0);
void fn_3_67620(void);
void fn_3_678B8(void);
void fn_3_67A48(void);
void fn_3_67C34(void);
void fn_3_67EF0(void);
void fn_3_685F0(void);
void fn_3_68BB4(void);
void fn_3_690FC(void);
void fn_3_6916C(void);
void fn_3_69184(void);
void fn_3_692E0(void);
void fn_3_695F8(void);
void fn_3_697CC(void);

#endif // !__GAME_rep_EA0_H_
