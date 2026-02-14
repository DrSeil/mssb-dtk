#include "game/rep_1D58.h"
#include "header_rep_data.h"

extern u8 lbl_3_data_11168[16];

static struct StadiumObject* lbl_3_bss_190C;
static u8 lbl_3_bss_1902;
static struct StadiumObject* lbl_3_bss_9940;

// .text:0x000B7FC8 size:0x108 mapped:0x806F705C
void fn_3_B7FC8(void) {
    return;
}

// .text:0x000B80D0 size:0xB4 mapped:0x806F7164
void fn_3_B80D0(void) {
    return;
}

// .text:0x000B8184 size:0xF8 mapped:0x806F7218
void fn_3_B8184(void) {
    return;
}

// .text:0x000B827C size:0x10 mapped:0x806F7310
struct StadiumObject* fn_3_B827C(void) {
    return lbl_3_bss_190C;
}

// .text:0x000B828C size:0xC mapped:0x806F7320
void fn_3_B828C(struct StadiumObject* arg0) {
    lbl_3_bss_190C = arg0;
}

// .text:0x000B8298 size:0x17C mapped:0x806F732C
void fn_3_B8298(void) {
    return;
}

// .text:0x000B8414 size:0x50 mapped:0x806F74A8
void fn_3_B8414(void) {
    return;
}

// .text:0x000B8464 size:0x110 mapped:0x806F74F8
void fn_3_B8464(void) {
    return;
}

// .text:0x000B8574 size:0x34 mapped:0x806F7608
void fn_3_B8574(void) {
    return;
}

// .text:0x000B85A8 size:0x34 mapped:0x806F763C
void fn_3_B85A8(void) {
    return;
}

// .text:0x000B85DC size:0x7C mapped:0x806F7670
void fn_3_B85DC(void) {
    return;
}

// .text:0x000B8658 size:0x24 mapped:0x806F76EC
s32 fn_3_B8658(f32* a, f32* b) {
    f32 fa = *a;
    f32 fb = *b;
    if (fa < fb) {
        return -1;
    }
    return fa > fb;
}

// .text:0x000B867C size:0x1AC mapped:0x806F7710
void fn_3_B867C(void) {
    return;
}

// .text:0x000B8828 size:0x3E0 mapped:0x806F78BC
void fn_3_B8828(void) {
    return;
}

// .text:0x000B8C08 size:0x424 mapped:0x806F7C9C
void fn_3_B8C08(void) {
    return;
}

// .text:0x000B902C size:0x60 mapped:0x806F80C0
void fn_3_B902C(void) {
    return;
}

// .text:0x000B908C size:0x98 mapped:0x806F8120
void fn_3_B908C(void) {
    return;
}

// .text:0x000B9124 size:0x48 mapped:0x806F81B8
void fn_3_B9124(void) {
    return;
}

// .text:0x000B916C size:0x5C mapped:0x806F8200
int processStadiumObjectFunction(int, struct StadiumObject*, int, struct StadiumObject*) {
    return;
}

// .text:0x000B91C8 size:0x1D4 mapped:0x806F825C
void fn_3_B91C8(void) {
    return;
}

// .text:0x000B939C size:0x28 mapped:0x806F8430
void fn_3_B939C(void) {
    return;
}

// .text:0x000B93C4 size:0x4 mapped:0x806F8458
void fn_3_B93C4(void) {
    return;
}

// .text:0x000B93C8 size:0x4 mapped:0x806F845C
void fn_3_B93C8(void) {
    return;
}

// .text:0x000B93CC size:0x140 mapped:0x806F8460
void fn_3_B93CC(void) {
    return;
}

// .text:0x000B950C size:0x4 mapped:0x806F85A0
void fn_3_B950C(void) {
    return;
}

// .text:0x000B9510 size:0x14 mapped:0x806F85A4
void fn_3_B9510(int idx) {
    lbl_3_data_11168[idx] = 1;
}

// .text:0x000B9524 size:0x10 mapped:0x806F85B8
void fn_3_B9524(void) {
    lbl_3_bss_1902 = 0;
}

// .text:0x000B9534 size:0xB8 mapped:0x806F85C8
void fn_3_B9534(void) {
    return;
}

// .text:0x000B95EC size:0x1DC mapped:0x806F8680
void fn_3_B95EC(void) {
    return;
}

// .text:0x000B97C8 size:0x14 mapped:0x806F885C
void fn_3_B97C8(struct StadiumObject* arg0) {
    if (arg0 != NULL) {
        lbl_3_bss_9940 = arg0;
    }
}

// .text:0x000B97DC size:0x10C mapped:0x806F8870
void fn_3_B97DC(void) {
    return;
}

// .text:0x000B98E8 size:0xFC mapped:0x806F897C
void fn_3_B98E8(void) {
    return;
}

// .text:0x000B99E4 size:0x1D0 mapped:0x806F8A78
void fn_3_B99E4(void) {
    return;
}

// .text:0x000B9BB4 size:0x1B4 mapped:0x806F8C48
void fn_3_B9BB4(void) {
    return;
}

// .text:0x000B9D68 size:0x250 mapped:0x806F8DFC
void fn_3_B9D68(void) {
    return;
}

// .text:0x000B9FB8 size:0x198 mapped:0x806F904C
void fn_3_B9FB8(void) {
    return;
}
