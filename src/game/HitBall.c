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
            minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID])) {
        inMemBatter.aiControlledInd = false;
    }

    if (gameControls.frameCountdownAtBeginningOfAtBatLockout == 0 && inMemBatter.beginningOfABAnimationOccuring == 0) {
        BOOL chargeSwung; // = FALSE;
        inMemBatter.isBallInHittableZone = chargeSwung = 0;
        inMemBatter.predictedPitchXWhenBallReachesBatter =
            inMemPitcher.pitchRelease.x + ((inMemPitcher.ballCurrentPosition.x - inMemPitcher.pitchRelease.x) *
                                           (inMemBatter.batPosition.z - inMemPitcher.pitchRelease.z)) /
                                              (inMemPitcher.ballCurrentPosition.z - inMemPitcher.pitchRelease.z);
        if (inMemBatter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
            chargeSwung = TRUE;
        }
        if (inMemPitcher.pitchTotalTimeCounter > 0 && inMemPitcher.framesUntilBallReachesBatterZ < 15) {
            if (hittableFrameInd[chargeSwung][inMemPitcher.framesUntilBallReachesBatterZ + 1]) {
                inMemBatter.isBallInHittableZone = BALL_HITTABLE_HITTABLE;
            } else if (inMemPitcher.framesUntilBallReachesBatterZ < swingSoundFrame[0][1]) {
                inMemBatter.isBallInHittableZone = BALL_HITTABLE_UNHITTABLE;
            }
        }
        if (inMemBatter.aiControlledInd) {
            batterAIControlled();
        } else {
            batterHumanControlled();
        }
        batterPreSwing_unused();
        inMemRunners[0].position.x = inMemBatter.batterPos.x;
        inMemRunners[0].position.z = inMemBatter.batterPos.z;
    }
}

// .text:0x0001482C size:0x2C mapped:0x806538c0
void initializeInMemBatter(void) {
    inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    inMemBatter.hitPower_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    inMemBatter.hitPower_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    inMemBatter.batterHand = BATTING_HAND_RIGHT;
}

// .text:0x000146C4 size:0x168 mapped:0x80653758
void setBatterContactConstants(void) {
    setInMemBatterConstants(gameControls.battingOrderAndPositionMapping
                                [gameControls.homeTeamBattingInd_fieldingTeam]
                                [gameControls.currentBatterPerTeam[gameControls.homeTeamBattingInd_fieldingTeam]][0]);
    // 1. Get the current batter's character ID first.
    // 2. Declare local pointers to influence register allocation.
    // The order can matter.
    // Assuming BatterHitboxType
    inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    inMemBatter.batPosition2.x = maybeInitialBatPos.x;
    inMemBatter.batPosition2.y = maybeInitialBatPos.y;
    inMemBatter.batPosition2.z = maybeInitialBatPos.z;
    // 3. Set simple properties

    // possible inline
    {
        BatterReachStruct* hitbox = &BatterHitbox[inMemBatter.charID];
        inMemBatter._8C = 0;
        inMemBatter.swingInd = 0;
        inMemBatter._95 = 0;
        inMemBatter._9C = 0;
        inMemBatter.framesSinceStartOfSwing = 0;
        inMemBatter.batterIsBeingCentredInd = 0;
        // 4. Use local pointers for position calculations
        inMemBatter.batPosition2.x = maybeInitialBatPos.x;
        inMemBatter.batPosition2.y = maybeInitialBatPos.y;
        inMemBatter.batPosition2.z = maybeInitialBatPos.z;
        inMemBatter.batPosition2.y = hitbox->PitchingHeight * charSizeMultipliers[inMemBatter.charID][0];

        // 5. Clamp the position
        if (inMemBatter.batPosition2.x < hitbox->HorizontalRangeNear) {
            inMemBatter.batPosition2.x = hitbox->HorizontalRangeNear;
        }
        if (inMemBatter.batPosition2.x > hitbox->HorizontalRangeFar) {
            inMemBatter.batPosition2.x = hitbox->HorizontalRangeFar;
        }
        if (inMemBatter.batPosition2.z < hitbox->VerticalRangeFront) {
            inMemBatter.batPosition2.z = hitbox->VerticalRangeFront;
        }
        if (inMemBatter.batPosition2.z > hitbox->VerticalRangeBack) {
            inMemBatter.batPosition2.z = hitbox->VerticalRangeBack;
        }

        // 6. Set final derived positions
        inMemBatter.batterPos.x = inMemBatter.batPosition2.x + hitbox->batOffsetFromBatterX;
        inMemBatter.batterPos.z = inMemBatter.batPosition2.z + hitbox->batOffsetFromBatterZ;

        if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
            inMemBatter.batterPos.x = -inMemBatter.batterPos.x;
        }

        gameControls.frameCountdownAtBeginningOfAtBatLockout = 0;
    }
}

