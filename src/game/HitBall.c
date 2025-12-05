#include "game/HitBall.h"
#include "header_rep_data.h"

#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_static.h"
#include "game/rep_1838.h"
#include "game/rep_540.h"
#include "game/rep_1188.h"
#include "game/rep_8C8.h"
#include "musyx/musyx.h"

// probably in data
extern f32 lbl_3_data_4C88[3];

extern sAng lbl_3_data_5A64[5][2][2];

typedef struct {
    s16 powerLower;
    s16 powerUpper;
    s16 addedGravity;
} hitball_power_range;

extern hitball_power_range lbl_3_data_5774[2][5];
extern hitball_power_range lbl_3_data_57B0[4][5];
extern hitball_power_range lbl_3_data_5828[4][5];
extern hitball_power_range lbl_3_data_5864[4][5];
extern hitball_power_range lbl_3_data_58A0[12][5];
extern hitball_power_range lbl_3_data_5A08[3][5];
extern struct {
    /* 0x00 */ f32 chemBobbleMults[2];
    /* 0x08 */ f32 chemMult[3];
    /* 0x14 */ f32 chemBattingMult[3];
} lbl_3_data_4618;
extern s32 lbl_3_data_5B34[][5];
extern f32 lbl_3_data_5E80[4][2];
extern f32 lbl_3_data_5AF0[2];
extern struct {
    f32 _00;
    f32 _04;
    f32 _08;
} lbl_3_data_21438;
extern s16 lbl_3_data_21410[5];
extern s16 lbl_3_data_5488[5][5][2];
extern s16 lbl_3_data_2141C[6][2];
extern f32 lbl_3_data_5AF8[3][5];

extern u8 lbl_3_data_7690[NUM_CHOOSABLE_CHARACTERS][2];
extern s16 lbl_3_data_5CD4[4];
extern s16 buntVerticalAngles[5][2][2][2];

#define FRAME_COUNT_HOLD_FOR_BUNT 8
// acts more like a flag than an actual timer
#define FRAME_COUNT_BUNT_ACTIVE 100
// acts more like a flag than an actual timer
#define FRAME_COUNT_BEGIN_RETREATING 300
#define FRAME_COUNT_BUNT_RETREAT (FRAME_COUNT_BEGIN_RETREATING + 30)
#define FRAME_COUNT_BUNT_LATE_RETREAT (FRAME_COUNT_BEGIN_RETREATING + 12)

// .text:0x00014858 size:0x35C mapped:0x806538ec
void atBat_batter(void) {
    f32* p;
    if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(
            g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID])) {
        g_Batter.aiControlledInd = false;
    }

    if (g_GameLogic.frameCountdownAtBeginningOfAtBatLockout == 0 && g_Batter.beginningOfABAnimationOccuring == 0) {
        BOOL chargeSwung; // = FALSE;
        g_Batter.isBallInHittableZone = chargeSwung = 0;
        g_Batter.predictedPitchXWhenBallReachesBatter =
            g_Pitcher.pitchRelease.x + ((g_Pitcher.ballCurrentPosition.x - g_Pitcher.pitchRelease.x) *
                                        (g_Batter.batPosition.z - g_Pitcher.pitchRelease.z)) /
                                           (g_Pitcher.ballCurrentPosition.z - g_Pitcher.pitchRelease.z);
        if (g_Batter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
            chargeSwung = TRUE;
        }
        if (g_Pitcher.pitchTotalTimeCounter > 0 && g_Pitcher.framesUntilBallReachesBatterZ < 15) {
            if (hittableFrameInd[chargeSwung][g_Pitcher.framesUntilBallReachesBatterZ + 1]) {
                g_Batter.isBallInHittableZone = BALL_HITTABLE_HITTABLE;
            } else if (g_Pitcher.framesUntilBallReachesBatterZ < swingSoundFrame[0][1]) {
                g_Batter.isBallInHittableZone = BALL_HITTABLE_UNHITTABLE;
            }
        }
        if (g_Batter.aiControlledInd) {
            batterAIControlled();
        } else {
            batterHumanControlled();
        }
        batterPreSwing_unused();
        g_Runners[0].position.x = g_Batter.batterPos.x;
        g_Runners[0].position.z = g_Batter.batterPos.z;
    }
}

// .text:0x0001482C size:0x2C mapped:0x806538c0
void initializeInMemBatter(void) {
    g_Batter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    g_Batter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    g_Batter.contactSize_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    g_Batter.hitPower_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    g_Batter.hitPower_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    g_Batter.batterHand = BATTING_HAND_RIGHT;
}

// .text:0x000146C4 size:0x168 mapped:0x80653758
void setBatterContactConstants(void) {
    setInMemBatterConstants(g_GameLogic.battingOrderAndPositionMapping
                                [g_GameLogic.homeTeamBattingInd_fieldingTeam]
                                [g_GameLogic.currentBatterPerTeam[g_GameLogic.homeTeamBattingInd_fieldingTeam]][0]);
    // 1. Get the current batter's character ID first.
    // 2. Declare local pointers to influence register allocation.
    // The order can matter.
    // Assuming BatterHitboxType
    g_Batter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    g_Batter.batPosition2.x = maybeInitialBatPos.x;
    g_Batter.batPosition2.y = maybeInitialBatPos.y;
    g_Batter.batPosition2.z = maybeInitialBatPos.z;
    // 3. Set simple properties

    // possible inline
    {
        BatterReachStruct* hitbox = &BatterHitbox[g_Batter.charID];
        g_Batter._8C = 0;
        g_Batter.swingInd = 0;
        g_Batter._95 = 0;
        g_Batter._9C = 0;
        g_Batter.framesSinceStartOfSwing = 0;
        g_Batter.batterIsBeingCentredInd = 0;
        // 4. Use local pointers for position calculations
        g_Batter.batPosition2.x = maybeInitialBatPos.x;
        g_Batter.batPosition2.y = maybeInitialBatPos.y;
        g_Batter.batPosition2.z = maybeInitialBatPos.z;
        g_Batter.batPosition2.y = hitbox->PitchingHeight * charSizeMultipliers[g_Batter.charID][0];

        // 5. Clamp the position
        if (g_Batter.batPosition2.x < hitbox->HorizontalRangeNear) {
            g_Batter.batPosition2.x = hitbox->HorizontalRangeNear;
        }
        if (g_Batter.batPosition2.x > hitbox->HorizontalRangeFar) {
            g_Batter.batPosition2.x = hitbox->HorizontalRangeFar;
        }
        if (g_Batter.batPosition2.z < hitbox->VerticalRangeFront) {
            g_Batter.batPosition2.z = hitbox->VerticalRangeFront;
        }
        if (g_Batter.batPosition2.z > hitbox->VerticalRangeBack) {
            g_Batter.batPosition2.z = hitbox->VerticalRangeBack;
        }

        // 6. Set final derived positions
        g_Batter.batterPos.x = g_Batter.batPosition2.x + hitbox->batOffsetFromBatterX;
        g_Batter.batterPos.z = g_Batter.batPosition2.z + hitbox->batOffsetFromBatterZ;

        if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
            g_Batter.batterPos.x = -g_Batter.batterPos.x;
        }

        g_GameLogic.frameCountdownAtBeginningOfAtBatLockout = 0;
    }
}

