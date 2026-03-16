#ifndef __GAME_HitBall_H_
#define __GAME_HitBall_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_Static.h"

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

extern struct {
    u8 _00[0x28];
    u8 _28;
} lbl_80366158;
extern u8 lbl_800E8754[];

extern s16 lbl_3_data_4D94[3][2][15][2];
extern s16 lbl_3_data_4EFC[3][2][15][2];
extern s16 lbl_3_data_5064[3][2][15][2];

void batterPreSwing_unused(void);
void calculateBallVelocityAcceleration(void);
void calculateBuntHorizontalPower(void);
void calculateBuntHorizontalAngle(void);
void calculateHorizontalPower(void);
void calculateVerticalAngle(void);
void calculateBallHorizontalAngleHit(void);
void calculateContactAndHitType(void);
void calculateHitVariables(void);
void calculateIfHitBall(void);
void ifBunt(void);
void starSwingSpendStars(void);
void ifSwing(void);
void batterInBoxMovement(void);
void batterHumanControlled(void);
void updateBallHittableZoneStatus(void);
int someAnimationIndFunction(void);
void setDefaultInMemBatter(void);
void setBatterContactConstants(void);
void initializeInMemBatter(void);
void atBat_batter(void);
void starHitSetting_unused(void);

#endif // !__GAME_HitBall_H_