// .text:0x00014454 size:0x270 mapped:0x806534e8
void setDefaultInMemBatter(void) {
    int i;
    inMemBatter.frameSwung = -1;
    inMemBatter.framesBuntHeld = 0;
    inMemBatter.isBunting = false;
    inMemBatter.buntStatus = BUNT_STATUS_NONE;
    inMemBatter.missedBuntStatus = 0;
    inMemBatter.contactMadeInd = 0;
    inMemBatter.hitByPitch = 0;
    inMemBatter.hitTrajectory = HIT_TRAJECTORY_0;
    inMemBatter.displayContactSprite = 0;
    inMemBatter._98 = 0;
    inMemBatter.missSwingOrBunt = DID_SWING_TYPE_NONE;
    inMemBatter._9C = 0;
    inMemBatter.chargeUp = 0.0f;
    inMemBatter.chargeDown = 0.0f;
    inMemBatter.frameFullyCharged = lbl_3_data_7690[inMemBatter.charID][0];
    inMemBatter.frameChargeDownBegins = lbl_3_data_7690[inMemBatter.charID][1];
    inMemBatter.chargeFrames = 0;
    inMemBatter._76 = 0;
    inMemBatter.chargeStatus = CHARGE_SWING_STAGE_NONE;
    inMemBatter.countUpUntilChargeEnables = 0;
    inMemBatter.isFullyCharged = false;
    inMemBatter.didNonCaptainStarSwingConnect = false;
    inMemBatter.isStarSwing = false;
    inMemBatter.captainStarSwingActivated = CAPTAIN_STAR_TYPE_NONE;
    inMemBatter.lightFireballStunID = 0;
    inMemBatter.starSwing_starsHaveBeenSpent = 0;
    inMemBatter.moonShotInd = false;
    inMemBatter.invisibleBallForPeachStarHit = 0;
    inMemBatter.inputDirection = STICK_SIDEWAYS_INPUT_NONE;
    inMemBatter.hitType = HIT_TYPE_SLAP_NONE;
    inMemBatter.isBallInHittableZone = BALL_HITTABLE_NONE;
    inMemBatter.swingMissThatWasHittable = 0;
    inMemBatter.nonCaptainStarSwingActivated = 0;
    inMemBatter.frameDelay[0] = lbl_3_data_5CD4[0];
    inMemBatter.frameDelay[1] = lbl_3_data_5CD4[1];

    if (minigameStruct.GameMode_MiniGame == MINI_GAME_ID_BARREL_BATTER) {
        inMemBatter.frameDelay[0] = lbl_3_data_5CD4[2];
        inMemBatter.frameDelay[1] = lbl_3_data_5CD4[3];
    }

    inMemBatter.batPosition.x = inMemBatter.batPosition2.x - lbl_3_data_4C88[2];
    inMemBatter.batPosition.y = inMemBatter.batPosition2.y;
    inMemBatter.batPosition.z = inMemBatter.batPosition2.z;
    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE) {
        inMemBatter.aiControlledInd = practiceStruct.practiceBatterHandedness;
    } else if (gameInitVariables.minigamesEnabled) {
        inMemBatter.aiControlledInd =
            minigameStruct.minigameControlStruct[0].battingHandedness[minigameStruct.rosterID];
    } else {
        inMemBatter.aiControlledInd = gameControls.batterHandedness[gameControls.homeTeamBattingInd_fieldingTeam];
    }
    if (inMemBatter.aiControlledInd != 0) {
        inMemBatter.easyBatting = false;
    }

    for (i = 0; i < 2; i++) {
        int contactSize = inMemBatter.contactSize_raw[i];
        int power = inMemBatter.hitPower_raw[i];

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

        inMemBatter.contactSize_capped[i] = contactSize;
        inMemBatter.hitPower_capped[i] = power;
    }

    if (gameControls.pre_PostMiniGameInd != 0) {
        inMemBatter.swingInd = 0;
        inMemBatter.framesSinceStartOfSwing = 0;
    }

    lbl_3_common_bss_34C58._2B = 0;
}

// .text:0x00014388 size:0xCC mapped:0x8065341c
int someAnimationIndFunction(void) {
    int r = gameControls.battingOrderAndPositionMapping
                [gameControls.homeTeamBattingInd_fieldingTeam]
                [gameControls.currentBatterPerTeam[gameControls.homeTeamBattingInd_fieldingTeam]][0];
    int r2;
    int p2 = 9;
    if (gameInitVariables.minigamesEnabled) {
        p2 = minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID];
        r2 = inMemRoster[0][p2].stats.CharID;
    } else {
        r2 = inMemRoster[gameControls.teamBatting][r].stats.CharID;
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
    inMemBatter.predictedPitchXWhenBallReachesBatter =
        inMemPitcher.pitchRelease.x + ((inMemPitcher.ballCurrentPosition.x - inMemPitcher.pitchRelease.x) *
                                       (inMemBatter.batPosition.z - inMemPitcher.pitchRelease.z)) /
                                          (inMemPitcher.ballCurrentPosition.z - inMemPitcher.pitchRelease.z);
    isCharged = inMemBatter.isBallInHittableZone = BALL_HITTABLE_IN_AIR;
    if (inMemBatter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        isCharged = true;
    }
    frameCounter = inMemPitcher.pitchTotalTimeCounter;
    if (inMemPitcher.pitchTotalTimeCounter > 0 && inMemPitcher.framesUntilBallReachesBatterZ < 15) {
        if (hittableFrameInd[isCharged][inMemPitcher.framesUntilBallReachesBatterZ + 1]) {
            inMemBatter.isBallInHittableZone = BALL_HITTABLE_HITTABLE;
        } else if (inMemPitcher.framesUntilBallReachesBatterZ < swingSoundFrame[0][1]) {
            inMemBatter.isBallInHittableZone = BALL_HITTABLE_UNHITTABLE;
        }
    }
}

// .text:0x00013D94 size:0x528 mapped:0x80652e28
void batterHumanControlled(void) {
}

// .text:0x000139A0 size:0x3F4 mapped:0x80652a34
void batterInBoxMovement(void) {
}

