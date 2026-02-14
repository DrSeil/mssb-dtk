#include "game/rep_540.h"
#include "header_rep_data.h"

#include "game/UnknownHomes_Game.h"

// .text:0x00006530 size:0x78 mapped:0x806455C4
void fn_3_6530(void) {
    return;
}

// .text:0x000065A8 size:0x20 mapped:0x8064563C
void fn_3_65A8(void) {
    if (g_Ball.ballState == 2) {
        g_Ball.ballState = 1;
    }
}



// .text:0x000065C8 size:0x2C mapped:0x8064565C
void fn_3_65C8(void) {
    if (g_Ball.matchFramesAndBallAngle.framesInsidePlant < 0x7FFE) {
        g_Ball.matchFramesAndBallAngle.framesInsidePlant++;
    } else {
        g_Ball.matchFramesAndBallAngle.framesInsidePlant = 0x7FFF;
    }
}

// .text:0x000065F4 size:0x2C mapped:0x80645688
void fn_3_65F4(void) {
    // Ordering is critical to match the interleaving of r5 and r3 stores
    g_Ball.pauseBallMovementWhenInPlant = 0;
    g_FieldingLogic.unk_13B = 1;
    g_Ball.frameCountdownAfterLeavingPlant = 3;
}

// .text:0x00006620 size:0x74 mapped:0x806456B4
void fn_3_6620(void) {
    return;
}

// .text:0x00006694 size:0x5A4 mapped:0x80645728
void fn_3_6694(void) {
    return;
}

// .text:0x00006C38 size:0x20B8 mapped:0x80645CCC
void fn_3_6C38(void) {
    return;
}

// .text:0x00008CF0 size:0x35C mapped:0x80647D84
void fn_3_8CF0(void) {
    return;
}

// .text:0x0000904C size:0x214 mapped:0x806480E0
void fn_3_904C(void) {
    return;
}

// .text:0x00009260 size:0x2A8 mapped:0x806482F4
void fn_3_9260(void) {
    return;
}

// .text:0x00009508 size:0x300 mapped:0x8064859C
void fn_3_9508(void) {
    return;
}

// .text:0x00009808 size:0x36C mapped:0x8064889C
void fn_3_9808(void) {
    return;
}

// .text:0x00009B74 size:0x16C mapped:0x80648C08
void fn_3_9B74(void) {
    return;
}

// .text:0x00009CE0 size:0x138 mapped:0x80648D74
void fn_3_9CE0(void) {
    return;
}

// .text:0x00009E18 size:0x6C mapped:0x80648EAC
void fn_3_9E18(void) {
    return;
}

// .text:0x00009E84 size:0x120 mapped:0x80648F18
void fn_3_9E84(void) {
    return;
}

// .text:0x00009FA4 size:0x7C mapped:0x80649038
void fn_3_9FA4(void) {
    return;
}

// .text:0x0000A020 size:0xD0 mapped:0x806490B4
void fn_3_A020(void) {
    return;
}

// .text:0x0000A0F0 size:0xA8 mapped:0x80649184
void fn_3_A0F0(void) {
    return;
}

// .text:0x0000A198 size:0x6A4 mapped:0x8064922C
void fn_3_A198(void) {
    return;
}

// .text:0x0000A83C size:0x134 mapped:0x806498D0
void fn_3_A83C(void) {
    return;
}

// .text:0x0000A970 size:0xAD0 mapped:0x80649A04
void fn_3_A970(void) {
    return;
}

// .text:0x0000B440 size:0x500 mapped:0x8064A4D4
void fn_3_B440(void) {
    return;
}

// .text:0x0000B940 size:0x27C mapped:0x8064A9D4
void fn_3_B940(void) {
    return;
}

// .text:0x0000BBBC size:0x98 mapped:0x8064AC50
void fn_3_BBBC(void) {
    return;
}

// .text:0x0000BC54 size:0x124 mapped:0x8064ACE8
void fn_3_BC54(void) {
    return;
}

// .text:0x0000BD78 size:0x2BC mapped:0x8064AE0C
void fn_3_BD78(void) {
    return;
}

// .text:0x0000C034 size:0x9C0 mapped:0x8064B0C8
void fn_3_C034(void) {
    return;
}

// .text:0x0000C9F4 size:0x434 mapped:0x8064BA88
void fn_3_C9F4(void) {
    return;
}

// .text:0x0000CE28 size:0xBC4 mapped:0x8064BEBC
void estimateAndSetFutureCoords(int) {
    return;
}

// .text:0x0000D9EC size:0x1E4 mapped:0x8064CA80
void fn_3_D9EC(void) {
    return;
}

// .text:0x0000DBD0 size:0x78 mapped:0x8064CC64
void fn_3_DBD0(void) {
    return;
}

// .text:0x0000DC48 size:0x68C mapped:0x8064CCDC
void fn_3_DC48(void) {
    return;
}

// .text:0x0000E2D4 size:0xB78 mapped:0x8064D368
void fn_3_E2D4(void) {
    return;
}

// .text:0x0000EE4C size:0x390 mapped:0x8064DEE0
void fn_3_EE4C(void) {
    return;
}

// .text:0x0000F1DC size:0x39C mapped:0x8064E270
void fn_3_F1DC(void) {
    return;
}

// .text:0x0000F578 size:0x240 mapped:0x8064E60C
void fn_3_F578(void) {
    return;
}

// .text:0x0000F7B8 size:0x240 mapped:0x8064E84C
void fn_3_F7B8(void) {
    return;
}

// .text:0x0000F9F8 size:0x1B0 mapped:0x8064EA8C
void fn_3_F9F8(void) {
    return;
}

// .text:0x0000FBA8 size:0x3A4 mapped:0x8064EC3C
void fn_3_FBA8(void) {
    return;
}
