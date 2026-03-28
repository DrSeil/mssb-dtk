#include "game/gap_114FC0.h"
#include "UnknownHeaders.h"

// .text:0x00114FC0 size:0x6C mapped:0x80754054
void fn_3_114FC0(void) {
    u8 temp = g_Minigame.turnNumberWithinRound + 1;
    g_GameLogic.pre_PostMiniGameInd = 1;
    g_GameLogic.minigameLastTurnSuccessInd = 1;
    g_GameLogic.hudLoadingRelated = 1;
    g_Minigame.turnNumberWithinRound = temp;
    if (temp >= g_Minigame.miniGameNumberOfParticipants) {
        fn_3_5A6D4(0x19);
    } else {
        fn_3_5A6D4(7);
    }
}

// .text:0x0011502C size:0xDC mapped:0x807540C0
void fn_3_11502C(void) {
    return;
}

// .text:0x00115108 size:0x438 mapped:0x8075419C
void fn_3_115108(void) {
    return;
}

// .text:0x00115540 size:0x1F8 mapped:0x807545D4
void fn_3_115540(void) {
    return;
}

// .text:0x00115738 size:0xF0 mapped:0x807547CC
void fn_3_115738(void) {
    return;
}

// .text:0x00115828 size:0x88 mapped:0x807548BC
void fn_3_115828(void) {
    return;
}

// .text:0x001158B0 size:0x48 mapped:0x80754944
void fn_3_1158B0(void) {
    if (g_Scores.highscore >= g_Scores.threshold) {
        fn_3_5A6D4(0xF);
        return;
    }
    fn_3_5A6D4(6);
}

// .text:0x001158F8 size:0x80 mapped:0x8075498C
void fn_3_1158F8(void) {
    return;
}

// .text:0x00115978 size:0x13C mapped:0x80754A0C
void fn_3_115978(void) {
    return;
}

// .text:0x00115AB4 size:0xA8 mapped:0x80754B48
void fn_3_115AB4(void) {
    return;
}

// .text:0x00115B5C size:0x80 mapped:0x80754BF0
void fn_3_115B5C(void) {
    return;
}

// .text:0x00115BDC size:0x48 mapped:0x80754C70
void fn_3_115BDC(void) {
    sndFXStartEx(0x1BD, lbl_800EFBA4.volume, 0x3F, 0);
    fn_3_114A88(1);
    fn_3_5A6D4(6);
}

// .text:0x00115C24 size:0x494 mapped:0x80754CB8
void fn_3_115C24(void) {
    return;
}

// .text:0x001160B8 size:0x4 mapped:0x8075514C
void fn_3_1160B8(void) {
    return;
}

// .text:0x001160BC size:0x5E0 mapped:0x80755150
void fn_3_1160BC(void) {
    return;
}

// .text:0x0011669C size:0x30 mapped:0x80755730
void fn_3_11669C(void) {
    g_hugeAnimStruct.entries_2D94[240].unk_26 = 0;
    g_hugeAnimStruct.entries_2D94[241].unk_26 = 0;
    g_hugeAnimStruct.entries_2D94[242].unk_26 = 0;
    g_hugeAnimStruct.entries_2D94[243].unk_26 = 0;
}

// .text:0x001166CC size:0xC0 mapped:0x80755760
void fn_3_1166CC(void) {
    return;
}

// .text:0x0011678C size:0xB4 mapped:0x80755820
void fn_3_11678C(void) {
    return;
}