// UNUSED .text:0x0001379C size:0x204 mapped:0x80652830
void batterPreSwing_unused(void) {
    f32* p = lbl_3_data_4C88;

    if (inMemBatter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        if (inMemBatter.swingInd) {
            inMemBatter.chargeStatus = CHARGE_SWING_STAGE_SWING;
        } else {
            if (inMemBatter.chargeStatus == CHARGE_SWING_STAGE_CHARGEUP) {

                inMemBatter.chargeFrames++;
                inMemBatter.chargeDown = 0.f;
                if (inMemBatter.chargeFrames >= inMemBatter.frameFullyCharged) {
                    if (inMemBatter.chargeFrames <= inMemBatter.frameChargeDownBegins) {
                        inMemBatter.chargeDown = 1.f;
                    } else {
                        int d1 = (inMemBatter.chargeFrames - inMemBatter.frameChargeDownBegins);
                        if (d1 > g_hitShorts.frameChargeDownEnds) {
                            inMemBatter.chargeDown = 0.f;
                        } else {
                            int d2 = g_hitShorts.frameChargeDownEnds - inMemBatter.frameChargeDownBegins;

                            inMemBatter.chargeDown = 1.f - (f32)d1 / (f32)(d2);
                            if (inMemBatter.chargeDown < 0.f) {
                                inMemBatter.chargeDown = 0.f;
                            }
                        }
                    }
                    inMemBatter.isFullyCharged = true;
                }

                if (inMemPitcher.pitcherActionState >= 4) {
                    inMemBatter.chargeStatus = CHARGE_SWING_STAGE_RELEASE;
                }
            }
            inMemBatter.chargeUp = (f32)inMemBatter.chargeFrames / (f32)inMemBatter.frameFullyCharged;
            if (inMemBatter.chargeUp > 1.f) {
                inMemBatter.chargeUp = 1.f;
            }
        }
    }
    if (inMemBatter.missedBuntStatus == 1) {
        inMemBatter.missedBuntStatus = 2;
    }

    inMemBatter.batPosition.x = inMemBatter.batPosition2.x - p[2];
    inMemBatter.batPosition.y = inMemBatter.batPosition2.y;
    inMemBatter.batPosition.z = inMemBatter.batPosition2.z;
    if (inMemBatter.buntStatus) {
        ifBunt();
    } else {
        ifSwing();
    }
}

// UNUSED .text:0x00013614 size:0x188 mapped:0x806526a8
static void fn_3_13614(void) {
    // TODO: pitcher enum and define for charging floats
    if (inMemBatter.swingInd) {
        inMemBatter.chargeStatus = CHARGE_SWING_STAGE_SWING;
    } else {
        if (inMemBatter.chargeStatus == CHARGE_SWING_STAGE_CHARGEUP) {
            inMemBatter.chargeFrames++;
            inMemBatter.chargeDown = 0.f;
            if (inMemBatter.chargeFrames >= inMemBatter.frameFullyCharged) {
                if (inMemBatter.chargeFrames <= inMemBatter.frameChargeDownBegins) {
                    inMemBatter.chargeDown = 1.f;
                } else {
                    int diff = inMemBatter.chargeFrames - inMemBatter.frameChargeDownBegins;
                    if (diff > g_hitShorts.frameChargeDownEnds) {
                        inMemBatter.chargeDown = 0.f;
                    } else {
                        inMemBatter.chargeDown = 1.f - (f32)(diff) / (f32)(g_hitShorts.frameChargeDownEnds -
                                                                           inMemBatter.frameChargeDownBegins);
                        if (inMemBatter.chargeDown < 0.f) {
                            inMemBatter.chargeDown = 0.f;
                        }
                    }
                }
                inMemBatter.isFullyCharged = true;
            }
            if (inMemPitcher.pitcherActionState >= 4) {
                inMemBatter.chargeStatus = CHARGE_SWING_STAGE_RELEASE;
            }
        }

        inMemBatter.chargeUp = (f32)inMemBatter.chargeFrames / (f32)inMemBatter.frameFullyCharged;

        if (inMemBatter.chargeUp > 1.f) {
            inMemBatter.chargeUp = 1.f;
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
    if (inMemBatter.swingInd) {
        if (inMemBatter.framesSinceStartOfSwing == 0) {
            if (inMemBatter.isBallInHittableZone == BALL_HITTABLE_HITTABLE) {
                inMemBatter.swingMissThatWasHittable = TRUE;
            } else {
                inMemBatter.swingMissThatWasHittable = FALSE;
            }
        }

        if (inMemBatter.frameSwung < 0) {
            inMemBatter.frameSwung = inMemBall.pitchHangtimeCounter;
        }

        inMemBatter.framesSinceStartOfSwing++;
        inMemBatter.Stored_Frame_SwingContact_SinceMiss = inMemBatter.framesSinceStartOfSwing;
        sndFXKeyOff(lbl_3_common_bss_34C58._1C);
        if (inMemBatter.framesSinceStartOfSwing == swingSoundFrame[inMemBatter.hitGeneralType][1]) {
            if (inMemBatter.hitGeneralType) {
                playSoundEffect(0x169);
            } else {
                playSoundEffect(0x168);
            }
        }

        if (inMemBatter.framesSinceStartOfSwing >= inMemBatter.frameDelay[inMemBatter.hitGeneralType ? 1 : 0] &&
            inMemBatter.noSwingAnimationInd) {
            inMemBatter.swingInd = 0;
            inMemBatter._95 = 0;
            inMemBatter.framesSinceStartOfSwing = 0;
            inMemBatter.chargeFrames = 0;
            inMemBatter.chargeStatus = CHARGE_SWING_STAGE_NONE;
            inMemBatter.isFullyCharged = FALSE;
            if (inMemBatter.captainStarSwingActivated) {
                sndFXKeyOff(lbl_3_bss_34);
            }
        } else {
            if (inMemBatter.contactMadeInd == 0 && inMemBatter.framesSinceStartOfSwing < 0xf) {
                calculateIfHitBall();
            }

            if (inMemBatter.contactMadeInd) {
                calculateHitVariables();
                if (inMemBatter.moonShotInd) {
                    gameControls.PauseSimulationFrameCount = g_hitShorts.moonshotGamePause;
                } else if (inMemBatter.captainStarSwingActivated) {
                    gameControls.PauseSimulationFrameCount = g_hitShorts.captainStarHitGamePause;
                } else {
                    gameControls.PauseSimulationFrameCount = g_hitShorts.regularHitGamePause;
                }

                if (gameControls.PauseSimulationFrameCount) {
                    lbl_80366158._28 = TRUE;
                }

                starSwingSpendStars();
                if (minigameStruct.GameMode_MiniGame == SECONDARY_GAME_MODE_TOY_FIELD) {
                    if (inMemBall.bODQualifyingHitInd) {
                        fn_3_6C854(minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID], 2);
                    }
                } else if (minigameStruct.GameMode_MiniGame != SECONDARY_GAME_MODE_BOBOMB_DERBY &&
                           !inMemBatter.aiControlledInd && !inMemBatter.captainStarSwingActivated &&
                           !inMemBatter.moonShotInd &&
                           (((inMemBatter.isFullyCharged && inMemBatter.contactType == HIT_CONTACT_TYPE_PERFECT)) ||
                            (inMemPitcher.starPitchType && !inMemBatter.chargeStatus))) {
                    if (gameInitVariables.minigamesEnabled) {
                        fn_3_6C854(minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID], 2);
                    } else {
                        fn_3_6C854(gameControls.teamBatting, 2);
                    }
                }
            }

            if (inMemBall.pitchHangtimeCounter > 0) {
                if (inMemBatter.framesSinceStartOfSwing >= swingSoundFrame[0][1] && inMemBatter._95 == 0) {
                    inMemBatter.missSwingOrBunt = DID_SWING_TYPE_SWING;
                    inMemPitcher.strikeInd = 1;
                }
                if (inMemBatter.framesSinceStartOfSwing >= swingSoundFrame[0][3]) {
                    starSwingSpendStars();
                }
            }
        }
    }
}

