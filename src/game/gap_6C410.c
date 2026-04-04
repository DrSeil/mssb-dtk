#include "game/gap_6C410.h"

// .text:0x0006C410 size:0x18 mapped:0x806AB4A4
void fn_3_6C410(void) {
    g_hugeAnimStruct.starAnimRelated->state_alt = 0;
}

// .text:0x0006C428 size:0x4 mapped:0x806AB4BC
void fn_3_6C428(void) {
    return;
}

// .text:0x0006C42C size:0x28 mapped:0x806AB4C0
void fn_3_6C42C(void) {
    g_hugeAnimStruct.starAnimRelated->state_alt = 0;
    lbl_3_common_bss_32724.unk_90 = -1;
}

// .text:0x0006C454 size:0x78 mapped:0x806AB4E8
void fn_3_6C454(void) {
    AnimationStruct** ptr = &g_hugeAnimStruct.AnimationStructPtrs[9];
    s32 i;
    for (i = 0; i < 4; i++, ptr++) {
        if (*ptr != NULL) {
            (*ptr)->unk_25D = 0;
        }
    }
    fn_3_6A25C();
    fn_3_6A250();
}

// .text:0x0006C4CC size:0x4 mapped:0x806AB560
void fn_3_6C4CC(void) {
    return;
}
