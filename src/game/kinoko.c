#include "game/kinoko.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/rep_3E58.h"
#include "stl/mem.h"

static KinokoState lbl_3_bss_BA00;

// .text:0x001695A4 size:0x5C mapped:0x807A8638
void fn_3_1695A4(s8 arg1, u8 arg2) {
    KinokoState *state = &lbl_3_bss_BA00;
    if (arg2 == 0) {
        state->ptr0 = &state->_unk08[0x98];
        state->ptr4 = &state->_unk08[0x24];
    } else {
        state->ptr0 = &state->_unk08[0x58];
        state->ptr4 = &state->_unk08[0x4];
    }
    fn_80011604(arg1, fn_3_16917C);
}

// .text:0x00169600 size:0x204 mapped:0x807A8694
void fn_3_169600(void) {
    return;
}

// .text:0x00169804 size:0x180 mapped:0x807A8898
void fn_3_169804(void) {
    return;
}

// .text:0x00169984 size:0x37C mapped:0x807A8A18
void fn_3_169984(void) {
    return;
}

// .text:0x00169D00 size:0x170 mapped:0x807A8D94
void fn_3_169D00(void) {
    return;
}

// .text:0x00169E70 size:0x20C mapped:0x807A8F04
void fn_3_169E70(void) {
    return;
}

// .text:0x0016A07C size:0x140C mapped:0x807A9110
void fn_3_16A07C(void) {
    return;
}

// .text:0x0016B488 size:0x12C mapped:0x807AA51C
void fn_3_16B488(void) {
    return;
}

// .text:0x0016B5B4 size:0x2D0 mapped:0x807AA648
void fn_3_16B5B4(void) {
    return;
}

// .text:0x0016B884 size:0xB10 mapped:0x807AA918
void fn_3_16B884(void) {
    return;
}

// .text:0x0016C394 size:0x78 mapped:0x807AB428
void fn_3_16C394(s8 arg0) {
    memset(&lbl_3_data_28928, 0, 0x19E0);
    memset(&lbl_3_bss_BAE0, 0, 0x1BF0);
    lbl_3_data_28928.unk_19DC = arg0;
    lbl_3_data_28928.unk_19DD = 1;
    lbl_3_data_28928.unk_19D4 = 1;
    fn_3_16B884();
    fn_800B0A5C_insertQueue(fn_3_16A07C, 0);
}

