#include <types.h>
#include "game/UnknownHomes.h"

void fn_3_124DC()
{
    inMemBatter.nonCaptainStarSwingActivated = inMemBatter.noncaptainStarSwing;
    switch(inMemBatter.noncaptainStarSwing)
    {
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

void calculateHitVariables()
{

}

void calculateIfHitBall() {
    f32 batterZ;
    f32 offs1, offs2, offs3;
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
    // issue here
    ballX = inMemBall.pastCoordinates[2].x;
    ballZ = inMemBall.pastCoordinates[2].z;
    ballFuturePosition = ((inMemBall.AtBat_Contact_BallPos.z - batterZ) / (inMemBall.AtBat_Contact_BallPos.z - ballZ)) *
                             (inMemBall.AtBat_Contact_BallPos.x - ballX) +
                         ballX;

    if (inMemBatter.batterHand != BATTING_HAND_RIGHT) {
        ballFuturePosition = -ballFuturePosition;
    }

    if (ballFuturePosition - inMemBatter.batPosition2.x < batContactRange[inMemBatter.trimmedBat][0] ||
        ballFuturePosition - inMemBatter.batPosition2.x > batContactRange[inMemBatter.trimmedBat][1]) {
        return;
    }
    inMemBatter.hitContactPos.x = ballFuturePosition;
    inMemBatter.hitContactPos.z = batterZ;
    inMemBatter.hitContactPos.y = inMemBatter.batPosition.y;
    inMemBatter.contactMadeInd = 1;
}

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
        if (inMemBatter.framesBuntHeld >= 8) {
            if ((inMemPitcher.framesUntilUnhittable < 3) && (inMemBall.pitchHangtimeCounter > 0)) {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = 300;
            } else {
                inMemBatter.buntStatus = BUNT_STATUS_SHOWING;
            }
        } else {
            /* If released bunt within 8 frames if it starting */
            if (!inMemBatter.isBunting) {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = 300;
            }
        }
        return;
    }
    
    if (inMemBatter.buntStatus == BUNT_STATUS_SHOWING) {
        inMemBatter.framesBuntHeld = 100;
        if (!inMemBatter.isBunting) {
            if (inMemBall.pitchHangtimeCounter > 0) {
                if (inMemPitcher.framesUntilUnhittable > 15) {
                    inMemBatter.buntStatus = BUNT_STATUS_7;
                    inMemBatter.framesBuntHeld = 300;
                } else {
                    inMemBatter.buntStatus = BUNT_STATUS_LATE;
                }
            } else {
                inMemBatter.buntStatus = BUNT_STATUS_RETREATING;
                inMemBatter.framesBuntHeld = 300;
            }
        } else {
            calculateIfHitBall();
            if (inMemBatter.contactMadeInd) {
                // issue here
                inMemBatter.contactMadeInd = true;
                inMemPitcher.strikeInd = true;
                inMemBatter.hitGeneralType = inMemBatter.buntStatus = BUNT_STATUS_STRIKE;
                calculateHitVariables();
            } else if (inMemBatter.batPosition.z > inMemBall.AtBat_Contact_BallPos.z &&
                       inMemBall.pitchHangtimeCounter > 1) {
                inMemBatter.missedBuntStatus = 1;
                inMemBatter.buntStatus = BUNT_STATUS_STRIKE;
                inMemPitcher.strikeInd = true;
                inMemBatter.missSwingOrBunt = DID_SWING_TYPE_BUNT;
            }
        }
        return;
    }
    if (inMemBatter.buntStatus == BUNT_STATUS_RETREATING) {
        if (inMemBatter.framesBuntHeld > 330) {
            inMemBatter.buntStatus = BUNT_STATUS_NONE;
        }
        return;
    } 
    
    if ((inMemBatter.buntStatus == BUNT_STATUS_7) && (inMemBatter.framesBuntHeld > 312)) {
        inMemBatter.buntStatus = BUNT_STATUS_NONE;
        return;
    }
}


// Not confident this is bunt over hit, but Ghidra suggests bunt
static void calculateBuntHorizontalPower(void) {
    s32 min_power, max_power, diff, horizontal_power;

    min_power = (u8)buntPower[inMemBatter.contactType][0];
    max_power = (u8)buntPower[inMemBatter.contactType][1];

    diff = max_power - min_power;

    horizontal_power = inMemBall.StaticRandomInt1 % diff;
    horizontal_power += min_power;
    inMemBall.Hit_HorizontalPower = horizontal_power;

    if (gameInitVariables.GameModeSelected == 2) {
        if (practiceStruct.instructionNumber >= 0) {
            inMemBall.Hit_HorizontalPower = practiceStruct.practice_hitHorizontalPower;
        }
    }
}


void initializeInMemBatter()
{
    inMemBatter.hitGeneralType = BAT_CONTACT_TYPE_SLAP;
    inMemBatter.slapContactSize_raw = 100;
    inMemBatter.chargeContactSize_raw = 100;
    inMemBatter.slapHitPower_raw = 100;
    inMemBatter.chargeHitPower_raw = 100;
    inMemBatter.batterHand = BATTING_HAND_RIGHT;
}
