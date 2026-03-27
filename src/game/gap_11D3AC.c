#include "game/gap_11D3AC.h"

// .text:0x0011D3AC size:0x2F4 mapped:0x8075C440
void fn_3_11D3AC(void) {
    return;
}

// .text:0x0011D6A0 size:0xE0 mapped:0x8075C734
void fn_3_11D6A0(void) {
    return;
}

// .text:0x0011D780 size:0x34C mapped:0x8075C814
void fn_3_11D780(void) {
    return;
}

// .text:0x0011DACC size:0x2F8 mapped:0x8075CB60
void fn_3_11DACC(void) {
    return;
}

// .text:0x0011DDC4 size:0xBC mapped:0x8075CE58
void fn_3_11DDC4(void) {
    return;
}

// .text:0x0011DE80 size:0x4C mapped:0x8075CF14
void fn_3_11DE80(void) {
    if (g_GameLogic._125 == 0) {
        g_Minigame.unk1E04 = fn_800B0A5C_insertQueue(fn_3_11DECC, 2);
    }
}

// .text:0x0011DECC size:0x43C mapped:0x8075CF60
void fn_3_11DECC(void) {
    return;
}

// .text:0x0011E308 size:0x5C mapped:0x8075D39C
void fn_3_11E308(void) {
    if (g_GameLogic._125 == 1 && *(u16*)&g_GameLogic.FrameCountOfCurrentAtBat_Copy == 0) {
        ((DrawingSceneStruct*)fn_800B0A5C_insertQueue(fn_3_E911C, 2))->unk18 = 1;
    }
}

// .text:0x0011E364 size:0x460 mapped:0x8075D3F8
void fn_3_11E364(void) {
    return;
}

// .text:0x0011E7C4 size:0x464 mapped:0x8075D858
void fn_3_11E7C4(void) {
    return;
}