// .text:0x00014454 size:0x270 mapped:0x806534e8
void setDefaultInMemBatter(void) {
    int i;
    g_Batter.frameSwung = -1;
    g_Batter.framesBuntHeld = 0;
    g_Batter.isBunting = false;
    g_Batter.buntStatus = BUNT_STATUS_NONE;
    g_Batter.missedBuntStatus = 0;
    g_Batter.contactMadeInd = 0;
    g_Batter.hitByPitch = 0;
    g_Batter.hitTrajectory = HIT_TRAJECTORY_0;
    g_Batter.displayContactSprite = 0;
    g_Batter._98 = 0;
    g_Batter.missSwingOrBunt = DID_SWING_TYPE_NONE;
    g_Batter._9C = 0;
    g_Batter.chargeUp = 0.0f;
    g_Batter.chargeDown = 0.0f;
    g_Batter.frameFullyCharged = lbl_3_data_7690[g_Batter.charID][0];
    g_Batter.frameChargeDownBegins = lbl_3_data_7690[g_Batter.charID][1];
    g_Batter.chargeFrames = 0;
    g_Batter._76 = 0;
    g_Batter.chargeStatus = CHARGE_SWING_STAGE_NONE;
    g_Batter.countUpUntilChargeEnables = 0;
    g_Batter.isFullyCharged = false;
    g_Batter.didNonCaptainStarSwingConnect = false;
    g_Batter.isStarSwing = false;
    g_Batter.captainStarSwingActivated = CAPTAIN_STAR_TYPE_NONE;
    g_Batter.lightFireballStunID = 0;
    g_Batter.starSwing_starsHaveBeenSpent = 0;
    g_Batter.moonShotInd = false;
    g_Batter.invisibleBallForPeachStarHit = 0;
    g_Batter.inputDirection = STICK_SIDEWAYS_INPUT_NONE;
    g_Batter.hitType = HIT_TYPE_SLAP_NONE;
    g_Batter.isBallInHittableZone = BALL_HITTABLE_NONE;
    g_Batter.swingMissThatWasHittable = 0;
    g_Batter.nonCaptainStarSwingActivated = 0;
    g_Batter.frameDelay[0] = lbl_3_data_5CD4[0];
    g_Batter.frameDelay[1] = lbl_3_data_5CD4[1];

    if (g_Minigame.GameMode_MiniGame == MINI_GAME_ID_BARREL_BATTER) {
        g_Batter.frameDelay[0] = lbl_3_data_5CD4[2];
        g_Batter.frameDelay[1] = lbl_3_data_5CD4[3];
    }

    g_Batter.batPosition.x = g_Batter.batPosition2.x - lbl_3_data_4C88[2];
    g_Batter.batPosition.y = g_Batter.batPosition2.y;
    g_Batter.batPosition.z = g_Batter.batPosition2.z;
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE) {
        g_Batter.aiControlledInd = g_Practice.practiceBatterHandedness;
    } else if (g_d_GameSettings.minigamesEnabled) {
        g_Batter.aiControlledInd = g_Minigame.minigameControlStruct[0].battingHandedness[g_Minigame.rosterID];
    } else {
        g_Batter.aiControlledInd = g_GameLogic.batterHandedness[g_GameLogic.homeTeamBattingInd_fieldingTeam];
    }
    if (g_Batter.aiControlledInd != 0) {
        g_Batter.easyBatting = false;
    }

    for (i = 0; i < 2; i++) {
        int contactSize = g_Batter.contactSize_raw[i];
        int power = g_Batter.hitPower_raw[i];

        if (power > 100) {
            power = 100;
        }
        if (power <= 1) {
            power = 1;
        }
        if (contactSize > 100) {
            contactSize = 100;
        }
        if (contactSize <= 1) {
            contactSize = 1;
        }

        g_Batter.contactSize_capped[i] = contactSize;
        g_Batter.hitPower_capped[i] = power;
    }

    if (g_GameLogic.pre_PostMiniGameInd != 0) {
        g_Batter.swingInd = 0;
        g_Batter.framesSinceStartOfSwing = 0;
    }

    lbl_3_common_bss_34C58._2B = 0;
}

// .text:0x00014388 size:0xCC mapped:0x8065341c
int someAnimationIndFunction(void) {
    int r = g_GameLogic.battingOrderAndPositionMapping
                [g_GameLogic.homeTeamBattingInd_fieldingTeam]
                [g_GameLogic.currentBatterPerTeam[g_GameLogic.homeTeamBattingInd_fieldingTeam]][0];
    int r2;
    int p2 = 9;
    if (g_d_GameSettings.minigamesEnabled) {
        p2 = g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID];
        r2 = inMemRoster[0][p2].stats.CharID;
    } else {
        r2 = inMemRoster[g_GameLogic.teamBatting][r].stats.CharID;
    }
    if (fn_8001C67C_animation(r2, p2) == 0) {
        return 0;
    } else {
        return 1;
    }
}

// .text:0x000142BC size:0xCC mapped:0x80653350
void updateBallHittableZoneStatus(void) {
    int frameCounter;
    BOOL isCharged;
    g_Batter.predictedPitchXWhenBallReachesBatter =
        g_Pitcher.pitchRelease.x + ((g_Pitcher.ballCurrentPosition.x - g_Pitcher.pitchRelease.x) *
                                    (g_Batter.batPosition.z - g_Pitcher.pitchRelease.z)) /
                                       (g_Pitcher.ballCurrentPosition.z - g_Pitcher.pitchRelease.z);
    isCharged = g_Batter.isBallInHittableZone = BALL_HITTABLE_IN_AIR;
    if (g_Batter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        isCharged = true;
    }
    frameCounter = g_Pitcher.pitchTotalTimeCounter;
    if (g_Pitcher.pitchTotalTimeCounter > 0 && g_Pitcher.framesUntilBallReachesBatterZ < 15) {
        if (hittableFrameInd[isCharged][g_Pitcher.framesUntilBallReachesBatterZ + 1]) {
            g_Batter.isBallInHittableZone = BALL_HITTABLE_HITTABLE;
        } else if (g_Pitcher.framesUntilBallReachesBatterZ < swingSoundFrame[0][1]) {
            g_Batter.isBallInHittableZone = BALL_HITTABLE_UNHITTABLE;
        }
    }
}

extern u8 lbl_800E8754[];

