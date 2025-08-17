#include <types.h>
#include "game/UnknownHomes.h"

extern s16 hittableFrameInd[2][15];
extern f32 batContactRange[2][2];
extern s16 hittableFrameInd[2][15];
extern f32 batContactRange[2][2];

void CalculateIfHitBall() {
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
