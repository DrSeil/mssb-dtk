#include "game/HitBall.h"
#include "header_rep_data.h"

#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_static.h"
#include "game/rep_1838.h"
#include "game/rep_540.h"
#include "game/rep_1188.h"
#include "game/rep_8C8.h"

// probably in data
extern f32 lbl_3_data_4C88[3];


#define FRAME_COUNT_HOLD_FOR_BUNT 8
// acts more like a flag than an actual timer
#define FRAME_COUNT_BUNT_ACTIVE 100
// acts more like a flag than an actual timer
#define FRAME_COUNT_BEGIN_RETREATING 300
#define FRAME_COUNT_BUNT_RETREAT (FRAME_COUNT_BEGIN_RETREATING + 30)
#define FRAME_COUNT_BUNT_LATE_RETREAT (FRAME_COUNT_BEGIN_RETREATING + 12)

// UNUSED .text:0x000102C8 size:0x1A8 mapped:0x8064f35c
static void starHitSetting_unused(void) {
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

extern s16 buntVerticalAngles[5][2][2][2];
// UNUSED .text:0x000108F8 size:0x188 mapped:0x8064f98c
static void calculateBuntVerticalAngle_unused(void) {
    // TODO this needs better variable names, probably better function name too
    int rng, ang0Low, ang0High, ang1Low, ang1High, contactLow, contactHigh, contact;
    rng = 0;
    if (inMemBall.StaticRandomInt1 % 2) {
        rng = 1;
    }

    contact = inMemBatter.contactType;
    ang0Low = buntVerticalAngles [contact][0][rng][0];
    ang0High = buntVerticalAngles[contact][0][rng][1];
    ang1Low =  buntVerticalAngles[contact][1][rng][0];
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

// UNUSED .text:0x00010A80 size:0x280 mapped:0x8064fb14
void calculateBuntHorizontalAngle(void) {
    int r29;
    int s;
    InputStruct* inputs = &lbl_3_common_bss_32848[gameControls.teams[gameControls.teamBatting]];
    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inputs = &practiceStruct.inputs[gameControls.teamBatting];
    } else if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(0)) {
    }

    if ((inMemBatter.batterHand == BATTING_HAND_RIGHT && r29 != 0) ||
        (inMemBatter.batterHand != BATTING_HAND_RIGHT && r29 == 0)) {
        if (s <= 0x800) {
            s = 0x800 - s;
        }else{
            s = 0x1800 - s;
        }
    }
    inMemBall.Hit_HorizontalAngle = fn_3_9FE6C_normalizeAngle(s);
    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_HorizontalAngle = practiceStruct.practice_hitHorizontalAngle;
    }
}

// .text:0x00010D00 size:0x768 mapped:0x8064fd94
void calculateHorizontalPower(void) {
}

// .text:0x00011468 size:0x734 mapped:0x806504fc
void calculateVerticalAngle(void) {
}

// .text:0x00011B9C size:0x334 mapped:0x80650c30
void calculateBallHorizontalAngleHit(void) {
}

// .text:0x00011ED0 size:0x60C mapped:0x80650f64
void calculateContactAndHitType(void) {
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

// .text:0x00012554 size:0x760 mapped:0x806515e8
void calculateHitVariables(void) {
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

// .text:0x00013008 size:0x254 mapped:0x80651e70
void starSwingSpendStars(void) {
}

// .text:0x0001325C size:0x3B8 mapped:0x806522f0
void ifSwing(void) {
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

// UNUSED .text:0x0001379C size:0x204 mapped:0x80652830
static void batterPreSwing_unused(void) {
}

// .text:0x000139A0 size:0x3F4 mapped:0x80652a34
void batterInBoxMovement(void) {
}

// .text:0x00013D94 size:0x528 mapped:0x80652e28
void batterHumanControlled(void) {
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

// .text:0x00014388 size:0xCC mapped:0x8065341c
int someAnimationIndFunction(void) {
    int r = gameControls.battingOrderAndPositionMapping
                [gameControls.homeTeamBattingInd_fieldingTeam]
                [gameControls.currentBatterPerTeam[gameControls.homeTeamBattingInd_fieldingTeam]][0];
    int r2;
    int p2 = 9;
    if (gameInitVariables.minigamesEnabled) {
        p2 = minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.minigamesRosterIDOfCurrentPlayer];
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

extern u8 lbl_3_data_7690[NUM_CHOOSABLE_CHARACTERS][2];
extern s16 lbl_3_data_5CD4[4];

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
            minigameStruct.minigameControlStruct[0].battingHandedness[minigameStruct.minigamesRosterIDOfCurrentPlayer];
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

// .text:0x0001482C size:0x2C mapped:0x806538c0
void initializeInMemBatter(void) {
    inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    inMemBatter.contactSize_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    inMemBatter.hitPower_raw[BAT_CONTACT_TYPE_SLAP] = 100;
    inMemBatter.hitPower_raw[BAT_CONTACT_TYPE_CHARGE] = 100;
    inMemBatter.batterHand = BATTING_HAND_RIGHT;
}

// .text:0x00014858 size:0x35C mapped:0x806538ec
void atBat_batter(void) {
    f32* p;
    if (minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(
            minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.minigamesRosterIDOfCurrentPlayer])) {
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
        p = lbl_3_data_4C88;
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
        inMemRunners[0].position.x = inMemBatter.batterPos.x;
        inMemRunners[0].position.z = inMemBatter.batterPos.z;
    }
}
