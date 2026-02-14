#include "game/rep_4138.h"
#include "header_rep_data.h"

// .text:0x0016D810 size:0x1A0 mapped:0x807AC8A4
void fn_3_16D810(void) {
    return;
}

// .text:0x0016D9B0 size:0x1BC mapped:0x807ACA44
void fn_3_16D9B0(void) {
    return;
}

// .text:0x0016DB6C size:0x458 mapped:0x807ACC00
void fn_3_16DB6C(void) {
    return;
}
// .text:0x0016E2FC size:0x2C mapped:0x807AD390
void fn_3_16E2FC(u16 *ptr, s32 index) {
    if (ptr == NULL) {
        return;
    }

    // lhz r5, 0(r3) -> subi r0, r5, 1 -> cmpw r0, r4
    if ((s32)(ptr[0] - 1) < index) {
        return;
    }

    // Direct access triggers lis/stw for each global
    lbl_3_bss_D6E4 = ptr;
    lbl_3_bss_D6E8 = index;
}


// .text:0x0016E328 size:0x10 mapped:0x807AD3BC
void fn_3_16E328(void) {
    lbl_3_bss_D6EC = TRUE;
}