// .text:0x00013D94 size:0x528 mapped:0x80652e28
void batterHumanControlled(void) {
    InputStruct* inputs;
    BOOL r27 = FALSE;
    BOOL r26 = FALSE;
    BOOL r25 = FALSE;
    inputs = &g_Controls[g_GameLogic.teams[g_GameLogic.teamBatting]];
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        inputs = &g_Practice.inputs[g_GameLogic.teamBatting];
        if (g_Practice._1C6 != 0 && g_Pitcher.framesUntilBallReachesBatterZ == swingSoundFrame[0][1]) {
            r26 = TRUE;
        }
    } else if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(
                   g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID])) {
        inputs = &g_Minigame._1D7C[g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID]];
    } else if (g_d_GameSettings.minigamesEnabled) {
        inputs = &g_Controls[g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID]];
    }
    if (g_GameLogic.frameCountdownAtBeginningOfAtBatLockout == 0 &&
        (g_d_GameSettings.GameModeSelected != GAME_TYPE_PRACTICE || g_Practice.instructionNumber >= 0 ||
         g_Practice.frames_sinceMovedToFromMenu >= 60)) {
        batterInBoxMovement();
        if (!g_Batter.swingInd && !g_Batter.isBunting && g_Pitcher.strikeZoneProcessNumber == 0 &&
            ((g_Ball.pitchHangtimeCounter <= 0 || g_Batter.countUpUntilChargeEnables != 0) &&
             g_Batter.buntStatus == BUNT_STATUS_NONE)) {

            // I couldn't match this without goto, this was the best I could do
            if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0 &&
                g_Practice._1C6 != 0) {
                if (g_Practice._1C6 == 2) {
                    if (r26) {
                        g_Practice._1C6 = 0;
                        goto _270;
                    } else {
                        goto _228;
                    }
                }
            } else if ((inputs->buttonInput & INPUT_BUTTON_A)) {
            _228:
                g_Batter.countUpUntilChargeEnables++;
                if (g_Batter.countUpUntilChargeEnables >= g_hitShorts.framesUntilChargeIsEnabled) {
                    if (g_Batter.chargeStatus == CHARGE_SWING_STAGE_NONE) {
                        g_Batter.chargeStatus = CHARGE_SWING_STAGE_CHARGEUP;
                    }
                    g_Batter.countUpUntilChargeEnables = g_hitShorts.framesUntilChargeIsEnabled;
                }
            } else {
            _270:
                if (g_Batter.countUpUntilChargeEnables) {
                    r27 = TRUE;
                    g_Batter.countUpUntilChargeEnables = 0;
                } else {
                    g_Batter.chargeStatus = CHARGE_SWING_STAGE_NONE;
                }
            }
        }

        if (!g_Batter.swingInd && g_Pitcher.pitcherActionState != 4 && g_Pitcher.pitcherActionState != 5 &&
            g_Batter._9C == 0) {
            if (g_Batter.buntStatus == BUNT_STATUS_NONE) {
                if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0 &&
                    g_Practice._1C6 != 0 && r26) {
                    r27 = TRUE;
                    if (g_Practice._1C6 == 3) {
                        r25 = TRUE;
                    }
                    g_Practice._1C6 = 0;
                }
                if (inputs->newButtonInput & INPUT_BUTTON_A) {
                    if (g_Batter.chargeStatus == CHARGE_SWING_STAGE_NONE && g_Batter.countUpUntilChargeEnables == 0) {
                        r27 = TRUE;
                    }
                } else if (!(inputs->buttonInput & INPUT_BUTTON_A)) {
                    if (g_Batter.chargeStatus) {
                        r27 = TRUE;
                    }
                }
                if (r27) {
                    g_Batter.swingInd = 1;
                    g_Batter.framesSinceStartOfSwing = 0;
                    g_Batter.isStarSwing = FALSE;
                    if (!g_d_GameSettings.minigamesEnabled) {
                        if (inputs->buttonInput & PAD_TRIGGER_R ||
                            (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE &&
                             g_Practice.instructionNumber >= 0 && r25)) {
                            if (lbl_800E8754[4] || g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE) {
                                g_Batter.isStarSwing = TRUE;
                                g_Batter.chargeUp = 0.0f;
                                g_Batter.chargeDown = 0.0f;
                            }
                        }
                    }
                    if (g_Batter.chargeStatus) {
                        g_Batter.hitGeneralType = BAT_CONTACT_TYPE_CHARGE;
                        if (g_Batter.chargeUp >= 1.0f) {
                            // do nothing
                        } else {
                            sndFXKeyOff(lbl_3_common_bss_34C58._1C);
                        }
                    } else {
                        g_Batter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
                    }
                }
            }

            if (!g_Batter.swingInd && g_Batter.chargeStatus == CHARGE_SWING_STAGE_NONE &&
                g_Batter.buntStatus != BUNT_STATUS_STRIKE && g_Batter.buntStatus != BUNT_STATUS_6) {
                BOOL b = FALSE;
                if (inputs->buttonInput & INPUT_BUTTON_B) {
                    b = TRUE;
                }
                if (b) {
                    g_Batter.isBunting = TRUE;
                    if (g_Batter.buntStatus == BUNT_STATUS_NONE) {
                        g_Batter.buntStatus = BUNT_STATUS_STARTING;
                        g_Batter.framesBuntHeld = 0;
                        g_Batter.chargeStatus = CHARGE_SWING_STAGE_NONE;
                    }
                } else {
                    g_Batter.isBunting = FALSE;
                    g_Batter.hitGeneralType = g_Batter._8C;
                }
            }
        }
    }
    return;
}

// .text:0x000139A0 size:0x3F4 mapped:0x80652a34
void batterInBoxMovement(void) {
}

// UNUSED .text:0x0001379C size:0x204 mapped:0x80652830
void batterPreSwing_unused(void) {
    f32* p = lbl_3_data_4C88;

    if (g_Batter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        if (g_Batter.swingInd) {
            g_Batter.chargeStatus = CHARGE_SWING_STAGE_SWING;
        } else {
            if (g_Batter.chargeStatus == CHARGE_SWING_STAGE_CHARGEUP) {

                g_Batter.chargeFrames++;
                g_Batter.chargeDown = 0.f;
                if (g_Batter.chargeFrames >= g_Batter.frameFullyCharged) {
                    if (g_Batter.chargeFrames <= g_Batter.frameChargeDownBegins) {
                        g_Batter.chargeDown = 1.f;
                    } else {
                        int d1 = (g_Batter.chargeFrames - g_Batter.frameChargeDownBegins);
                        if (d1 > g_hitShorts.frameChargeDownEnds) {
                            g_Batter.chargeDown = 0.f;
                        } else {
                            int d2 = g_hitShorts.frameChargeDownEnds - g_Batter.frameChargeDownBegins;

                            g_Batter.chargeDown = 1.f - (f32)d1 / (f32)(d2);
                            if (g_Batter.chargeDown < 0.f) {
                                g_Batter.chargeDown = 0.f;
                            }
                        }
                    }
                    g_Batter.isFullyCharged = true;
                }

                if (g_Pitcher.pitcherActionState >= 4) {
                    g_Batter.chargeStatus = CHARGE_SWING_STAGE_RELEASE;
                }
            }
            g_Batter.chargeUp = (f32)g_Batter.chargeFrames / (f32)g_Batter.frameFullyCharged;
            if (g_Batter.chargeUp > 1.f) {
                g_Batter.chargeUp = 1.f;
            }
        }
    }
    if (g_Batter.missedBuntStatus == 1) {
        g_Batter.missedBuntStatus = 2;
    }

    g_Batter.batPosition.x = g_Batter.batPosition2.x - p[2];
    g_Batter.batPosition.y = g_Batter.batPosition2.y;
    g_Batter.batPosition.z = g_Batter.batPosition2.z;
    if (g_Batter.buntStatus) {
        ifBunt();
    } else {
        ifSwing();
    }
}