// .text:0x00013008 size:0x254 mapped:0x80651e70
void starSwingSpendStars(void) {
    u8 miniGamesEnabled = gameInitVariables.minigamesEnabled;
    if (!miniGamesEnabled && inMemBatter.isStarSwing && !inMemBatter.starSwing_starsHaveBeenSpent) {
        u8* pStars = &gameControls.TeamStars[gameControls.teamBatting];
        if (*pStars) {
            if (gameInitVariables.GameModeSelected != GAME_TYPE_PRACTICE ||
                practiceStruct.practiceType_2 == PRACTICE_TYPE_FREEPLAY) {
                if (inMemBatter.moonShotInd) {
                    *pStars -= starPowerCosts.moonShotCost;
                } else if (gameControls.Team_CaptainRosterLoc[gameControls.teamBatting] == inMemBatter.rosterID &&
                           inMemBatter.captainStarSwingActivated) {
                    if (*pStars >= starPowerCosts.captainStarCost) {
                        *pStars -= starPowerCosts.captainStarCost;
                    } else {
                        return;
                    }
                } else if (inMemBatter.captainStarSwingActivated) {
                    if (*pStars >= starPowerCosts.nonCaptain_CaptainStarCost) {
                        *pStars -= starPowerCosts.nonCaptain_CaptainStarCost;
                    } else {
                        return;
                    }
                } else if (inMemBatter.noncaptainStarSwing) {
                    if (*pStars >= starPowerCosts.regularStarCost) {
                        *pStars -= starPowerCosts.regularStarCost;
                    } else {
                        return;
                    }
                }
            }
            inMemBatter.starSwing_starsHaveBeenSpent = 1;
            if (!miniGamesEnabled && inMemBatter.aiControlledInd == 0) {
                fn_3_6C854(gameControls.teamBatting, 2);
            }
            if (inMemBatter.aiControlledInd) {
                aiStruct._67++;
                aiStruct.starRelated[gameControls.homeTeamBattingInd_fieldingTeam] = 1;
                aiStruct.batterAIABStrat = -1;
            }
            {
                int c = 9;
                VecXYZ a;
                if (gameInitVariables.minigamesEnabled) {
                    c = minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID];
                }
                getAnimRelatedCoordinates(c, 26, &a);
                fn_3_C0CE8(inMemBatter.captainStarHitPitch, a.x, a.y, a.z);
            }
        }
    }
}

