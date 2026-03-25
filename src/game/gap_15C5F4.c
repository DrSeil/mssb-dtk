#include "game/gap_15C5F4.h"

// .text:0x0015C5F4 size:0x44 mapped:0x8079B688
void fn_3_15C5F4(void) {
    DrawingSceneStruct* scene = lbl_803CC1B8;
    if (g_GameLogic._125 == TRANSITION_CALCULATION_TYPE_7) {
        fn_80034CEC((u32)scene);
        fn_800B0A14_removeQueue();
    }
}

// .text:0x0015C638 size:0xAC mapped:0x8079B6CC
void fn_3_15C638(void) {
    return;
}

// .text:0x0015C6E4 size:0xAF4 mapped:0x8079B778
void fn_3_15C6E4(void) {
    return;
}

// .text:0x0015D1D8 size:0x96C mapped:0x8079C26C
void fn_3_15D1D8(void) {
    return;
}

// .text:0x0015DB44 size:0x12E8 mapped:0x8079CBD8
void fn_3_15DB44(void) {
    return;
}

// .text:0x0015EE2C size:0x25C mapped:0x8079DEC0
void fn_3_15EE2C(void) {
    return;
}

// .text:0x0015F088 size:0x198 mapped:0x8079E11C
void fn_3_15F088(void) {
    return;
}

// .text:0x0015F220 size:0x160 mapped:0x8079E2B4
void fn_3_15F220(void) {
    return;
}

// .text:0x0015F380 size:0x44 mapped:0x8079E414
void fn_3_15F380(void) {
    DrawingSceneStruct* scene = lbl_803CC1B8;
    if (g_GameLogic._125 == TRANSITION_CALCULATION_TYPE_10) {
        fn_80034CEC((u32)scene);
        fn_800B0A14_removeQueue();
    }
}

// .text:0x0015F3C4 size:0x4C mapped:0x8079E458
void fn_3_15F3C4(void) {
    fn_80034E20((void(*)(void))lbl_803CC1B8, (int*)&lbl_3_data_273E0, (void**)&lbl_803CC1B8);
    lbl_803CC1B8->aFunctionPointer = fn_3_15F380;
}

// .text:0x0015F410 size:0x164 mapped:0x8079E4A4
void fn_3_15F410(void) {
    return;
}

