#include "game/gap_7976C.h"

// .text:0x0007976C size:0x294 mapped:0x806B8800
void fn_3_7976C(void) {
    return;
}

// .text:0x00079A00 size:0xCC mapped:0x806B8A94
void fn_3_79A00(void) {
    return;
}

// .text:0x00079ACC size:0x308 mapped:0x806B8B60
void fn_3_79ACC(void) {
    return;
}

// .text:0x00079DD4 size:0x120 mapped:0x806B8E68
void fn_3_79DD4(void) {
    return;
}

// .text:0x00079EF4 size:0x260 mapped:0x806B8F88
void fn_3_79EF4(void) {
    return;
}

// .text:0x0007A154 size:0x9E0 mapped:0x806B91E8
void fn_3_7A154(void) {
    return;
}

// .text:0x0007AB34 size:0x44 mapped:0x806B9BC8
void fn_3_7AB34(void) {
    StatTableEntry* entry = &lbl_803535C8[g_GameLogic.teamFielding][g_GameLogic.battingOrderAndPositionMapping[g_GameLogic.awayTeamBattingInd_battingTeam][0][0]];
    entry->unkE++;
}

// .text:0x0007AB78 size:0x1F0 mapped:0x806B9C0C
void fn_3_7AB78(void) {
    return;
}

// .text:0x0007AD68 size:0x140 mapped:0x806B9DFC
void fn_3_7AD68(void) {
    return;
}

// .text:0x0007AEA8 size:0x40 mapped:0x806B9F3C
s32 fn_3_7AEA8(void) {
    s32 team = g_GameLogic.homeTeamBattingInd_fieldingTeam;
    s32 bssVal = lbl_3_common_bss_32A94.unk44[team];
    s32 currentBatter = g_GameLogic.currentBatterPerTeam[team];
    return currentBatter + (bssVal - 1) * 9 - 1;
}

// .text:0x0007AEE8 size:0x4 mapped:0x806B9F7C
void fn_3_7AEE8(void) {
    return;
}

// .text:0x0007AEEC size:0x7C mapped:0x806B9F80
void fn_3_7AEEC(void) {
    return;
}

// .text:0x0007AF68 size:0x7C mapped:0x806B9FFC
void fn_3_7AF68(void) {
    return;
}

// .text:0x0007AFE4 size:0x14C mapped:0x806BA078
void fn_3_7AFE4(void) {
    return;
}

// .text:0x0007B130 size:0x1D8 mapped:0x806BA1C4
void fn_3_7B130(void) {
    return;
}

// .text:0x0007B308 size:0x86C mapped:0x806BA39C
void fn_3_7B308(void) {
    return;
}

// .text:0x0007BB74 size:0x4C mapped:0x806BAC08
BatterStatEntry* fn_3_7BB74(void) {
    s32 fieldingTeam = g_GameLogic.homeTeamBattingInd_fieldingTeam;
    s32 batting = g_GameLogic.teamBatting;
    s32 currentBatter = g_GameLogic.currentBatterPerTeam[fieldingTeam];
    s32 rosterSlot = g_GameLogic.battingOrderAndPositionMapping[fieldingTeam][currentBatter][0];
    return &lbl_803537E4[batting][rosterSlot];
}

// .text:0x0007BBC0 size:0x38 mapped:0x806BAC54
StatTableEntry* fn_3_7BBC0(void) {
    s32 awayTeam = g_GameLogic.awayTeamBattingInd_battingTeam;
    s32 teamFielding = g_GameLogic.teamFielding;
    s32 rosterSlot = g_GameLogic.battingOrderAndPositionMapping[awayTeam][0][0];
    return &lbl_803535C8[teamFielding][rosterSlot];
}

// .text:0x0007BBF8 size:0x14 mapped:0x806BAC8C
void fn_3_7BBF8(void) {
    g_Stats.unk39 = 1;
}

// .text:0x0007BC0C size:0x14 mapped:0x806BACA0
void fn_3_7BC0C(void) {
    g_Stats.unk39 = 1;
}

// .text:0x0007BC20 size:0x570 mapped:0x806BACB4
void fn_3_7BC20(void) {
    return;
}

// .text:0x0007C190 size:0x4 mapped:0x806BB224
void fn_3_7C190(void) {
    return;
}

// .text:0x0007C194 size:0x68 mapped:0x806BB228
void fn_3_7C194(void) {
    return;
}

