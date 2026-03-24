#include "game/gap_C1930.h"

// .text:0x000C1930 size:0x34 mapped:0x807009C4
s32 fn_3_C1930(s32 arg0) {
    s32 i = 1;
    Data17260Entry* entry = &lbl_3_data_17260[1];
    do {
        if (arg0 == entry->field0) break;
        entry--;
    } while (i--);
    return i;
}

