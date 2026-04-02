#include "game/gap_E587C.h"

#include "mssbTypes.h"

extern u8 lbl_3_data_196B4[];

// .text:0x000E587C size:0xA8
void fn_3_E587C(void) {
    return;
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
