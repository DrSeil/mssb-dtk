#include "game/gap_6B4C8.h"

#include "UnknownHeaders.h"
#include "game/UnknownHomes_Game.h"

typedef struct {
    u8 _pad[0xa0];
    s8 charIDs[0x32];
    u8 _pad2[0x31];
    s8 field_0x103;
    u8 _pad3[0x4];
} Lbl80353A90Struct6B; // size: 0x108

extern Lbl80353A90Struct6B lbl_80353A90;
extern s32 fn_80016344(s8, s32, s32, s32);

// .text:0x0006B4C8 size:0x110
void fn_3_6B4C8(void) {
    return;
}

// .text:0x0006B5D8 size:0x9C
s32 fn_3_6B5D8(void) {
    switch (lbl_3_common_bss_32724.unk9A) {
    case 0:
        g_hugeAnimStruct.field186_0x2d77 = 0;
        lbl_3_common_bss_34C58._2C = 0;
        lbl_3_common_bss_32724.unk9A = lbl_3_common_bss_32724.unk9A + 1;
        break;
    case 1:
        if (fn_80016344((s8)lbl_80353A90.field_0x103, 9, 0, 0) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}

// .text:0x0006B674 size:0x1FC
void fn_3_6B674(void) {
    return;
}

// .text:0x0006B870 size:0x1F4
void fn_3_6B870(void) {
    return;
}

// .text:0x0006BA64 size:0x308
void fn_3_6BA64(void) {
    return;
}

// .text:0x0006BD6C size:0x138
void fn_3_6BD6C(void) {
    return;
}