// UNUSED .text:0x00013614 size:0x188 mapped:0x806526a8
static void fn_3_13614(void) {
    // TODO: pitcher enum and define for charging floats
    if (g_Batter.swingInd) {
        g_Batter.chargeStatus = CHARGE_SWING_STAGE_SWING;
    } else {
        if (g_Batter.chargeStatus == CHARGE_SWING_STAGE_CHARGEUP) {
            g_Batter.chargeFrames++;
            g_Batter.chargeDown = 0.f;
            if (g_Batter.chargeFrames >= g_Batter.frameFullyCharged) {
                if (g_Batter.chargeFrames <= g_Batter.frameChargeDownBegins) {
                    g_Batter.chargeDown = 1.f;
                } else {
                    int diff = g_Batter.chargeFrames - g_Batter.frameChargeDownBegins;
                    if (diff > g_hitShorts.frameChargeDownEnds) {
                        g_Batter.chargeDown = 0.f;
                    } else {
                        g_Batter.chargeDown =
                            1.f - (f32)(diff) / (f32)(g_hitShorts.frameChargeDownEnds - g_Batter.frameChargeDownBegins);
                        if (g_Batter.chargeDown < 0.f) {
                            g_Batter.chargeDown = 0.f;
                        }
                    }
                }
                g_Batter.isFullyCharged = true;
            }
            if (g_Pitcher.pitcherActionState >= 4) {
                g_Batter.chargeStatus = CHARGE_SWING_STAGE_RELEASE;
            }
        }

        g_Batter.chargeUp = (f32)g_Batter.chargeFrames / (f32)g_Batter.frameFullyCharged;

        if (g_Batter.chargeUp > 1.f) {
            g_Batter.chargeUp = 1.f;
        }
    }
}
int lbl_3_bss_34;
extern struct {
    u8 _00[0x28];
    u8 _28;
} lbl_80366158;
// .text:0x0001325C size:0x3B8 mapped:0x806522f0
void ifSwing(void) {
    if (g_Batter.swingInd) {
        if (g_Batter.framesSinceStartOfSwing == 0) {
            if (g_Batter.isBallInHittableZone == BALL_HITTABLE_HITTABLE) {
                g_Batter.swingMissThatWasHittable = TRUE;
            } else {
                g_Batter.swingMissThatWasHittable = FALSE;
            }
        }

        if (g_Batter.frameSwung < 0) {
            g_Batter.frameSwung = g_Ball.pitchHangtimeCounter;
        }

        g_Batter.framesSinceStartOfSwing++;
        g_Batter.Stored_Frame_SwingContact_SinceMiss = g_Batter.framesSinceStartOfSwing;
        sndFXKeyOff(lbl_3_common_bss_34C58._1C);
        if (g_Batter.framesSinceStartOfSwing == swingSoundFrame[g_Batter.hitGeneralType][1]) {
            if (g_Batter.hitGeneralType) {
                playSoundEffect(0x169);
            } else {
                playSoundEffect(0x168);
            }
        }

        if (g_Batter.framesSinceStartOfSwing >= g_Batter.frameDelay[g_Batter.hitGeneralType ? 1 : 0] &&
            g_Batter.noSwingAnimationInd) {
            g_Batter.swingInd = 0;
            g_Batter._95 = 0;
            g_Batter.framesSinceStartOfSwing = 0;
            g_Batter.chargeFrames = 0;
            g_Batter.chargeStatus = CHARGE_SWING_STAGE_NONE;
            g_Batter.isFullyCharged = FALSE;
            if (g_Batter.captainStarSwingActivated) {
                sndFXKeyOff(lbl_3_bss_34);
            }
        } else {
            if (g_Batter.contactMadeInd == 0 && g_Batter.framesSinceStartOfSwing < 0xf) {
                calculateIfHitBall();
            }

            if (g_Batter.contactMadeInd) {
                calculateHitVariables();
                if (g_Batter.moonShotInd) {
                    g_GameLogic.PauseSimulationFrameCount = g_hitShorts.moonshotGamePause;
                } else if (g_Batter.captainStarSwingActivated) {
                    g_GameLogic.PauseSimulationFrameCount = g_hitShorts.captainStarHitGamePause;
                } else {
                    g_GameLogic.PauseSimulationFrameCount = g_hitShorts.regularHitGamePause;
                }

                if (g_GameLogic.PauseSimulationFrameCount) {
                    lbl_80366158._28 = TRUE;
                }

                starSwingSpendStars();
                if (g_Minigame.GameMode_MiniGame == SECONDARY_GAME_MODE_TOY_FIELD) {
                    if (g_Ball.bODQualifyingHitInd) {
                        fn_3_6C854(g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID], 2);
                    }
                } else if (g_Minigame.GameMode_MiniGame != SECONDARY_GAME_MODE_BOBOMB_DERBY &&
                           !g_Batter.aiControlledInd && !g_Batter.captainStarSwingActivated && !g_Batter.moonShotInd &&
                           (((g_Batter.isFullyCharged && g_Batter.contactType == HIT_CONTACT_TYPE_PERFECT)) ||
                            (g_Pitcher.starPitchType && !g_Batter.chargeStatus))) {
                    if (g_d_GameSettings.minigamesEnabled) {
                        fn_3_6C854(g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID], 2);
                    } else {
                        fn_3_6C854(g_GameLogic.teamBatting, 2);
                    }
                }
            }

            if (g_Ball.pitchHangtimeCounter > 0) {
                if (g_Batter.framesSinceStartOfSwing >= swingSoundFrame[0][1] && g_Batter._95 == 0) {
                    g_Batter.missSwingOrBunt = DID_SWING_TYPE_SWING;
                    g_Pitcher.strikeInd = 1;
                }
                if (g_Batter.framesSinceStartOfSwing >= swingSoundFrame[0][3]) {
                    starSwingSpendStars();
                }
            }
        }
    }
}