// .text:0x00012DDC size:0x22C mapped:0x80651e70
void ifBunt(void) {
    if (inMemBatter.framesBuntHeld < S16_MAX - 1) {
        inMemBatter.framesBuntHeld++;
    } else {
        inMemBatter.framesBuntHeld = S16_MAX;
    }

    if (inMemBatter.chargeStatus != CHARGE_SWING_STAGE_NONE) {
        inMemBatter.buntStatus = BUNT_STATUS_NONE;
        return;
    }

    if (inMemBatter.buntStatus == BUNT_STATUS_STARTING) {
        if (inMemBatter.framesBuntHeld >= FRAME_COUNT_HOLD_FOR_BUNT) {
            if ((inMemPitcher.framesUntilUnhittable < 3) && (inMemBall.pitchHangtimeCounter > 0)) {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            } else {
                inMemBatter.buntStatus = BUNT_STATUS_SHOWING;
            }
        } else {
            /* If released bunt within 8 frames if it starting */
            if (!inMemBatter.isBunting) {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            }
        }
        return;
    }

    if (inMemBatter.buntStatus == BUNT_STATUS_SHOWING) {
        inMemBatter.framesBuntHeld = FRAME_COUNT_BUNT_ACTIVE;
        if (!inMemBatter.isBunting) {
            if (inMemBall.pitchHangtimeCounter > 0) {
                if (inMemPitcher.framesUntilUnhittable > 15) {
                    inMemBatter.buntStatus = BUNT_STATUS_7;
                    inMemBatter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
                } else {
                    inMemBatter.buntStatus = BUNT_STATUS_LATE;
                }
            } else {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = FRAME_COUNT_BEGIN_RETREATING;
            }
        } else {
            calculateIfHitBall();
            if (inMemBatter.contactMadeInd) {
                inMemBatter.contactMadeInd = true;
                inMemPitcher.strikeInd = true;
                inMemBatter.hitGeneralType = inMemBatter.buntStatus = BUNT_STATUS_STRIKE;
                calculateHitVariables();
            } else if (inMemBatter.batPosition.z > inMemBall.AtBat_Contact_BallPos.z &&
                       inMemBall.pitchHangtimeCounter > 1) {
                inMemBatter.missedBuntStatus = true;
                inMemBatter.buntStatus = BUNT_STATUS_STRIKE;
                inMemPitcher.strikeInd = true;
                inMemBatter.missSwingOrBunt = DID_SWING_TYPE_BUNT;
            }
        }
        return;
    }

    if (inMemBatter.buntStatus == BUNT_STATUS_RETREATING) {
        if (inMemBatter.framesBuntHeld > FRAME_COUNT_BUNT_RETREAT) {
            inMemBatter.buntStatus = BUNT_STATUS_NONE;
        }
        return;
    }

    if ((inMemBatter.buntStatus == BUNT_STATUS_7) && (inMemBatter.framesBuntHeld > FRAME_COUNT_BUNT_LATE_RETREAT)) {
        inMemBatter.buntStatus = BUNT_STATUS_NONE;
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
    if (inMemPitcher.pitchDidntResultInLiveBallInd) {
        return;
    }

    if (inMemBatter.isBunting) {
        batterZ = inMemBatter.batPosition2.z;
    }
    /* If this frame isn't allowed to make contact, then no contact. */
    else if (hittableFrameInd[inMemBatter.hitGeneralType != 0][inMemBatter.framesSinceStartOfSwing] == false) {
        return;
    } else {
        batterZ = inMemBatter.batPosition2.z;
    }

    /* If ball is past hitter, then no contact. */
    if (batterZ >= inMemBall.pastCoordinates[0].z || batterZ < inMemBall.AtBat_Contact_BallPos.z) {
        return;
    }

    /* calculate slope of where ball would intersect bat plane */
    {
        // BUG: maybe incorrect math?
        // it looks like they had 2 XZ points and were trying to interpolate an X based on the Z
        // if that's true, it should be
        // ballFuturePosition = ((ballX - contactX) / (ballZ - contactZ)) * (batterZ - contactZ) + contactX;

        dZ = inMemBall.AtBat_Contact_BallPos.z;
        dZ -= inMemBall.pastCoordinates[2].z;
        dZ = (inMemBall.AtBat_Contact_BallPos.z - batterZ) / dZ;
        dX = inMemBall.AtBat_Contact_BallPos.x - inMemBall.pastCoordinates[2].x;
        dX *= dZ;
        ballFuturePosition = dX + inMemBall.pastCoordinates[2].x;
    }

    if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
        ballFuturePosition = -ballFuturePosition;
    }

    if (ballFuturePosition - inMemBatter.batPosition2.x < batContactRange[inMemBatter.trimmedBat].near ||
        ballFuturePosition - inMemBatter.batPosition2.x > batContactRange[inMemBatter.trimmedBat].far) {
        return;
    }

    inMemBatter.hitContactPos.x = ballFuturePosition;
    inMemBatter.hitContactPos.z = batterZ;
    inMemBatter.hitContactPos.y = inMemBatter.batPosition.y;
    inMemBatter.contactMadeInd = true;
}

// .text:0x00012554 size:0x760 mapped:0x806515e8
void calculateHitVariables(void) {
}

