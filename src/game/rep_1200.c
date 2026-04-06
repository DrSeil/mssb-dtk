#include "game/rep_1200.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_Static.h"

extern f32 lbl_3_rodata_1250;
extern void fn_3_5C69C(void*, InMemPitcherType*);

// .text:0x0006F6CC size:0x7C mapped:0x806AE760
s32 fn_3_6F6CC(void) {
        f32 limit;
    InMemRunnerType* runner;

    s32 i;
    
    limit = lbl_3_rodata_1250;

    runner = g_Runners;
    
    for (i = 1; i < 4; i++) {
        if (runner[i].runnerOnFieldOrOutOrScored == 1 && runner[i].percentTowardsNextBase > limit) {
            g_Pitcher.pickOffLoc = 4;
            fn_3_5C69C(0, &g_Pitcher);
            return 1;
        }
    }

    return 0;
}

// .text:0x0006F748 size:0x2E0 mapped:0x806AE7DC
void fn_3_6F748(void) {
    return;
}

// .text:0x0006FA28 size:0x170 mapped:0x806AEABC
void fn_3_6FA28(void) {
    return;
}

// .text:0x0006FB98 size:0x208 mapped:0x806AEC2C
void fn_3_6FB98(void) {
    return;
}

// .text:0x0006FDA0 size:0x224 mapped:0x806AEE34
void fn_3_6FDA0(void) {
    return;
}

// .text:0x0006FFC4 size:0x2BC mapped:0x806AF058
void fn_3_6FFC4(void) {
    return;
}

// .text:0x00070280 size:0x16C mapped:0x806AF314
void fn_3_70280(void) {
    return;
}

// .text:0x000703EC size:0x294 mapped:0x806AF480
void fn_3_703EC(void) {
    return;
}

// .text:0x00070680 size:0x38 mapped:0x806AF714



/**
 * @address 00070680
 */
s32 fn_3_70680(f32 value) {
    /* * Target: lfs f0, 0x7c(r3) -> g_Pitcher.strikeZoneLeft
     * fcmpo cr0, f1, f0 -> value vs left
     * cror eq, gt, eq   -> implements >=
     * bne .L_FAIL       -> if !(value >= left) return 0
     */
    if (value >= g_Pitcher.strikeZoneLeft) {
        /* * Target: lfs f0, 0x80(r3) -> g_Pitcher.strikeZoneRight
         * fcmpo cr0, f1, f0 -> value vs right
         * cror eq, lt, eq   -> implements <=
         * bne .L_FAIL       -> if !(value <= right) return 0
         */
        if (value <= g_Pitcher.strikeZoneRight) {
            /* li r3, 1 -> blr */
            return 1;
        }
    }
    
    /* .L_FAIL: li r3, 0 -> blr */
    return 0;
}

// .text:0x000706B8 size:0xB0 mapped:0x806AF74C
void fn_3_706B8(void) {
    return;
}

// .text:0x00070768 size:0xD0 mapped:0x806AF7FC
void fn_3_70768(void) {
    return;
}

// .text:0x00070838 size:0x17C mapped:0x806AF8CC
void fn_3_70838(void) {
    return;
}

// .text:0x000709B4 size:0x138 mapped:0x806AFA48
void fn_3_709B4(void) {
    return;
}

// .text:0x00070AEC size:0xA8 mapped:0x806AFB80
void fn_3_70AEC(void) {
    return;
}

// .text:0x00070B94 size:0x360 mapped:0x806AFC28
void fn_3_70B94(void) {
    return;
}

// .text:0x00070EF4 size:0x354 mapped:0x806AFF88
void fn_3_70EF4(void) {
    return;
}

// .text:0x00071248 size:0x1520 mapped:0x806B02DC
void fn_3_71248(void) {
    return;
}

// .text:0x00072768 size:0x540 mapped:0x806B17FC
void fn_3_72768(void) {
    return;
}

// .text:0x00072CA8 size:0x464 mapped:0x806B1D3C
void fn_3_72CA8(void) {
    return;
}

// .text:0x0007310C size:0x49C mapped:0x806B21A0
void fn_3_7310C(void) {
    return;
}

// .text:0x000735A8 size:0x124 mapped:0x806B263C
void fn_3_735A8(void) {
    return;
}

// .text:0x000736CC size:0x4C mapped:0x806B2760

void fn_3_736CC(void) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (g_Strikes.unk_18[i] == -1) {
            g_Strikes.unk_18[i] = 0;
            return;
        }
    }
}

// .text:0x00073718 size:0x14 mapped:0x806B27AC
void fn_3_73718(void) {
    g_Ball.postPitchResultCounter = -1;
}

// .text:0x0007372C size:0x124 mapped:0x806B27C0
void fn_3_7372C(void) {
    return;
}

// .text:0x00073850 size:0x58 mapped:0x806B28E4
void fn_3_73850(void) {
    s32 i;

    g_Strikes.outs++;
    for (i = 0; i < 3; i++) {
        if (g_Strikes.unk_18[i] == -1) {
            g_Strikes.unk_18[i] = 0;
            return;
        }
    }
}

