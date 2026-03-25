#include "game/gap_6A160.h"

// .text:0x0006A160 size:0xF0 mapped:0x806A91F4
void fn_3_6A160(void) {
    return;
}

// .text:0x0006A250 size:0x4 mapped:0x806A92E4
void fn_3_6A250(void) {
    return;
}

// .text:0x0006A254 size:0x4 mapped:0x806A92E8
void fn_3_6A254(void) {
    return;
}

// .text:0x0006A258 size:0x4 mapped:0x806A92EC
void fn_3_6A258(void) {
    return;
}

// .text:0x0006A25C size:0x48 mapped:0x806A92F0
void fn_3_6A25C(void) {
    if (g_hugeAnimStruct.starAnimRelated != NULL) {
        g_hugeAnimStruct.starAnimRelated->unk2CE = 0;
        g_hugeAnimStruct.starAnimRelated->unk2F6 = 0;
        g_hugeAnimStruct.starAnimRelated->unk31E = 0;
        g_hugeAnimStruct.starAnimRelated->unk346 = 0;
    }
    lbl_3_common_bss_32724.someFlag = 0;
}