// UNUSED .text:0x000124DC size:0x78 mapped:0x80650f64
static void fn_3_124DC(void) {
    inMemBatter.nonCaptainStarSwingActivated = inMemBatter.noncaptainStarSwing;
    switch (inMemBatter.noncaptainStarSwing) {
        case REGULAR_STAR_SWING_POPFLY:
            inMemBatter.chargeUp = BATTER_MAX_CHARGE;
            inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_CHARGE;
            inMemBatter.didNonCaptainStarSwingConnect = true;
            break;
        case REGULAR_STAR_SWING_GROUNDER:
            inMemBatter.chargeUp = BATTER_MAX_CHARGE;
            inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_CHARGE;
            inMemBatter.didNonCaptainStarSwingConnect = true;
            break;
        case REGULAR_STAR_SWING_LINEDRIVE:
            inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
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
    f32 charged = inMemBatter.chargeUp;
    hitball_power_range* contactArray = &lbl_3_data_5774[inMemBatter.hitGeneralType][inMemBatter.contactType];
    f32 lower, upper, calcedDistance, power;
    if (inMemBatter.captainStarSwingActivated) {
        contactArray = &lbl_3_data_58A0[inMemBatter.captainStarSwingActivated - 1][inMemBatter.contactType];
        charged = 0.0f;
    } else if (inMemBatter.nonCaptainStarSwingActivated) {
        contactArray = &lbl_3_data_5A08[inMemBatter.nonCaptainStarSwingActivated - 1][inMemBatter.contactType];
        charged = 0.0f;
    }
    if (inMemBatter.didNonCaptainStarSwingConnect) {
        inMemBatter.chargeDown = 1.0f;
    }

    if (inMemBatter.moonShotInd) {
        contactArray = &lbl_3_data_5774[BAT_CONTACT_TYPE_CHARGE][inMemBatter.contactType];
    } else if (gameInitVariables.GameModeSelected == GAME_TYPE_TOY_FIELD) {
        contactArray = &lbl_3_data_57B0[inMemBatter.hitGeneralType][inMemBatter.contactType];
    } else if (minigameStruct.GameMode_MiniGame == MINI_GAME_ID_BOBOMB_DERBY) {
        contactArray = &lbl_3_data_5828[inMemBatter.hitGeneralType][inMemBatter.contactType];
    } else if (minigameStruct.GameMode_MiniGame == MINI_GAME_ID_BARREL_BATTER) {
        contactArray = &lbl_3_data_5864[inMemBatter.hitGeneralType][inMemBatter.contactType];
    }

    lower = contactArray->powerLower;
    upper = contactArray->powerUpper;
    calcedDistance = lower + (upper - lower) * inMemBatter.contactQuality;

    if (inMemBatter.captainStarSwingActivated) {
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

        power = inMemBatter.hitPower_capped[BAT_CONTACT_TYPE_CHARGE];
        v = (inMemBatter.hitPower_capped[BAT_CONTACT_TYPE_CHARGE] - inMemBatter.hitPower_capped[BAT_CONTACT_TYPE_SLAP]);
        power1 = v * g_hitFloats.overChargeLerpBtwnSlapAndChargePower;
        power1 = power1 * (1.0f - inMemBatter.chargeDown);
        power -= power1;
    } else {
        power = (f32)inMemBatter.hitPower_capped[BAT_CONTACT_TYPE_SLAP];
    }

    if (inMemBatter.captainStarSwingActivated == CAPTAIN_STAR_TYPE_NONE &&
        inMemBatter.nonCaptainStarSwingActivated == REGULAR_STAR_SWING_NONE) {
        int contactQuality = 2;
        if (inMemBatter.contactType == HIT_CONTACT_TYPE_RIGHT_NICE ||
            inMemBatter.contactType == HIT_CONTACT_TYPE_LEFT_NICE) {
            contactQuality = 1;
        } else if (inMemBatter.contactType == HIT_CONTACT_TYPE_PERFECT) {
            contactQuality = 0;
        }
        power *= LinearInterpolateToNewRange(inMemPitcher.calced_cursedBall, 0.0f, 100.0f,
                                             lbl_3_data_5E80[contactQuality][0], lbl_3_data_5E80[contactQuality][1]);
    }

    if (inMemBatter.chemLinksOnBase != 0 && charged > 0.0f) {
        power *= lbl_3_data_4618.chemBattingMult[inMemBatter.chemLinksOnBase - 1];
    }

    if (inMemBatter.hitType >= 0) {
        power = power * lbl_3_data_5B34[inMemBatter.hitType][1 - inMemBatter.easyBatting] / 100.0f;
    }
    
    power = calcedDistance * (power / 100.0f * (lbl_3_data_5AF0[1] - lbl_3_data_5AF0[0]) + lbl_3_data_5AF0[0]);
    
    if (minigameStruct.GameMode_MiniGame == MINI_GAME_ID_BOBOMB_DERBY) {
        if (inMemBatter.contactType == HIT_CONTACT_TYPE_PERFECT && inMemBatter.isFullyCharged) {
            inMemBall.Hit_VerticalAngle = lbl_3_data_5488[inMemBatter.contactType][4][0] + inMemBall.StaticRandomInt1 % (lbl_3_data_5488[inMemBatter.contactType][4][1] - lbl_3_data_5488[inMemBatter.contactType][4][0]);
            
            inMemBall.bODQualifyingHitInd = 1;
            minigameStruct.bODAngleIndexBasedOnHitPower = 0;
            power *= lbl_3_data_21438._08;

            for (; minigameStruct.bODAngleIndexBasedOnHitPower < 5; minigameStruct.bODAngleIndexBasedOnHitPower++) {
                if (power < lbl_3_data_21410[minigameStruct.bODAngleIndexBasedOnHitPower]) {
                    break;
                }
            }
            inMemBall.Hit_VerticalAngle =
                fn_3_9ED1C_randBetween(lbl_3_data_2141C[minigameStruct.bODAngleIndexBasedOnHitPower + 1][0],
                           lbl_3_data_2141C[minigameStruct.bODAngleIndexBasedOnHitPower + 1][1]);
            lbl_3_common_bss_32718._08 = 2;
        } else {
            inMemBall.Hit_VerticalAngle = fn_3_9ED1C_randBetween(lbl_3_data_2141C[0][0], lbl_3_data_2141C[0][1]);
            power *= 0.7f;
        }
    }

    inMemBall.hittingAddedGravityFactor  = contactArray->addedGravity * 1e-5f;

    if (inMemBatter.captainStarSwingActivated == CAPTAIN_STAR_TYPE_NONE) {
        int ballAngle1 = inMemBall.Hit_HorizontalAngle;
        int ballAngle;
        int zone;
        if (ballAngle1 < 0x200) {
            ballAngle = 0;
        } else if (ballAngle1 > 0x600) {
            ballAngle = 0x400;
        } else {
            ballAngle = ballAngle1 - 0x200;
        }
        
        if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
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
                                             lbl_3_data_5AF8[inMemBatter.trajectoryPushPull][zone],
                                             lbl_3_data_5AF8[inMemBatter.trajectoryPushPull][zone + 1]);
    }

    if (inMemBatter.moonShotInd) {
        power *= g_hitFloats.moonshotMult;
    }

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_HorizontalPower = practiceStruct.practice_hitHorizontalPower;
    } else if (gameInitVariables.GameModeSelected != GAME_TYPE_PRACTICE ||
               practiceStruct.practiceType_2 != PRACTICE_TYPE_FIELDING) {
        inMemBall.Hit_HorizontalPower = power;
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

    inputs = &inMemControls[gameControls.teams[gameControls.teamBatting]];
    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inputs = &practiceStruct.inputs[gameControls.teamBatting];
    } else if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(
                   minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID])) {
        inputs = &minigameStruct._1D7C[minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID]];
    } else if (gameInitVariables.minigamesEnabled) {
        inputs = &inMemControls[minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.rosterID]];
    }

    contact = inMemBatter.contactType;

    upperBunt = lbl_3_data_5A64[contact][0][1] + ((inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] *
                                                   (lbl_3_data_5A64[contact][1][1] - lbl_3_data_5A64[contact][0][1])) /
                                                  100);

    lowerBunt = lbl_3_data_5A64[contact][0][0] + ((inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] *
                                                   (lbl_3_data_5A64[contact][1][0] - lbl_3_data_5A64[contact][0][0])) /
                                                  100);

    horizAngle = lowerBunt + (inMemBall.StaticRandomInt1 % (upperBunt - lowerBunt));

    if (contact >= 1 && contact <= 3) {
        if (inputs->buttonInput & INPUT_BUTTON_LEFT) {
            if (inMemBatter.batterHand == BATTING_HAND_RIGHT) {
                pullInd = 1;
            }
        } else if (inputs->buttonInput & INPUT_BUTTON_RIGHT) {
            if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
                pullInd = 1;
            }
        } else {
            pullInd = inMemBall.StaticRandomInt2 % 2;
        }
    } else if (contact == 0) {
        pullInd = 1;
    }

    if ((inMemBatter.batterHand == BATTING_HAND_RIGHT && pullInd != 0) ||
        (inMemBatter.batterHand != BATTING_HAND_RIGHT && pullInd == 0)) {
        if (horizAngle <= 0x800) {
            horizAngle = 0x800 - horizAngle;
        } else {
            horizAngle = 0x1800 - horizAngle;
        }
    }

    inMemBall.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(horizAngle);
    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_HorizontalAngle = practiceStruct.practice_hitHorizontalAngle;
    }
}

