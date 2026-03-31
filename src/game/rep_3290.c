#include "game/rep_3290.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/rep_3880.h"

// .text:0x001133C4 size:0x338 mapped:0x80752458
void fn_3_1133C4(void) {
    return;
}

// .text:0x001136FC size:0x220 mapped:0x80752790
void fn_3_1136FC(void) {
    return;
}

// .text:0x0011391C size:0x34 mapped:0x807529B0

void fn_3_11391C(void) {
    /* * Using the struct member directly ensures the compiler generates 
     * the two-step address calculation:
     * 1. lis r3, g_Minigame@ha / addi r3, r3, g_Minigame@l
     * 2. addi r3, r3, 0x1d7c
     */
    memset(g_Minigame._1D7C, 0, 0x78);
}

// .text:0x00113950 size:0xF8 mapped:0x807529E4
void fn_3_113950(void) {
    return;
}

// .text:0x00113A48 size:0x2D8 mapped:0x80752ADC
void fn_3_113A48(void) {
    return;
}

// .text:0x00113D20 size:0x1A0 mapped:0x80752DB4
void fn_3_113D20(void) {
    return;
}

// .text:0x00113EC0 size:0x54 mapped:0x80752F54
void fn_3_113EC0(void) {
    g_Pitcher.ballVelocity.z = -g_Pitcher.ballVelocity.z;
    g_Minigame.ballStoppedBreakingWallsInd = 1;
    g_Pitcher.ballVelocity.x *= lbl_3_data_21674;
    g_Pitcher.ballVelocity.y *= lbl_3_data_21674;
    g_Pitcher.ballVelocity.z *= lbl_3_data_21674;
}

// .text:0x00113F14 size:0x2F0 mapped:0x80752FA8
void fn_3_113F14(void) {
    return;
}

// .text:0x00114204 size:0x180 mapped:0x80753298
void fn_3_114204(void) {
    return;
}

// .text:0x00114384 size:0x634 mapped:0x80753418
void fn_3_114384(void) {
    return;
}

// .text:0x001149B8 size:0x74 mapped:0x80753A4C
s32 fn_3_1149B8(u8* arg0, u8* arg1) {
    u8 temp_r5;
    WallBallPlayerSortInfo* player0Info = &g_Minigame.wallBallSortInfoOverlay.wallBallPlayerSortInfo[*arg0];
    WallBallPlayerSortInfo* player1Info = &g_Minigame.wallBallSortInfoOverlay.wallBallPlayerSortInfo[*arg1];

    temp_r5 = player0Info->_28;

    if (temp_r5 == 3 && player1Info->_28 != 3) {
        return -1;
    }
    if (temp_r5 != 3 && player1Info->_28 == 3) {
        return 1;
    }
    return player0Info->_2A - player1Info->_2A;
}

// .text:0x00114A2C size:0x5C mapped:0x80753AC0
void fn_3_114A2C(void) {
    u8 state = g_Minigame.wallBallGameState;
    if (state == 0) {
        fn_3_114384();
        fn_3_1500C8();
    } else if (state == 1) {
        fn_3_114204();
    } else if (state == 2) {
        fn_3_113F14();
        fn_3_113D20();
    }
}

// .text:0x00114A88 size:0x538 mapped:0x80753B1C
void fn_3_114A88(s32 arg0) {
    return;
}