// .text:0x00013008 size:0x254 mapped:0x80651e70
void starSwingSpendStars(void) {
    u8 miniGamesEnabled = g_d_GameSettings.minigamesEnabled;
    if (!miniGamesEnabled && g_Batter.isStarSwing && !g_Batter.starSwing_starsHaveBeenSpent) {
        u8* pStars = &g_GameLogic.TeamStars[g_GameLogic.teamBatting];
        if (*pStars) {
            if (g_d_GameSettings.GameModeSelected != GAME_TYPE_PRACTICE ||
                g_Practice.practiceType_2 == PRACTICE_TYPE_FREEPLAY) {
                if (g_Batter.moonShotInd) {
                    *pStars -= starPowerCosts.moonShotCost;
                } else if (g_GameLogic.Team_CaptainRosterLoc[g_GameLogic.teamBatting] == g_Batter.rosterID &&
                           g_Batter.captainStarSwingActivated) {
                    if (*pStars >= starPowerCosts.captainStarCost) {
                        *pStars -= starPowerCosts.captainStarCost;
                    } else {
                        return;
                    }
                } else if (g_Batter.captainStarSwingActivated) {
                    if (*pStars >= starPowerCosts.nonCaptain_CaptainStarCost) {
                        *pStars -= starPowerCosts.nonCaptain_CaptainStarCost;
                    } else {
                        return;
                    }
                } else if (g_Batter.noncaptainStarSwing) {
                    if (*pStars >= starPowerCosts.regularStarCost) {
                        *pStars -= starPowerCosts.regularStarCost;
                    } else {
                        return;
                    }
                }
            }
            g_Batter.starSwing_starsHaveBeenSpent = 1;
            if (!miniGamesEnabled && g_Batter.aiControlledInd == 0) {
                fn_3_6C854(g_GameLogic.teamBatting, 2);
            }
            if (g_Batter.aiControlledInd) {
                g_AiLogic._67++;
                g_AiLogic.starRelated[g_GameLogic.homeTeamBattingInd_fieldingTeam] = 1;
                g_AiLogic.batterAIABStrat = -1;
            }
            {
                int c = 9;
                VecXYZ a;
                if (g_d_GameSettings.minigamesEnabled) {
                    c = g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID];
                }
                getAnimRelatedCoordinates(c, 26, &a);
                fn_3_C0CE8(g_Batter.captainStarHitPitch, a.x, a.y, a.z);
            }
        }
    }
}

// .text:0x00012DDC size:0x22C mapped:0x80651e70
void ifBunt(void) {
    if (g_Batter.framesBuntHeld < S16_MAX - 1) {
        g_Batter.framesBuntHeld++;
    } else {
        g_Batter.framesBuntHeld = S16_MAX;
    }

    if (g_Batter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        g_Batter.buntStatus = BUNT_STATUS_NONE;
        return;
    }

    if (g_Batter.buntStatus == BUNT_STATUS_STARTING) {
        if (g_Batter.framesBuntHeld >= FRAME_COUNT_HOLD_FOR_BUNT) {
            if ((g_Pitcher.framesUntilUnhittable < 3) && (g_Ball.pitchHangtimeCounter > 0)) {
                g_Batter.buntStatus = BUNT_STATUS_RETREATING;
                g_Batter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            } else {
                g_Batter.buntStatus = BUNT_STATUS_SHOWING;
            }
        } else {
            /* If released bunt within 8 frames if it starting */
            if (!g_Batter.isBunting) {
                g_Batter.buntStatus = BUNT_STATUS_RETREATING;
                g_Batter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            }
        }
        return;
    }

    if (g_Batter.buntStatus == BUNT_STATUS_SHOWING) {
        g_Batter.framesBuntHeld = FRAME_COUNT_BUNT_ACTIVE;
        if (!g_Batter.isBunting) {
            if (g_Ball.pitchHangtimeCounter > 0) {
                if (g_Pitcher.framesUntilUnhittable > 15) {
                    g_Batter.buntStatus = BUNT_STATUS_7;
                    g_Batter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
                } else {
                    g_Batter.buntStatus = BUNT_STATUS_LATE;
                }
            } else {
                g_Batter.buntStatus = BUNT_STATUS_RETREATING;
                g_Batter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            }
        } else {
            calculateIfHitBall();
            if (g_Batter.contactMadeInd) {
                g_Batter.contactMadeInd = true;
                g_Pitcher.strikeInd = true;
                g_Batter.hitGeneralType = g_Batter.buntStatus = BUNT_STATUS_STRIKE;
                calculateHitVariables();
            } else if (g_Batter.batPosition.z > g_Ball.AtBat_Contact_BallPos.z && g_Ball.pitchHangtimeCounter > 1) {
                g_Batter.missedBuntStatus = true;
                g_Batter.buntStatus = BUNT_STATUS_STRIKE;
                g_Pitcher.strikeInd = true;
                g_Batter.missSwingOrBunt = DID_SWING_TYPE_BUNT;
            }
        }
        return;
    }

    if (g_Batter.buntStatus == BUNT_STATUS_RETREATING) {
        if (g_Batter.framesBuntHeld > FRAME_COUNT_BUNT_RETREAT) {
            g_Batter.buntStatus = BUNT_STATUS_NONE;
        }
        return;
    }

    if ((g_Batter.buntStatus == BUNT_STATUS_7) && (g_Batter.framesBuntHeld > FRAME_COUNT_BUNT_LATE_RETREAT)) {
        g_Batter.buntStatus = BUNT_STATUS_NONE;
        return;
    }
}

// .text:0x00012CB4 size:0x128 mapped:0x80651d48
void calculateIfHitBall(void) {
    f32 batterZ;
    f32 ballFuturePosition;
    f32 dX;
    f32 dZ;
    /* If HBP or pitch got to catcher. */
    if (g_Pitcher.pitchDidntResultInLiveBallInd) {
        return;
    }

    if (g_Batter.isBunting) {
        batterZ = g_Batter.batPosition2.z;
    }
    /* If this frame isn't allowed to make contact, then no contact. */
    else if (hittableFrameInd[g_Batter.hitGeneralType != 0][g_Batter.framesSinceStartOfSwing] == false) {
        return;
    } else {
        batterZ = g_Batter.batPosition2.z;
    }

    /* If ball is past hitter, then no contact. */
    if (batterZ >= g_Ball.pastCoordinates[0].z || batterZ < g_Ball.AtBat_Contact_BallPos.z) {
        return;
    }

    /* calculate slope of where ball would intersect bat plane */
    {
        // BUG: maybe incorrect math?
        // it looks like they had 2 XZ points and were trying to interpolate an X based on the Z
        // if that's true, it should be
        // ballFuturePosition = ((ballX - contactX) / (ballZ - contactZ)) * (batterZ - contactZ) + contactX;

        dZ = g_Ball.AtBat_Contact_BallPos.z;
        dZ -= g_Ball.pastCoordinates[2].z;
        dZ = (g_Ball.AtBat_Contact_BallPos.z - batterZ) / dZ;
        dX = g_Ball.AtBat_Contact_BallPos.x - g_Ball.pastCoordinates[2].x;
        dX *= dZ;
        ballFuturePosition = dX + g_Ball.pastCoordinates[2].x;
    }

    if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
        ballFuturePosition = -ballFuturePosition;
    }

    if (ballFuturePosition - g_Batter.batPosition2.x < batContactRange[g_Batter.trimmedBat].near ||
        ballFuturePosition - g_Batter.batPosition2.x > batContactRange[g_Batter.trimmedBat].far) {
        return;
    }

    g_Batter.hitContactPos.x = ballFuturePosition;
    g_Batter.hitContactPos.z = batterZ;
    g_Batter.hitContactPos.y = g_Batter.batPosition.y;
    g_Batter.contactMadeInd = true;
}