// .text:0x000738A8 size:0x540 mapped:0x806B293C
void fn_3_738A8(void) {
    return;
}

// .text:0x00073DE8 size:0x144 mapped:0x806B2E7C
void fn_3_73DE8(void) {
    return;
}

// .text:0x00073F2C size:0x80 mapped:0x806B2FC0
void fn_3_73F2C(void) {
    if (g_Pitcher.pitchTotalTimeCounter < 0x7FFE) {
        g_Pitcher.pitchTotalTimeCounter++;
    } else {
        g_Pitcher.pitchTotalTimeCounter = 0x7FFF;
    }

    if (g_Ball.pitchHangtimeCounter < 0x7FFE) {
        g_Ball.pitchHangtimeCounter++;
    } else {
        g_Ball.pitchHangtimeCounter = 0x7FFF;
    }

    g_Pitcher.warioWaluStarAnimationStage = 0;
    fn_3_71248();
}

// .text:0x00073FAC size:0x124 mapped:0x806B3040
void fn_3_73FAC(void) {
    return;
}

// .text:0x000740D0 size:0x58 mapped:0x806B3164
void fn_3_740D0(void) {
    g_Pitcher.nonCaptainStarPitchTriggeredType = g_Pitcher.nonCaptainStarPitch;
    switch (g_Pitcher.nonCaptainStarPitch) {
        case 1:
            g_Pitcher.specialPitchTypeCode = 0x10;
            break;
        case 2:
            g_Pitcher.specialPitchTypeCode = 0x11;
            break;
        case 3:
            g_Pitcher.specialPitchTypeCode = 0x12;
            break;
    }
}

// .text:0x00074128 size:0x99C mapped:0x806B31BC
void fn_3_74128(void) {
    return;
}

// .text:0x00074AC4 size:0x248 mapped:0x806B3B58
void fn_3_74AC4(void) {
    return;
}

// .text:0x00074D0C size:0x384 mapped:0x806B3DA0
void fn_3_74D0C(void) {
    return;
}

// .text:0x00075090 size:0x34 mapped:0x806B4124


void fn_3_75090(void) {

    g_Pitcher.pitcherActionState = 1;
    g_Pitcher.currentStateFrameCounter = 0;
    g_Ball.AtBat_Contact_BallPos.x = g_Pitcher.ballCurrentPosition.x;
    g_Ball.AtBat_Contact_BallPos.y = g_Pitcher.ballCurrentPosition.y;
    g_Ball.AtBat_Contact_BallPos.z = g_Pitcher.ballCurrentPosition.z;
}

// .text:0x000750C4 size:0x18 mapped:0x806B4158
void fn_3_750C4(s8 value) {
    g_Pitcher.pitcherActionState = value;
    g_Pitcher.currentStateFrameCounter = 0;
}

// .text:0x000750DC size:0xD8 mapped:0x806B4170
void fn_3_750DC(void) {
    return;
}

// .text:0x000751B4 size:0x234 mapped:0x806B4248
void fn_3_751B4(void) {
    return;
}

// .text:0x000753E8 size:0x4C mapped:0x806B447C

void fn_3_753E8(s32 param_1) {
    // 0x000753EC: cmpwi r3, 0x0
    // The incoming r3 (param_1) is compared to 0.
    
    // 0x000753F8: stb r0, 0x13e(r3)
    g_Pitcher.pitcherActionState = 0;
    
    // 0x000753FC: sth r0, 0x120(r3)
    g_Pitcher.currentStateFrameCounter = 0;

    // 0x00075400: bne .L_0007540C
    // If param_1 != 0, skip these two assignments.
    if (param_1 == 0) {
        // 0x00075404: stb r0, 0x159(r3)
        g_Pitcher.nPitchesThisAB = 0;
        // 0x00075408: stb r0, 0x15a(r3)
        g_Pitcher.nPickoffAttempts = 0;
    }

    // 0x00075410: lfsu f1, lbl_3_data_446C@l(r4)
    // 0x0007541C: lfs f0, 0x4(r4)
    // 0x00075424: stfs f1, 0x8c(r3)
    // 0x00075428: stfs f0, 0x90(r3)
    g_Pitcher.pitcher.x = lbl_3_data_446C.x;
    g_Pitcher.pitcher.z = lbl_3_data_446C.z;

    // 0x0007542C: stb r0, 0x15e(r3)
    g_Pitcher.pitchDeliveryAnimationPlaying = 0;
}

// .text:0x00075434 size:0x84 mapped:0x806B44C8
void fn_3_75434(void) {
    return;
}

// .text:0x000754B8 size:0xA8 mapped:0x806B454C
void fn_3_754B8(void) {
    return;
}

// .text:0x00075560 size:0x45C mapped:0x806B45F4
void fn_3_75560(void) {
    return;
}
