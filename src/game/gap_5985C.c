#include "game/gap_5985C.h"

// .text:0x0005985C size:0x74 mapped:0x806988F0
void fn_3_5985C(void) {
    return;
}

// .text:0x000598D0 size:0x48 mapped:0x80698964
void fn_3_598D0(void) {
    fn_3_58688();
    if (g_GameLogic.teamAIInd[g_GameLogic.awayTeamBattingInd_battingTeam] != 0) {
        fn_3_583B8();
        return;
    }
    fn_3_3B9E4();
}

// .text:0x00059918 size:0x178 mapped:0x806989AC
void fn_3_59918(s32 arg0, s32 arg1) {
    return;
}

// .text:0x00059A90 size:0x30 mapped:0x80698B24
void fn_3_59A90(void) {
    g_UnkSound_32718._02 = 0;
    g_UnkSound_32718._03 = 0;
    g_UnkSound_32718._04 = 0;
    g_UnkSound_32718._05 = 0;
    g_UnkSound_32718._06 = 0;
    g_UnkSound_32718._00 = 0;
    g_UnkSound_32718._07 = 0;
    g_UnkSound_32718._08 = 0;
}

// .text:0x00059AC0 size:0x24 mapped:0x80698B54
void fn_3_59AC0(s32 arg0, s32 arg1, s32 arg2) {
    fn_80017D28(arg2);
}

// .text:0x00059AE4 size:0x3C mapped:0x80698B78
s32 fn_3_59AE4(void) {
    if (g_d_GameSettings.GameModeSelected == 6) {
        return fn_80022B68();
    }
    return 1;
}

// .text:0x00059B20 size:0xAC mapped:0x80698BB4
void fn_3_59B20(void) {
    return;
}

// .text:0x00059BCC size:0x60 mapped:0x80698C60
s32 fn_3_59BCC(s32 arg0) {
    DrawingSceneStruct *r31 = lbl_803CC1B8;
    if (arg0 < 1) {
        return 0;
    }
    if (arg0 == 1) {
        fn_8001AAA4();
    }
    if (r31->loadingState != 0) {
        fn_3_6AEC0();
        return 1;
    }
    return 0;
}

// .text:0x00059C2C size:0x314 mapped:0x80698CC0
void fn_3_59C2C(void) {
    return;
}

// .text:0x00059F40 size:0x34C mapped:0x80698FD4
void fn_3_59F40(void) {
    return;
}

// .text:0x0005A28C size:0x3F8 mapped:0x80699320
void fn_3_5A28C(void) {
    return;
}

// .text:0x0005A684 size:0x1C mapped:0x80699718
void fn_3_5A684(void) {
    g_Strikes.strikes = 0;
    g_Strikes.balls = 0;
    g_Strikes.outs = 0;
    g_Strikes.unk_10 = 0;
}

// .text:0x0005A6A0 size:0x34 mapped:0x80699734
void fn_3_5A6A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp = arg1 ^ arg0;
    g_GameLogic.homeTeamBattingInd_fieldingTeam = arg0;
    g_GameLogic.awayTeamBattingInd_battingTeam = arg0 ^ 1;
    g_GameLogic.homeTeamInd = arg1;
    g_GameLogic.teamBatting = temp;
    g_GameLogic.teamFielding = temp ^ 1;
    g_GameLogic._1C = arg2;
    g_GameLogic._20 = arg3;
}

// .text:0x0005A6D4 size:0x28 mapped:0x80699768
void fn_3_5A6D4(u8 arg0) {
    g_GameLogic.FrameCountOfCurrentPitch = 0;
    g_GameLogic.gameStatus_prev = g_GameLogic.gameStatus;
    g_GameLogic.gameStatus = arg0;
    g_GameLogic.FrameCountOfCurrentAtBat_Copy = 0;
    g_GameLogic._125 = 0;
}

// .text:0x0005A6FC size:0x180 mapped:0x80699790
void fn_3_5A6FC(void) {
    return;
}

// .text:0x0005A87C size:0x424 mapped:0x80699910
void fn_3_5A87C(void) {
    return;
}

// .text:0x0005ACA0 size:0x16C mapped:0x80699D34
void fn_3_5ACA0(void) {
    return;
}

// .text:0x0005AE0C size:0x90 mapped:0x80699EA0
void fn_3_5AE0C(void) {
    return;
}

// .text:0x0005AE9C size:0x228 mapped:0x80699F30
void fn_3_5AE9C(void) {
    return;
}

// .text:0x0005B0C4 size:0x15C mapped:0x8069A158
void fn_3_5B0C4(void) {
    return;
}

// .text:0x0005B220 size:0x148 mapped:0x8069A2B4
void fn_3_5B220(void) {
    return;
}

// .text:0x0005B368 size:0x18 mapped:0x8069A3FC
void fn_3_5B368(void) {
    g_GameLogic.frames_memoryCardWriteOnMVP = 0;
    g_GameLogic.endGameStage = 0;
}

// .text:0x0005B380 size:0x88 mapped:0x8069A414
void fn_3_5B380(void) {
    return;
}