// .text:0x00012554 size:0x760 mapped:0x806515e8
void calculateHitVariables(void) {
}

// UNUSED .text:0x000124DC size:0x78 mapped:0x80650f64
static void fn_3_124DC(void) {
    g_Batter.nonCaptainStarSwingActivated = g_Batter.noncaptainStarSwing;
    switch (g_Batter.noncaptainStarSwing) {
        case REGULAR_STAR_SWING_POPFLY:
            g_Batter.chargeUp = BATTER_MAX_CHARGE;
            g_Batter.hitGeneralType = BAT_CONTACT_TYPE_CHARGE;
            g_Batter.didNonCaptainStarSwingConnect = true;
            break;
        case REGULAR_STAR_SWING_GROUNDER:
            g_Batter.chargeUp = BATTER_MAX_CHARGE;
            g_Batter.hitGeneralType = BAT_CONTACT_TYPE_CHARGE;
            g_Batter.didNonCaptainStarSwingConnect = true;
            break;
        case REGULAR_STAR_SWING_LINEDRIVE:
            g_Batter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
            break;
            break;
    }
}

// .text:0x00011ED0 size:0x60C mapped:0x80650f64
void calculateContactAndHitType(void) {
}

// .text:0x00011B9C size:0x334 mapped:0x80650c30
void calculateBallHorizontalAngleHit(void) {
}

// .text:0x00011468 size:0x734 mapped:0x806504fc
void calculateVerticalAngle(void) {
}

// .text:0x00010D00 size:0x768 mapped:0x8064fd94
void calculateHorizontalPower(void) {
    f32 charged = g_Batter.chargeUp;
    hitball_power_range* contactArray = &lbl_3_data_5774[g_Batter.hitGeneralType][g_Batter.contactType];
    f32 lower, upper, calcedDistance, power;
    if (g_Batter.captainStarSwingActivated) {
        contactArray = &lbl_3_data_58A0[g_Batter.captainStarSwingActivated - 1][g_Batter.contactType];
        charged = 0.0f;
    } else if (g_Batter.nonCaptainStarSwingActivated) {
        contactArray = &lbl_3_data_5A08[g_Batter.nonCaptainStarSwingActivated - 1][g_Batter.contactType];
        charged = 0.0f;
    }
    if (g_Batter.didNonCaptainStarSwingConnect) {
        g_Batter.chargeDown = 1.0f;
    }

    if (g_Batter.moonShotInd) {
        contactArray = &lbl_3_data_5774[BAT_CONTACT_TYPE_CHARGE][g_Batter.contactType];
    } else if (g_d_GameSettings.GameModeSelected == GAME_TYPE_TOY_FIELD) {
        contactArray = &lbl_3_data_57B0[g_Batter.hitGeneralType][g_Batter.contactType];
    } else if (g_Minigame.GameMode_MiniGame == MINI_GAME_ID_BOBOMB_DERBY) {
        contactArray = &lbl_3_data_5828[g_Batter.hitGeneralType][g_Batter.contactType];
    } else if (g_Minigame.GameMode_MiniGame == MINI_GAME_ID_BARREL_BATTER) {
        contactArray = &lbl_3_data_5864[g_Batter.hitGeneralType][g_Batter.contactType];
    }

    lower = contactArray->powerLower;
    upper = contactArray->powerUpper;
    calcedDistance = lower + (upper - lower) * g_Batter.contactQuality;

    if (g_Batter.captainStarSwingActivated) {
        power = 100.0f;
    } else if (charged > 0.0f) {
        // issue here
        // lbl_3_rodata_6E8 is 1.0f
        // lbl_3_rodata_710 is int->float
        f32 v;
        f32 v2;
        f32 v3;
        f32 power1;
        f32 power2;

        power = g_Batter.hitPower_capped[BAT_CONTACT_TYPE_CHARGE];
        v = (g_Batter.hitPower_capped[BAT_CONTACT_TYPE_CHARGE] - g_Batter.hitPower_capped[BAT_CONTACT_TYPE_SLAP]);
        power1 = v * g_hitFloats.overChargeLerpBtwnSlapAndChargePower;
        power1 = power1 * (1.0f - g_Batter.chargeDown);
        power -= power1;
    } else {
        power = (f32)g_Batter.hitPower_capped[BAT_CONTACT_TYPE_SLAP];
    }

    if (g_Batter.captainStarSwingActivated == CAPTAIN_STAR_TYPE_NONE &&
        g_Batter.nonCaptainStarSwingActivated == REGULAR_STAR_SWING_NONE) {
        int contactQuality = 2;
        if (g_Batter.contactType == HIT_CONTACT_TYPE_RIGHT_NICE || g_Batter.contactType == HIT_CONTACT_TYPE_LEFT_NICE) {
            contactQuality = 1;
        } else if (g_Batter.contactType == HIT_CONTACT_TYPE_PERFECT) {
            contactQuality = 0;
        }
        power *= LinearInterpolateToNewRange(g_Pitcher.calced_cursedBall, 0.0f, 100.0f,
                                             lbl_3_data_5E80[contactQuality][0], lbl_3_data_5E80[contactQuality][1]);
    }

    if (g_Batter.chemLinksOnBase != 0 && charged > 0.0f) {
        power *= lbl_3_data_4618.chemBattingMult[g_Batter.chemLinksOnBase - 1];
    }

    if (g_Batter.hitType >= 0) {
        power = power * lbl_3_data_5B34[g_Batter.hitType][1 - g_Batter.easyBatting] / 100.0f;
    }

    power = calcedDistance * (power / 100.0f * (lbl_3_data_5AF0[1] - lbl_3_data_5AF0[0]) + lbl_3_data_5AF0[0]);

    if (g_Minigame.GameMode_MiniGame == MINI_GAME_ID_BOBOMB_DERBY) {
        if (g_Batter.contactType == HIT_CONTACT_TYPE_PERFECT && g_Batter.isFullyCharged) {
            g_Ball.Hit_VerticalAngle = lbl_3_data_5488[g_Batter.contactType][4][0] +
                                       g_Ball.StaticRandomInt1 % (lbl_3_data_5488[g_Batter.contactType][4][1] -
                                                                  lbl_3_data_5488[g_Batter.contactType][4][0]);

            g_Ball.bODQualifyingHitInd = 1;
            g_Minigame.bODAngleIndexBasedOnHitPower = 0;
            power *= lbl_3_data_21438._08;

            for (; g_Minigame.bODAngleIndexBasedOnHitPower < 5; g_Minigame.bODAngleIndexBasedOnHitPower++) {
                if (power < lbl_3_data_21410[g_Minigame.bODAngleIndexBasedOnHitPower]) {
                    break;
                }
            }
            g_Ball.Hit_VerticalAngle =
                RandomInt_Game_Range(lbl_3_data_2141C[g_Minigame.bODAngleIndexBasedOnHitPower + 1][0],
                                     lbl_3_data_2141C[g_Minigame.bODAngleIndexBasedOnHitPower + 1][1]);
            g_UnkSound_32718._08 = 2;
        } else {
            g_Ball.Hit_VerticalAngle = RandomInt_Game_Range(lbl_3_data_2141C[0][0], lbl_3_data_2141C[0][1]);
            power *= 0.7f;
        }
    }

    g_Ball.hittingAddedGravityFactor = contactArray->addedGravity * 1e-5f;

    if (g_Batter.captainStarSwingActivated == CAPTAIN_STAR_TYPE_NONE) {
        int ballAngle1 = g_Ball.Hit_HorizontalAngle;
        int ballAngle;
        int zone;
        if (ballAngle1 < 0x200) {
            ballAngle = 0;
        } else if (ballAngle1 > 0x600) {
            ballAngle = 0x400;
        } else {
            ballAngle = ballAngle1 - 0x200;
        }

        if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
            ballAngle = 0x400 - ballAngle;
        }

        if (ballAngle < 0x100) {
            zone = 0;
        } else if (ballAngle < 0x200) {
            zone = 1;
            ballAngle -= 0x100;
        } else if (ballAngle < 0x300) {
            zone = 2;
            ballAngle -= 0x200;
        } else {
            zone = 3;
            ballAngle -= 0x300;
        }
        power *= LinearInterpolateToNewRange(ballAngle / (f32)0x100, 0.0f, 1.0f,
                                             lbl_3_data_5AF8[g_Batter.trajectoryPushPull][zone],
                                             lbl_3_data_5AF8[g_Batter.trajectoryPushPull][zone + 1]);
    }

    if (g_Batter.moonShotInd) {
        power *= g_hitFloats.moonshotMult;
    }

    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        g_Ball.Hit_HorizontalPower = g_Practice.practice_hitHorizontalPower;
    } else if (g_d_GameSettings.GameModeSelected != GAME_TYPE_PRACTICE ||
               g_Practice.practiceType_2 != PRACTICE_TYPE_FIELDING) {
        g_Ball.Hit_HorizontalPower = power;
    }
}