// UNUSED .text:0x000108F8 size:0x188 mapped:0x8064f98c
static void calculateBuntVerticalAngle_unused(void) {
    // TODO this needs better variable names, probably better function name too
    int rng, ang0Low, ang0High, ang1Low, ang1High, contactLow, contactHigh, contact;
    rng = 0;
    if (inMemBall.StaticRandomInt1 % 2) {
        rng = 1;
    }

    contact = inMemBatter.contactType;
    ang0Low = buntVerticalAngles[contact][0][rng][0];
    ang0High = buntVerticalAngles[contact][0][rng][1];
    ang1Low = buntVerticalAngles[contact][1][rng][0];
    ang1High = buntVerticalAngles[contact][1][rng][1];

    contactLow = ang0Low + ((inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] * (ang1Low - ang0Low)) / 100);
    contactHigh = ang0High + ((inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] * (ang1High - ang0High)) / 100);

    inMemBall.Hit_VerticalAngle = contactLow + (inMemBall.StaticRandomInt1 % (contactHigh - contactLow));

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_VerticalAngle = practiceStruct.practice_hitVerticalAngle;
    }

    if (inMemBall.Hit_VerticalAngle > SANG_ANG_90) {
        inMemBall.Hit_VerticalAngle = SANG_ANG_180 - inMemBall.Hit_VerticalAngle;
        inMemBall.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(SANG_ANG_180 + inMemBall.Hit_HorizontalAngle);
    } else if (inMemBall.Hit_VerticalAngle < -SANG_ANG_90) {
        inMemBall.Hit_VerticalAngle = SANG_ANG_360 + inMemBall.Hit_VerticalAngle;
        inMemBall.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(SANG_ANG_180 + inMemBall.Hit_HorizontalAngle);
    } else if (inMemBall.Hit_VerticalAngle < 0) {
        inMemBall.Hit_VerticalAngle = SANG_ANG_360 + inMemBall.Hit_VerticalAngle;
    }
}

// .text:0x0001087C size:0x7C mapped:0x8064f910
void calculateBuntHorizontalPower(void) {
    s32 min_power = buntPower[inMemBatter.contactType][0];
    s32 max_power = buntPower[inMemBatter.contactType][1];

    s32 diff = max_power - min_power;

    s32 horizontal_power = inMemBall.StaticRandomInt1 % diff;
    horizontal_power += min_power;
    inMemBall.Hit_HorizontalPower = horizontal_power;

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_HorizontalPower = practiceStruct.practice_hitHorizontalPower;
    }
}

