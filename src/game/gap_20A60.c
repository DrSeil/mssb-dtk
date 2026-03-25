#include "game/gap_20A60.h"
#include "game/UnknownHomes_Game.h"

// .text:0x00020A60 size:0x50 mapped:0x8065FAF4
void fn_3_20A60(void) {
    g_AiLogic.boxHorizontalPoint = g_Batter.batPosition2.x;
    g_AiLogic.boxVerticalPoint = g_Batter.batPosition2.z;
    g_AiLogic.batterAIDesiredZPosInBox = maybeInitialBatPos.z;
    g_AiLogic.batterAIABStrat = -1;
    g_AiLogic._67 = 0;
    g_AiLogic.batterAIBuntPossibility = 1;
    g_AiLogic.batterAIBuntInd = 0;
}

// .text:0x00020AB0 size:0x30 mapped:0x8065FB44
void fn_3_20AB0(void) {
    g_AiLogic.lastPitchFramesUntilPitchGetsToBatter = 0;
    g_AiLogic.lastPitchType = 0xFF;
    g_AiLogic.starRelated[0] = 0;
    g_AiLogic.starRelated[1] = 0;
    g_AiLogic.lastPitchBallLocZone = 0xFF;
    g_AiLogic.lastPitchMoundZone = 0xFF;
    g_AiLogic.batterAIStealIndicator = 0;
}

// .text:0x00020AE0 size:0x50 mapped:0x8065FB74
void fn_3_20AE0(void) {
    f32 r938 = lbl_3_rodata_938;
    f32 z = maybeInitialBatPos.z;
    f32 x = lbl_3_rodata_930;
    f32 r93C = lbl_3_rodata_93C;
    g_AiLogic.batterAIDesiredXPosInBox = x;
    g_AiLogic.batterAIDesiredZPosInBox = z;
    g_AiLogic._10 = r938;
    g_AiLogic._14 = r93C;
    g_AiLogic._18 = r938;
    g_AiLogic._1C = r93C;
    g_AiLogic.batterAIDesiredXPosInBox = x;
    g_AiLogic.batterAIDesiredZPosInBox = z;
}

// .text:0x00020B30 size:0x104 mapped:0x8065FBC4
void fn_3_20B30(void) {
    return;
}

// .text:0x00020C34 size:0xB8 mapped:0x8065FCC8
void fn_3_20C34(void) {
    return;
}