// UNUSED .text:0x00010A80 size:0x280 mapped:0x8064fb14
void calculateBuntHorizontalAngle(void) {
    int pullInd = 0;
    InputStruct* inputs;
    int horizAngle;
    int lowerBunt;
    int upperBunt;
    u8 contact;

    inputs = &g_Controls[g_GameLogic.teams[g_GameLogic.teamBatting]];
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        inputs = &g_Practice.inputs[g_GameLogic.teamBatting];
    } else if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(
                   g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID])) {
        inputs = &g_Minigame._1D7C[g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID]];
    } else if (g_d_GameSettings.minigamesEnabled) {
        inputs = &g_Controls[g_Minigame.minigameControlStruct[0].characterIndex[g_Minigame.rosterID]];
    }

    contact = g_Batter.contactType;

    upperBunt = lbl_3_data_5A64[contact][0][1] + ((g_Batter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] *
                                                   (lbl_3_data_5A64[contact][1][1] - lbl_3_data_5A64[contact][0][1])) /
                                                  100);

    lowerBunt = lbl_3_data_5A64[contact][0][0] + ((g_Batter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] *
                                                   (lbl_3_data_5A64[contact][1][0] - lbl_3_data_5A64[contact][0][0])) /
                                                  100);

    horizAngle = lowerBunt + (g_Ball.StaticRandomInt1 % (upperBunt - lowerBunt));

    if (contact >= 1 && contact <= 3) {
        if (inputs->buttonInput & INPUT_BUTTON_LEFT) {
            if (g_Batter.batterHand == BATTING_HAND_RIGHT) {
                pullInd = 1;
            }
        } else if (inputs->buttonInput & INPUT_BUTTON_RIGHT) {
            if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
                pullInd = 1;
            }
        } else {
            pullInd = g_Ball.StaticRandomInt2 % 2;
        }
    } else if (contact == 0) {
        pullInd = 1;
    }

    if ((g_Batter.batterHand == BATTING_HAND_RIGHT && pullInd != 0) ||
        (g_Batter.batterHand != BATTING_HAND_RIGHT && pullInd == 0)) {
        if (horizAngle <= 0x800) {
            horizAngle = 0x800 - horizAngle;
        } else {
            horizAngle = 0x1800 - horizAngle;
        }
    }

    g_Ball.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(horizAngle);
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        g_Ball.Hit_HorizontalAngle = g_Practice.practice_hitHorizontalAngle;
    }
}

// UNUSED .text:0x000108F8 size:0x188 mapped:0x8064f98c
static void calculateBuntVerticalAngle_unused(void) {
    // TODO this needs better variable names, probably better function name too
    int rng, ang0Low, ang0High, ang1Low, ang1High, contactLow, contactHigh, contact;
    rng = 0;
    if (g_Ball.StaticRandomInt1 % 2) {
        rng = 1;
    }

    contact = g_Batter.contactType;
    ang0Low = buntVerticalAngles[contact][0][rng][0];
    ang0High = buntVerticalAngles[contact][0][rng][1];
    ang1Low = buntVerticalAngles[contact][1][rng][0];
    ang1High = buntVerticalAngles[contact][1][rng][1];

    contactLow = ang0Low + ((g_Batter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] * (ang1Low - ang0Low)) / 100);
    contactHigh = ang0High + ((g_Batter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] * (ang1High - ang0High)) / 100);

    g_Ball.Hit_VerticalAngle = contactLow + (g_Ball.StaticRandomInt1 % (contactHigh - contactLow));

    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        g_Ball.Hit_VerticalAngle = g_Practice.practice_hitVerticalAngle;
    }

    if (g_Ball.Hit_VerticalAngle > SANG_ANG_90) {
        g_Ball.Hit_VerticalAngle = SANG_ANG_180 - g_Ball.Hit_VerticalAngle;
        g_Ball.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(SANG_ANG_180 + g_Ball.Hit_HorizontalAngle);
    } else if (g_Ball.Hit_VerticalAngle < -SANG_ANG_90) {
        g_Ball.Hit_VerticalAngle = SANG_ANG_360 + g_Ball.Hit_VerticalAngle;
        g_Ball.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(SANG_ANG_180 + g_Ball.Hit_HorizontalAngle);
    } else if (g_Ball.Hit_VerticalAngle < 0) {
        g_Ball.Hit_VerticalAngle = SANG_ANG_360 + g_Ball.Hit_VerticalAngle;
    }
}

// .text:0x0001087C size:0x7C mapped:0x8064f910
void calculateBuntHorizontalPower(void) {
    s32 min_power = buntPower[g_Batter.contactType][0];
    s32 max_power = buntPower[g_Batter.contactType][1];

    s32 diff = max_power - min_power;

    s32 horizontal_power = g_Ball.StaticRandomInt1 % diff;
    horizontal_power += min_power;
    g_Ball.Hit_HorizontalPower = horizontal_power;

    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_PRACTICE && g_Practice.instructionNumber >= 0) {
        g_Ball.Hit_HorizontalPower = g_Practice.practice_hitHorizontalPower;
    }
}