// .text:0x0005B408 size:0x14 mapped:0x8069A49C
void fn_3_5B408(void) {
    g_GameLogic.frame_exitMenuShowing = 0;
}

// .text:0x0005B41C size:0x184 mapped:0x8069A4B0
void fn_3_5B41C(void) {
    return;
}

// .text:0x0005B5A0 size:0x48C mapped:0x8069A634
void fn_3_5B5A0(void) {
    return;
}

// .text:0x0005BA2C size:0x314 mapped:0x8069AAC0
void fn_3_5BA2C(void) {
    return;
}

// .text:0x0005BD40 size:0x6D8 mapped:0x8069ADD4
void fn_3_5BD40(void) {
    return;
}

// .text:0x0005C418 size:0x118 mapped:0x8069B4AC
void fn_3_5C418(void) {
    return;
}

// .text:0x0005C530 size:0x98 mapped:0x8069B5C4
void fn_3_5C530(void) {
    return;
}

// .text:0x0005C5C8 size:0xD4 mapped:0x8069B65C
void fn_3_5C5C8(void) {
    return;
}

// .text:0x0005C69C size:0xB0 mapped:0x8069B730
void fn_3_5C69C(void) {
    return;
}

// .text:0x0005C74C size:0x5D8 mapped:0x8069B7E0
void fn_3_5C74C(void) {
    return;
}

// .text:0x0005CD24 size:0x90 mapped:0x8069BDB8
void fn_3_5CD24(void) {
    return;
}

// .text:0x0005CDB4 size:0x21C mapped:0x8069BE48
void fn_3_5CDB4(void) {
    return;
}

// .text:0x0005CFD0 size:0xC4 mapped:0x8069C064
void fn_3_5CFD0(void) {
    return;
}

// .text:0x0005D094 size:0x368 mapped:0x8069C128
void fn_3_5D094(void) {
    return;
}

// .text:0x0005D3FC size:0x120 mapped:0x8069C490
void fn_3_5D3FC(void) {
    return;
}

// .text:0x0005D51C size:0xCC mapped:0x8069C5B0
void fn_3_5D51C(void) {
    return;
}

// .text:0x0005D5E8 size:0x410 mapped:0x8069C67C
void fn_3_5D5E8(void) {
    return;
}

// .text:0x0005D9F8 size:0xA4 mapped:0x8069CA8C
void fn_3_5D9F8(void) {
    return;
}

// .text:0x0005DA9C size:0x244 mapped:0x8069CB30
void fn_3_5DA9C(void) {
    return;
}

// .text:0x0005DCE0 size:0x50 mapped:0x8069CD74
s32 fn_3_5DCE0(void) {
    s32 result = 0;
    if (g_Pitcher.strikeOutOrWalk == 1) {
        fn_3_736CC();
        return 1;
    }
    if (g_Pitcher.strikeOutOrWalk == 2) {
        fn_3_735A8();
        result = 1;
    }
    return result;
}

// .text:0x0005DD30 size:0x224 mapped:0x8069CDC4
void fn_3_5DD30(void) {
    return;
}

// .text:0x0005DF54 size:0x370 mapped:0x8069CFE8
void fn_3_5DF54(void) {
    return;
}

// .text:0x0005E2C4 size:0xAD4 mapped:0x8069D358
void fn_3_5E2C4(void) {
    return;
}

// .text:0x0005ED98 size:0x40 mapped:0x8069DE2C
void fn_3_5ED98(void) {
    fn_3_5A6D4(2);
    g_FieldingLogic.unkAE = 0;
    g_Pitcher.peachDaisyStarAnimationOn = 0;
}

// .text:0x0005EDD8 size:0x80 mapped:0x8069DE6C
void fn_3_5EDD8(void) {
    return;
}

// .text:0x0005EE58 size:0x194 mapped:0x8069DEEC
void fn_3_5EE58(void) {
    return;
}

// .text:0x0005EFEC size:0x168 mapped:0x8069E080
void fn_3_5EFEC(void) {
    return;
}

// .text:0x0005F154 size:0x2A8 mapped:0x8069E1E8
void fn_3_5F154(void) {
    return;
}

// .text:0x0005F3FC size:0x324 mapped:0x8069E490
void fn_3_5F3FC(void) {
    return;
}

// .text:0x0005F720 size:0x88 mapped:0x8069E7B4
void fn_3_5F720(void) {
    return;
}

// .text:0x0005F7A8 size:0x188 mapped:0x8069E83C
void fn_3_5F7A8(void) {
    return;
}

// .text:0x0005F930 size:0x558 mapped:0x8069E9C4
void fn_3_5F930(void) {
    return;
}

// .text:0x0005FE88 size:0x88 mapped:0x8069EF1C
void fn_3_5FE88(void) {
    return;
}

// .text:0x0005FF10 size:0x20C mapped:0x8069EFA4
void fn_3_5FF10(void) {
    return;
}

// .text:0x0006011C size:0x64C mapped:0x8069F1B0
void fn_3_6011C(void) {
    return;
}

