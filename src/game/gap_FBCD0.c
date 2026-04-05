#include "game/gap_FBCD0.h"

extern u32 lbl_3_data_20E0C[];

// .text:0x000FBCD0 size:0x88 mapped:0x8073AD64
void fn_3_FBCD0(void) {
    return;
}

// .text:0x000FBD58 size:0x18 mapped:0x8073ADEC
void fn_3_FBD58(void) {
    lbl_3_common_bss_DE94->field_0x9AC = 0;
}

// .text:0x000FBD70 size:0x3C mapped:0x8073AE04
void fn_3_FBD70(void) {
    BssDE94Item* p = lbl_3_common_bss_DE94;
    p->field_0x9AF = 0;
    lbl_3_common_bss_DE94->field_0x9B0 = 0;
    lbl_3_common_bss_DE94->field_0x9AE = 0;
    lbl_3_common_bss_DE94->field_0x9AB = 0;
    lbl_3_common_bss_DE94->field_0x9B6 = 0;
    p->field_0x18 = 0;
}

// .text:0x000FBDAC size:0x78 mapped:0x8073AE40
void fn_3_FBDAC(u32 arg0) {
    lbl_3_common_bss_DE94->field_0x9AB = 1;
    lbl_3_common_bss_DE94->field_0x9AC = 1;
    lbl_3_common_bss_DE94->field_0x9AF = 1;
    lbl_3_common_bss_DE94->field_0x9B0 = 1;
    lbl_3_common_bss_DE94->field_0x18 = 0;
    lbl_3_common_bss_DE94->unk_08 = 0;
    lbl_3_common_bss_DE94->unk_14 = 0;
    lbl_3_common_bss_DE94->unk_92C = arg0;
    lbl_3_common_bss_DE94->unk_00 = lbl_3_data_20E0C[arg0];
    lbl_3_common_bss_DE94->field_0x9AE = 1;
    lbl_3_common_bss_DE94->unk_118 = 0;
    lbl_3_common_bss_DE94->unk_944 = 0;
}

// .text:0x000FBE24 size:0x4D8 mapped:0x8073AEB8
void fn_3_FBE24(void) {
    return;
}

// .text:0x000FC2FC size:0x14C mapped:0x8073B390
void fn_3_FC2FC(void) {
    return;
}