// .text:0x00010470 size:0x40C mapped:0x8064f504
void calculateBallVelocityAcceleration(void) {
    f32 s_verticalAngle, power, s_horizontalAngle, c_horizontalAngle, c_verticalAngle, verticalAngle, horizontalAngle,
        tmp_horizontalAngle, tmp_verticalAngle;

    power = inMemBall.Hit_HorizontalPower / 2.f;
    tmp_horizontalAngle = shortAngleToRad_Capped(inMemBall.Hit_HorizontalAngle);
    tmp_verticalAngle = shortAngleToRad_Capped(inMemBall.Hit_VerticalAngle);
    s_verticalAngle = SINF(tmp_verticalAngle);
    c_verticalAngle = COSF(tmp_verticalAngle) * power;
    s_horizontalAngle = COSF(tmp_horizontalAngle);
    c_horizontalAngle = SINF(tmp_horizontalAngle);

    verticalAngle = s_horizontalAngle * c_verticalAngle;
    horizontalAngle = c_horizontalAngle * c_verticalAngle;

    inMemBall.physicsSubstruct.velocity.x = (verticalAngle) / 100.f;
    inMemBall.physicsSubstruct.velocity.y = (power * s_verticalAngle) / 100.f;
    inMemBall.physicsSubstruct.velocity.z = (horizontalAngle) / 100.f;

    inMemBall.physicsSubstruct.acceleration.x = 0.f;
    inMemBall.physicsSubstruct.acceleration.y = inMemBall.hittingAddedGravityFactor;
    inMemBall.physicsSubstruct.acceleration.z = 0.f;

    if (!inMemBatter.isBunting && (inMemBall.Hit_HorizontalAngle <= 0x900 || inMemBall.Hit_HorizontalAngle >= 0xf00) &&
        inMemBall.currentStarSwing != CAPTAIN_STAR_TYPE_DK && inMemBall.currentStarSwing != CAPTAIN_STAR_TYPE_DIDDY) {

        f32 contactQual, v1, v2;
        int hit_vert, hit_hor;

        BOOL hasSuperCurve =
            checkFieldingStat(gameControls.teamBatting, inMemBatter.rosterID, FIELDING_ABILITY_SUPER_CURVE);
        if (inMemBatter.nonCaptainStarSwingActivated == REGULAR_STAR_SWING_LINEDRIVE) {
            hasSuperCurve = true;
        }

        power = 1.f;
        contactQual = inMemBatter.contactQualityAbsolute;
        if (contactQual > 100.f) {
            contactQual = 200.f - contactQual;
        }
        contactQual *= 0.01f;

        contactQual = (1.0f - contactQual) * s_ballCurveData[hasSuperCurve].x;
        power -= contactQual;

        hit_vert = inMemBall.Hit_VerticalAngle;
        if (hit_vert > 0x180 && hit_vert <= 0x400) {
            int t = hit_vert - 0x180;
            contactQual = t;
            if (t > (f32)0x200) {
                contactQual = (f32)0x200;
            }

            contactQual *= (2.f / (f32)SANG_ANG_90); // 45 degrees
            power *= (1.f - contactQual);
        }

        hit_hor = inMemBall.Hit_HorizontalAngle;

        if (hit_hor > 0xc00 || hit_hor < 0x100) {
            hit_hor = 0x100;
        } else if (hit_hor > 0x700) {
            hit_hor = 0x700;
        }

        if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
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

        inMemBall.physicsSubstruct.acceleration.x = verticalAngle * power * s_ballCurveData[hasSuperCurve].y;
        inMemBall.physicsSubstruct.acceleration.z = horizontalAngle * power * s_ballCurveData[hasSuperCurve].z;

        if (inMemBall.physicsSubstruct.acceleration.z > 0.f) {
            inMemBall.physicsSubstruct.acceleration.z = -inMemBall.physicsSubstruct.acceleration.z;
        }

        if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
            inMemBall.physicsSubstruct.acceleration.x = -inMemBall.physicsSubstruct.acceleration.x;
        }
    }
    estimateAndSetFutureCoords(2);
}

// UNUSED .text:0x000102C8 size:0x1A8 mapped:0x8064f35c
void starHitSetting_unused(void) {
    E(u8, CAPTAIN_STAR_TYPE) starType = (s8)inMemBatter.captainStarSwingActivated;
    inMemBall.inAirOrBefore2ndBounceOrLowBallEnergy = false;
    inMemBall.currentStarSwing2 = inMemBall.currentStarSwing = starType;
    if (starType == CAPTAIN_STAR_TYPE_DK || starType == CAPTAIN_STAR_TYPE_DIDDY) {
        inMemBall.matchFramesAndBallAngle.bananaHitStartFrame =
            inMemBall.hangtimeOfHit * g_hitFloats.DKStarHangtimePercentStart;
        inMemBall.matchFramesAndBallAngle.bananaHitEndFrame =
            inMemBall.hangtimeOfHit * g_hitFloats.DKStarHangtimePercentEnd;
        inMemBall.directionOfBananaHit = inMemBatter.batterHand;
    }

    else if (starType == CAPTAIN_STAR_TYPE_WARIO || starType == CAPTAIN_STAR_TYPE_WALUIGI) {
        inMemBall.warioWaluStarHitDirection = RandomInt_Game(2);
        inMemBall.unused_garlicHitRelated.x = 0.f;
        inMemBall.matchFramesAndBallAngle.garlicHitFramesUntilHitGroundForSplit =
            g_hitShorts.framesBeforeGroundWhenGarlicSplits;
    }

    else if (starType == CAPTAIN_STAR_TYPE_BOWSER || starType == CAPTAIN_STAR_TYPE_BOWSERJR) {
        inMemBall.inAirOrBefore2ndBounceOrLowBallEnergy = true;
        inMemBall.hardHitIndicator = true;
        inMemBall.ballEnergy = inMemBall.Hit_HorizontalPower * g_hitFloats.bulletStarEnergyMultiplier;

    }

    else if (starType == CAPTAIN_STAR_TYPE_PEACH || starType == CAPTAIN_STAR_TYPE_DAISY) {
        inMemBall.autoFielderAvoidDropSpotForPeachesStarHit = true;
    }
}
