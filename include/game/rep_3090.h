#ifndef __GAME_rep_3090_H_
#define __GAME_rep_3090_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"


typedef struct Struct_DE94_Inner {
    u8 _pad[0x18];
    u32 field_0x18; // 0x18
    u8 _pad2[0x99A];
    u8 field_0x9B6; // 0x9B6
} Struct_DE94_Inner;

extern Struct_DE94_Inner* lbl_3_common_bss_DE94;

typedef struct SourceObj_104B20 {
    u8 _pad[0x30];
    VecXYZ pos;
} SourceObj_104B20;

void fn_3_FC448(void);
void fn_3_FC938(void);
void fn_3_FCE38(void);
void fn_3_FCEAC(void);
void fn_3_FCEB0(void);
void fn_3_FCF20(void);
void fn_3_FCF24(void);
void fn_3_FD408(void);
void fn_3_FD4DC(void);
void fn_3_FD51C(void);
void fn_3_FD5A8(void);
void fn_3_FD670(void);
u32 fn_3_FD9FC(void);
void fn_3_FDA1C(void);
void fn_3_FDB30(void);
BOOL fn_3_100018(void);
void fn_3_100038(void);
void fn_3_10007C(void);
void fn_3_1000D8(void);
void fn_3_101CC4(void);
void fn_3_103C30(void);
void fn_3_103E7C(void);
void fn_3_1040D8(void);
void fn_3_104338(void);
void fn_3_1045A8(void);
void fn_3_104740(void);
void fn_3_1048E0(void);
void fn_3_104A3C(void);
void fn_3_104A88(void);
void fn_3_104AD4(void);
void fn_3_104B20(VecXYZ* dst, SourceObj_104B20* src);
void fn_3_104B3C(void);
void fn_3_1054CC(void);
void fn_3_1054D0(void);
void fn_3_105A10(void);
void fn_3_105ACC(void);
void fn_3_105BD8(void);
void fn_3_105C28(void);
void fn_3_105C84(void);
void fn_3_105CDC(void);
void fn_3_105E00(void);
void fn_3_106014(void);
void fn_3_1060D8(void);
void fn_3_10617C(void);
void fn_3_106270(void);
void fn_3_10698C(void);
void* fn_3_1069B0(void* base, s32 index);
void fn_3_1069C0(void);
void fn_3_106BA0(void);

#endif // !__GAME_rep_3090_H_
