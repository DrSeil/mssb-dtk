#include "game/gap_E587C.h"

#include "mssbTypes.h"

extern u8 lbl_3_data_196B4[];

// .text:0x000E587C size:0xA8
s32 fn_3_E587C(void) {
    s32 count = 0;
    s32 last;
    s32 i;
    for (i = 0; i < 7; i++) {
        if (lbl_3_data_196B4[i] != 0) {
            last = i;
            count++;
        }
    }
    if (count != 1) return -1;
    return last;
}

// .text:0x000E5924 size:0x90
s32 fn_3_E5924(void) {
    s32 result;
    s32 i;
    for (i = 0; i < 7; i++) {
        if (lbl_3_data_196B4[i] == 0) break;
    }
    result = i >= 7;
    return result;
}
