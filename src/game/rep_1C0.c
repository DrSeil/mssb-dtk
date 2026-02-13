#include "game/rep_1C0.h"
#include "header_rep_data.h"

extern u8 lbl_803CBBC0;
extern struct LBL_3_DATA_A10* lbl_3_data_A10[2];
extern void fn_80052694(struct Fn_80052694_Arg*);

static struct FanObject* lbl_3_bss_18[6];

// .text:0x000035D4 size:0x10
struct FanObject* setFanObjPtr(void) {
    return lbl_3_bss_18[0];
}

// .text:0x000035E4 size:0xC
void fn_3_35E4(struct FanObject* arg0) {
    lbl_3_bss_18[0] = arg0;
}

// .text:0x000038E8 size:0x1C
void fn_3_38E8(struct LBL_3_DATA_A10* arg0) {
    lbl_3_data_A10[lbl_803CBBC0] = arg0;
}

// .text:0x00003904 size:0x2E4 mapped:0x80642998
void fn_3_3904(void) {
    return;
}

// .text:0x00003BE8 size:0x300 mapped:0x80642C7C
void fn_3_3BE8(void) {
    return;
}

// .text:0x00003EE8 size:0x3E4 mapped:0x80642F7C
void fn_3_3EE8(void) {
    return;
}

// .text:0x000042CC size:0x6B8 mapped:0x80643360
void fn_3_42CC(void) {
    return;
}

// .text:0x00004984 size:0xB4 mapped:0x80643A18
void fn_3_4984(void) {
    return;
}

// .text:0x00004A38 size:0x558 mapped:0x80643ACC
void fn_3_4A38(void) {
    return;
}

// .text:0x00004F90 size:0x450 mapped:0x80644024
void fn_3_4F90(void) {
    return;
}

// .text:0x000053E0 size:0x138 mapped:0x80644474
void fn_3_53E0(void) {
    return;
}

// .text:0x00005518 size:0x164 mapped:0x806445AC
void fn_3_5518(void) {
    return;
}

// .text:0x0000567C size:0x530 mapped:0x80644710
void fn_3_567C(void) {
    return;
}

// .text:0x00005BAC size:0x20 mapped:0x80644C40
void fn_3_5BAC(void) {
    fn_3_567C();
}

// .text:0x00005BCC size:0x24 mapped:0x80644C60
typedef struct Fn3_5BCCArg {
    u8 _pad[0x8];
    struct Fn_80052694_Arg* unk_0x8;
} Fn3_5BCCArg;

void fn_3_5BCC(Fn3_5BCCArg* arg0) {
    Fn3_5BCCArg* s = arg0;
    fn_80052694(s->unk_0x8);
}

// .text:0x00005BF0 size:0x78 mapped:0x80644C84
void fn_3_5BF0(void) {
    return;
}

// .text:0x00005C68 size:0x1F8 mapped:0x80644CFC
void fn_3_5C68(void) {
    return;
}
