#include "game/gap_249E8.h"
#include "game/gap_219CC.h"
#include "game/rep_E08.h"

#include <string.h>

// .text:0x000249E8 size:0xF4 mapped:0x80663A7C
void fn_3_249E8(s32 arg0) {
    return;
}

// .text:0x00024ADC size:0x2E0 mapped:0x80663B70
void fn_3_24ADC(s32 arg0, s32 arg1) {
    return;
}

// .text:0x00024DBC size:0x80 mapped:0x80663E50
void fn_3_24DBC(s32 arg0) {
    s32 i;

    for (i = 0; i < 13; i++) {
        lbl_3_common_bss_1323C->unk240[i] = -1;
        memset(&lbl_3_common_bss_1323C->unk0[i], 0, 0x2C);
        lbl_3_common_bss_1323C->unk261[i] = 0;
    }

    fn_3_6714C(0);

    lbl_3_common_bss_1323C->unk25C = 1;
    lbl_3_common_bss_1323C->unk23C = 0;
    lbl_3_common_bss_1323C->unk27C = 0;
    lbl_3_common_bss_1323C->unk25D = (u8)arg0;
    lbl_3_common_bss_1323C->unk25F = 1;
    lbl_3_common_bss_1323C->unk27D = 0;
    lbl_3_common_bss_1323C->unk27E = 0;
    lbl_3_common_bss_1323C->unk25A = 0;
    lbl_3_common_bss_1323C->unk280 = 1;
}

// .text:0x00024EA0 size:0x80 mapped:0x80663F34
void fn_3_24EA0(void) {
    s32 i;

    for (i = 0; i < 13; i++) {
        lbl_3_common_bss_1323C->unk240[i] = -1;
        memset(&lbl_3_common_bss_1323C->unk0[i], 0, 0x2C);
        lbl_3_common_bss_1323C->unk261[i] = 0;
    }
}

// .text:0x00024F20 size:0x4 mapped:0x80663FB4
void fn_3_24F20(void) {}

// .text:0x00024F24 size:0x1D8 mapped:0x80663FB8
void fn_3_24F24(void) {
    return;
}

// .text:0x000250FC size:0xE8 mapped:0x80664190
void fn_3_250FC(void) {
    s32 i;
    lbl_3_common_bss_1323C->unk25C = 0;

    for (i=0; i < 0x21; i++) {
        lbl_3_data_2398[i].unk34 = 0;
        lbl_3_data_2398[i].unk38 = 0;
        lbl_3_data_2398[i].unk3C = 0;
    }

    lbl_3_common_bss_1323C->unk27B = 0;
}
