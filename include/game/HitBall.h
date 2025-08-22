#ifndef __GAME_HitBall_H_
#define __GAME_HitBall_H_

#include "types.h"

void calculateBallVelocityAcceleration(void);
void fn_3_1087C(void);
void calculateBuntHorizontalAngle(void);
void calculateHorizontalPower(void);
void calculateVerticalAngle(void);
void calculateBallHorizontalAngleHit(void);
void calculateContactAndHitType(void);
void fn_3_124DC(void);
void calculateHitVariables(void);
void calculateIfHitBall(void);
void ifBunt(void);
void starSwingSpendStars(void);
void ifSwing(void);
void fn_3_13614(void);
void batterInBoxMovement(void);
void batterHumanControlled(void);
void updateBallHittableZoneStatus(void);
void someAnimationIndFunction(void);
void setDefaultInMemBatter(void);
void setBatterContactConstants(void);
void initializeInMemBatter(void);
void atBat_batter(void);

#endif // !__GAME_HitBall_H_