// .text:0x00010470 size:0x40C mapped:0x8064f504
void calculateBallVelocityAcceleration(void) {
    f32 s_verticalAngle, power, s_horizontalAngle, c_horizontalAngle, c_verticalAngle, verticalAngle, horizontalAngle,
        tmp_horizontalAngle, tmp_verticalAngle;

    power = g_Ball.Hit_HorizontalPower / 2.f;
    tmp_horizontalAngle = shortAngleToRad_Capped(g_Ball.Hit_HorizontalAngle);
    tmp_verticalAngle = shortAngleToRad_Capped(g_Ball.Hit_VerticalAngle);
    s_verticalAngle = SINF(tmp_verticalAngle);
    c_verticalAngle = COSF(tmp_verticalAngle) * power;
    s_horizontalAngle = COSF(tmp_horizontalAngle);
    c_horizontalAngle = SINF(tmp_horizontalAngle);

    verticalAngle = s_horizontalAngle * c_verticalAngle;
    horizontalAngle = c_horizontalAngle * c_verticalAngle;

    g_Ball.physicsSubstruct.velocity.x = (verticalAngle) / 100.f;
    g_Ball.physicsSubstruct.velocity.y = (power * s_verticalAngle) / 100.f;
    g_Ball.physicsSubstruct.velocity.z = (horizontalAngle) / 100.f;

    g_Ball.physicsSubstruct.acceleration.x = 0.f;
    g_Ball.physicsSubstruct.acceleration.y = g_Ball.hittingAddedGravityFactor;
    g_Ball.physicsSubstruct.acceleration.z = 0.f;

    if (!g_Batter.isBunting && (g_Ball.Hit_HorizontalAngle <= 0x900 || g_Ball.Hit_HorizontalAngle >= 0xf00) &&
        g_Ball.currentStarSwing != CAPTAIN_STAR_TYPE_DK && g_Ball.currentStarSwing != CAPTAIN_STAR_TYPE_DIDDY) {

        f32 contactQual, v1, v2;
        int hit_vert, hit_hor;

        BOOL hasSuperCurve =
            checkFieldingStat(g_GameLogic.teamBatting, g_Batter.rosterID, FIELDING_ABILITY_SUPER_CURVE);
        if (g_Batter.nonCaptainStarSwingActivated == REGULAR_STAR_SWING_LINEDRIVE) {
            hasSuperCurve = true;
        }

        power = 1.f;
        contactQual = g_Batter.contactQualityAbsolute;
        if (contactQual > 100.f) {
            contactQual = 200.f - contactQual;
        }
        contactQual *= 0.01f;

        contactQual = (1.0f - contactQual) * s_ballCurveData[hasSuperCurve].x;
        power -= contactQual;

        hit_vert = g_Ball.Hit_VerticalAngle;
        if (hit_vert > 0x180 && hit_vert <= 0x400) {
            int t = hit_vert - 0x180;
            contactQual = t;
            if (t > (f32)0x200) {
                contactQual = (f32)0x200;
            }

            contactQual *= (2.f / (f32)SANG_ANG_90); // 45 degrees
            power *= (1.f - contactQual);
        }

        hit_hor = g_Ball.Hit_HorizontalAngle;

        if (hit_hor > 0xc00 || hit_hor < 0x100) {
            hit_hor = 0x100;
        } else if (hit_hor > 0x700) {
            hit_hor = 0x700;
        }

        if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
            hit_hor = 0x800 - hit_hor;
        }

        if (hit_hor < 0x460) {
            contactQual = (0x460 - hit_hor) / 864.f;
        } else {
            contactQual = (0x460 - hit_hor) / 672.f;
        }

        power *= contactQual;

        if (power < 0.f) {
            verticalAngle = -c_horizontalAngle;
            horizontalAngle = s_horizontalAngle;
            power = -power;
        } else if (contactQual > 0.f) {
            verticalAngle = c_horizontalAngle;
            horizontalAngle = -s_horizontalAngle;
        }

        g_Ball.physicsSubstruct.acceleration.x = verticalAngle * power * s_ballCurveData[hasSuperCurve].y;
        g_Ball.physicsSubstruct.acceleration.z = horizontalAngle * power * s_ballCurveData[hasSuperCurve].z;

        if (g_Ball.physicsSubstruct.acceleration.z > 0.f) {
            g_Ball.physicsSubstruct.acceleration.z = -g_Ball.physicsSubstruct.acceleration.z;
        }

        if (g_Batter.batterHand != BATTING_HAND_RIGHT) {
            g_Ball.physicsSubstruct.acceleration.x = -g_Ball.physicsSubstruct.acceleration.x;
        }
    }
    estimateAndSetFutureCoords(2);
}

// UNUSED .text:0x000102C8 size:0x1A8 mapped:0x8064f35c
void starHitSetting_unused(void) {
    E(u8, CAPTAIN_STAR_TYPE) starType = (s8)g_Batter.captainStarSwingActivated;
    g_Ball.inAirOrBefore2ndBounceOrLowBallEnergy = false;
    g_Ball.currentStarSwing2 = g_Ball.currentStarSwing = starType;
    if (starType == CAPTAIN_STAR_TYPE_DK || starType == CAPTAIN_STAR_TYPE_DIDDY) {
        g_Ball.matchFramesAndBallAngle.bananaHitStartFrame =
            g_Ball.hangtimeOfHit * g_hitFloats.DKStarHangtimePercentStart;
        g_Ball.matchFramesAndBallAngle.bananaHitEndFrame = g_Ball.hangtimeOfHit * g_hitFloats.DKStarHangtimePercentEnd;
        g_Ball.directionOfBananaHit = g_Batter.batterHand;
    }

    else if (starType == CAPTAIN_STAR_TYPE_WARIO || starType == CAPTAIN_STAR_TYPE_WALUIGI) {
        g_Ball.warioWaluStarHitDirection = RandomInt_Game(2);
        g_Ball.unused_garlicHitRelated.x = 0.f;
        g_Ball.matchFramesAndBallAngle.garlicHitFramesUntilHitGroundForSplit =
            g_hitShorts.framesBeforeGroundWhenGarlicSplits;
    }

    else if (starType == CAPTAIN_STAR_TYPE_BOWSER || starType == CAPTAIN_STAR_TYPE_BOWSERJR) {
        g_Ball.inAirOrBefore2ndBounceOrLowBallEnergy = true;
        g_Ball.hardHitIndicator = true;
        g_Ball.ballEnergy = g_Ball.Hit_HorizontalPower * g_hitFloats.bulletStarEnergyMultiplier;

    }

    else if (starType == CAPTAIN_STAR_TYPE_PEACH || starType == CAPTAIN_STAR_TYPE_DAISY) {
        g_Ball.autoFielderAvoidDropSpotForPeachesStarHit = true;
    }
}
