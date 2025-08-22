#include "game/HitBall.h"
#include "game/UnknownHomes.h"
#include "header_rep_data.h"

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
    // UNFINISHED
    f32 power = inMemBall.Hit_HorizontalPower / 2.f;
    f32 horizontalAngle = angle4096_toRad(inMemBall.Hit_HorizontalAngle);
    f32 verticalAngle = angle4096_toRad(inMemBall.Hit_VerticalAngle);
    f32 s_verticalAngle = COSF(verticalAngle) * SINF(verticalAngle);
    f32 c_horizontalAngle = SINF(horizontalAngle) * COSF(horizontalAngle);
    inMemBall.physicsSubstruct.velocity.x = (c_horizontalAngle * power) / 100.f;
    inMemBall.physicsSubstruct.acceleration.y = inMemBall.hittingAddedGravityFactor;
    inMemBall.physicsSubstruct.velocity.y = (s_verticalAngle * power) / 100.f;
    inMemBall.physicsSubstruct.acceleration.x = 0.f;
    inMemBall.physicsSubstruct.acceleration.z = 0.f;
    if (!inMemBatter.isBunting && (inMemBall.Hit_HorizontalAngle < 0x900 || inMemBall.Hit_HorizontalAngle > 0xf00) &&
        inMemBall.currentStarSwing != CAPTAIN_STAR_TYPE_DK && inMemBall.currentStarSwing != CAPTAIN_STAR_TYPE_DIDDY) {
        bool hasSuperCurve = checkFieldingStat(gameControls.teamBatting, inMemBatter.rosterID, FIELDING_ABILITY_SUPER_CURVE);
    }
    estimateAndSetFutureCoords(2);
}

// .text:0x0001087C size:0x7C mapped:0x8064f910
void calculateBuntHorizontalPower(void) {
   s32 min_power, max_power, diff, horizontal_power;

    min_power = (u8)buntPower[inMemBatter.contactType][0];
    max_power = (u8)buntPower[inMemBatter.contactType][1];

    diff = max_power - min_power;

    horizontal_power = inMemBall.StaticRandomInt1 % diff;
    horizontal_power += min_power;
    inMemBall.Hit_HorizontalPower = horizontal_power;

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        inMemBall.Hit_HorizontalPower = practiceStruct.practice_hitHorizontalPower;
    }
}

// UNUSED .text:0x000108F8 size:0x188 mapped:0x8064f98c
static void calculateBallHorizontalAngle_unused(void) {
}

// UNUSED .text:0x00010A80 size:0x280 mapped:0x8064fb14
void calculateBuntHorizontalAngle(void) {
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
    f32 ballX;
    f32 ballZ;
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

        f32 product;
        ballX = inMemBall.pastCoordinates[2].x;
        ballZ = inMemBall.pastCoordinates[2].z;
        product = (inMemBall.AtBat_Contact_BallPos.x - ballX) *
                  ((inMemBall.AtBat_Contact_BallPos.z - batterZ) / (inMemBall.AtBat_Contact_BallPos.z - ballZ));
        ballFuturePosition = product + ballX;
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
}

// .text:0x00014388 size:0xCC mapped:0x8065341c
void someAnimationIndFunction(void) {
}

// .text:0x00014454 size:0x270 mapped:0x806534e8
void setDefaultInMemBatter(void) {
}

// .text:0x000146C4 size:0x168 mapped:0x80653758
void setBatterContactConstants(void) {
}

// .text:0x0001482C size:0x2C mapped:0x806538c0
void initializeInMemBatter(void) {
    inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    inMemBatter.slapContactSize_raw = 100;
    inMemBatter.chargeContactSize_raw = 100;
    inMemBatter.slapHitPower_raw = 100;
    inMemBatter.chargeHitPower_raw = 100;
    inMemBatter.batterHand = BATTING_HAND_RIGHT;
}

// .text:0x00014858 size:0x35C mapped:0x806538ec
void atBat_batter(void) {
}
