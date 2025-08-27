#ifndef __UNKNOWN_HOMES_H_
#define __UNKNOWN_HOMES_H_

#include "mssbTypes.h"
#include "Dolphin/pad.h"
#include "math.h"

#define SINF(x) ((f32)sin(x))
#define COSF(x) ((f32)cos(x))
#define ATAN2F(y, x) ((f32)atan2((y), (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))

#define BATTER_MAX_CHARGE (1.f)
#define BATTER_MIN_CHARGE (0.f)
#define PLAYERS_PER_TEAM (9)
#define TEAMS_PER_GAME (2)


typedef struct _VecXZ {
    f32 x, z;
} VecXZ;

typedef struct _VecXYZ {
    f32 x, y, z;
} VecXYZ;

typedef struct _Orientationf {
    s16 yaw, pitch, roll;
} Orientationf;

typedef struct _InMemBatterType {
    /*0x00*/ VecXZ batterPos;
    /*0x08*/ VecXYZ batPosition; // with x offset
    /*0x14*/ u8 _14[0x10];
    /*0x24*/ VecXYZ hitContactPos;
    /*0x30*/ f32 rightNiceThreshold;
    /*0x34*/ f32 rightPerfectThreshold;
    /*0x38*/ f32 leftPerfectThreshold;
    /*0x3C*/ f32 leftNiceThreshold;
    /*0x40*/ f32 contactQualityAbsolute;
    /*0x44*/ f32 contactQuality;
    /*0x48*/ f32 predictedPitchXWhenBallReachesBatter;
    /*0x4C*/ VecXYZ batPosition2;
    /*0x58*/ f32 chargeUp;
    /*0x5C*/ f32 chargeDown;
    /*0x60*/ s16 rosterID;
    /*0x62*/ E(s16, CHAR_ID) charID;
    /*0x64*/ s16 charIDForScoutFlagMission;
    /*0x66*/ s16 framesSinceStartOfSwing;
    /*0x68*/ s16 frameSwung;
    /*0x6A*/ s16 framesBuntHeld;
    // min Frames Until Can Swing Again: [0]slap [1]charge/Star
    /*0x6C*/ s16 frameDelay[2];
    /*0x70*/ s16 frameFullyCharged;
    /*0x72*/ s16 frameChargeDownBegins;
    /*0x74*/ s16 chargeFrames;
    /*0x76*/ s16 _76;
    /*0x78*/ u8 aiControlledInd;
    /*0x79*/ u8 aiLevel;
    /*0x7A*/ u8 easyBatting;
    /*0x7B*/ E(u8, BATTING_HAND) batterHand;
    /*0x7C*/ u8 slapHitPower_raw;
    /*0x7D*/ u8 chargeHitPower_raw;
    /*0x7E*/ u8 slapContactSize_raw;
    /*0x7F*/ u8 chargeContactSize_raw;
    /*0x80*/ u8 slapHitPower_capped;
    /*0x81*/ u8 chargeHitPower_capped;
    /*0x82*/ u8 slapContactSize_capped;
    /*0x83*/ u8 chargeContactSize_capped;
    /*0x84*/ u8 buntingContactSize;
    /*0x85*/ u8 trajectoryPushPull;
    /*0x85*/ u8 trajectoryHighLow;
    /*0x87*/ u8 captainStarHitPitch;
    /*0x88*/ E(u8, REGULAR_STAR_SWING) noncaptainStarSwing;
    /*0x89*/ E(u8, CHARACTER_CLASS) characterClass;
    /*0x8A*/ bool trimmedBat;
    /*0x8B*/ E(u8, BAT_CONTACT_TYPE) hitGeneralType;
    /*0x8C*/ u8 _8C;
    /*0x8D*/ u8 swingInd;
    /*0x8E*/ u8 isBunting;
    /*0x8F*/ E(u8, BUNT_STATUS) buntStatus;
    /*0x90*/ u8 missedBuntStatus;
    /*0x91*/ u8 contactMadeInd;
    /*0x92*/ E(u8, HIT_CONTACT_TYPE) contactType;
    /*0x93*/ u8 hitByPitch;
    /*0x94*/ E(u8, HIT_TRAJECTORY) hitTrajectory;
    /*0x95*/ u8 _95; // always0_cutHitPowerInHalf
    /*0x96*/ u8 _96;
    /*0x97*/ u8 displayContactSprite; // displays nice/perfect on screen
    /*0x98*/ u8 _98;
    /*0x98*/ E(u8, DID_SWING_TYPE) missSwingOrBunt;
    /*0x9A*/ u8 noSwingAnimationInd;
    /*0x9B*/ u8 Stored_Frame_SwingContact_SinceMiss;
    /*0x9C*/ u8 _9C;
    /*0x9D*/ E(u8, CHARGE_SWING_STAGE) chargeStatus;
    /*0x9E*/ bool isFullyCharged;
    /*0x9F*/ u8 countUpUntilChargeEnables;
    /*0xA0*/ bool didNonCaptainStarSwingConnect;
    /*0xA1*/ bool isStarSwing;
    /*0xA2*/ E(u8, CAPTAIN_STAR_TYPE) captainStarSwingActivated;
    /*0xA3*/ u8 lightFireballStunID;
    /*0xA4*/ u8 starSwing_starsHaveBeenSpent;
    /*0xA5*/ bool moonShotInd;
    /*0xA6*/ u8 beginningOfABAnimationOccuring;
    /*0xA7*/ u8 batterIsBeingCentredInd;
    /*0xA8*/ u8 invisibleBallForPeachStarHit;
    /*0xA9*/ E(u8, STICK_SIDEWAYS_INPUT) inputDirection;
    /*0xAA*/ u8 chemLinksOnBase;
    /*0xAB*/ u8 runnersOnBase;
    /*0xAC*/ E(u8, HIT_TYPE) hitType;
    /*0xAD*/ E(u8, BALL_HITTABLE) isBallInHittableZone;
    /*0xAE*/ u8 swingMissThatWasHittable;
    /*0xAF*/ E(u8, REGULAR_STAR_SWING) nonCaptainStarSwingActivated;
} InMemBatterType; // size: 0xb0

typedef struct _InMemRunnerType {
    /*0x000*/ VecXYZ position;
    /*0x00C*/ VecXYZ positionStored;
    /*0x018*/ VecXYZ velocity;
    /*0x024*/ VecXYZ velocityStored;
    /*0x030*/ f32 runningAngle;
    /*0x034*/ f32 angleToBall;
    /*0x038*/ f32 distanceFromBall;
    /*0x03C*/ VecXYZ currentBaseCoordinates;
    /*0x048*/ VecXYZ nextBaseCoordinates;
    /*0x054*/ f32 slidingAdjustment_backwards; // 0.03f
    /*0x058*/ f32 slidingAdjustment_forwards;  // 0.03f
    /*0x05C*/ f32 percentFromCurrentBase_slideAdj;
    /*0x060*/ f32 percentTowardsNextBase_slideAdj;
    /*0x064*/ f32 fractionalBasesRan;
    /*0x068*/ f32 percentTowardsNextBase;
    /*0x06C*/ f32 calculatedLengthOfCurrentBaseline;
    /*0x070*/ f32 fractionOfCalculatedBaselineRan;
    /*0x074*/ f32 distToCurrentBase;
    /*0x078*/ f32 distToNextBase;
    /*0x07C*/ f32 fractionalBasesRan_stored;
    /*0x080*/ f32 percentTowardsNextBase_stored;
    /*0x084*/ f32 groundVelocity[4];
    /*0x094*/ f32 acceleration;
    /*0x098*/ f32 baseAcceleration;                        // between 0.005 and 0.01
    /*0x09C*/ f32 baseAccelerationWhileChaingingDirection; // 0.005
    /*0x0A0*/ f32 maximumBaseVelocity;
    /*0x0A4*/ f32 percentRanPerFrame_slideAdj;
    /*0x0A8*/ f32 velocityPercent_stamAdj;
    /*0x0AC*/ f32 accelerationPercent_stamAdj;
    /*0x0B0*/ f32 roundingStrengthPercent;                    // unsure
    /*0x0B4*/ f32 overrun1B_someDistConst_proportionPerFrame; // maybe
    /*0x0B8*/ f32 overrun1B_somePositionControl;
    /*0x0BC*/ f32 leadoffDistancePercent;
    /*0x0C0*/ f32 mashVeloAdjustment;
    /*0x0C4*/ f32 maxMashVeloAdjustment; // 0.02
    /*0x0C8*/ f32 mashPercent;
    /*0x0CC*/ f32 percentAddedPerMash;                  // 0.2
    /*0x0D0*/ f32 stamina_MashPercentTakenAwayPerFrame; // 0.2
    /*0x0D4*/ f32 staminaMult;
    /*0x0D8*/ f32 accelerationStaminaEffect;
    /*0x0DC*/ f32 accelerationStaminaEffectWhileChangingDirection;
    /*0x0E0*/ s16 rosterID;
    /*0x0E2*/ s16 charID;
    /*0x0E4*/ s16 unused_starMissionCharID;
    /*0x0E6*/ s16 baseStandingOn;
    /*0x0E8*/ s16 baseStandingOn_Stored;
    /*0x0EA*/ s16 framesToNextBase;
    /*0x0EC*/ s16 framesToPreviousBase; // maybe
    /*0x0EE*/ E(s16, FORCE_OUT_TYPE) forceOutCd;
    /*0x0F0*/ E(s16, PREVENT_PASSING_TYPE) restrictedMovementCodes;
    /*0x0F2*/ E(s16, PREVENT_PASSING_TYPE) restrictedMovementCodes_stored;
    /*0x0F4*/ s16 framesSinceOut;
    /*0x0F6*/ s16 stamina;
    /*0x0F8*/ s16 baseNumberEarned_NotIncludingFieldersChoice;
    /*0x0FA*/ s16 timeStandingOnBase;
    /*0x0FC*/ s16 pitcherWhoLetRunnerOnBase;
    /*0x0FE*/ s16 baseReachedAtTimeOfThrow;
    /*0x100*/ s16 actionFrames_countUp;
    /*0x102*/ s16 actionFrames_countDown;
    /*0x104*/ s16 overrunBaseFrames_countUp;
    /*0x106*/ s16 overrunBaseFrames_countDown;
    /*0x108*/ s16 runningToDugoutFrameCounter;
    /*0x10A*/ s16 framesToReachDugout; // maybe
    /*0x10C*/ s16 leadoffDurationFrames;
    /*0x10E*/ s16 leadOffTotalFrameCountDown;
    /*0x110*/ s16 _110;
    /*0x112*/ s16 framesSinceLastDirectionChange;
    /*0x114*/ s16 slideHomeFrames_CountDown;
    /*0x116*/ s16 framesSinceStealStarted;
    /*0x118*/ s16 _118;
    /*0x11A*/ u8 unused_batterHandednessForCCS; // maybe
    /*0x11B*/ u8 aIStrength0Special3Weak;
    /*0x11C*/ E(u8, BATTING_HAND) battingHand;
    /*0x11D*/ u8 weight;
    /*0x11E*/ E(u8, CHARACTER_CLASS) characterClass;
    /*0x11F*/ u8 _11f;
    /*0x120*/ u8 speed;
    /*0x121*/ u8 _121; // 10
    /*0x122*/ u8 delayBeforeStartingToRun;
    /*0x123*/ E(u8, RUNNER_STATUS) runnerOnFieldOrOutOrScored;
    /*0x124*/ u8 startingBase_baseAchieved;
    /*0x125*/ u8 currentBase;
    /*0x126*/ u8 nextBase;
    /*0x127*/ u8 baseRunningTowards;
    /*0x128*/ E(u8, TAG_UP_TYPE) tagUpInd;
    /*0x129*/ u8 batterStayInBattersBoxReason;
    /*0x12A*/ u8 unused_AIRelated;
    /*0x12B*/ u8 relatedToRunnerPos;
    /*0x12C*/ u8 outType;
    /*0x12D*/ u8 forcedToAdvanceInd; // maybe
    /*0x12E*/ u8 unused_someBaseNum;
    /*0x12F*/ u8 baseOfFailedBodyCheck; // maybe
    /*0x130*/ u8 tagType;               // 1=Running 2=Sliding
    /*0x131*/ u8 runnerDidntReachOnError;
    /*0x132*/ u8 isEligibleToScore; // maybe
    /*0x133*/ u8 leadOffStatus;
    /*0x134*/ u8 unused_const_1;
    /*0x135*/ u8 runningDirectionDesired;
    /*0x136*/ u8 nextDirectionBeingProcessed;
    /*0x137*/ u8 runningDirectionCode;
    /*0x138*/ u8 runnerDirectionCode_stored;
    /*0x139*/ u8 offsetFromNormalRunningPathInd; // maybe
    /*0x13A*/ u8 actionCode;
    /*0x13B*/ u8 actionInForwardDirectionInd;
    /*0x13C*/ u8 actionStage;
    /*0x13D*/ u8 forceOutType_unsed;
    /*0x13E*/ u8 baseRoundingState;
    /*0x13F*/ u8 overrun1st_doneChecking;
    /*0x140*/ u8 overRun1BStage;
    /*0x141*/ u8 overrunning1BIndicator;
    /*0x142*/ u8 overrunBaseStage;
    /*0x143*/ u8 offsetPositionForRoundingInd;
    /*0x144*/ u8 roundingInitiaializedInd;
    /*0x145*/ u8 runningToDugoutInd;
    /*0x146*/ u8 runningToDugoutStage;
    /*0x147*/ u8 turnaroundCode;
    /*0x148*/ u8 turningAroundInd; // maybe
    /*0x149*/ u8 newButtonThisFrame_forMashPurposes;
    /*0x14A*/ u8 framesSinceLastMash;
    /*0x14B*/ u8 FramesUntilNotSprinting;        // 6?
    /*0x14C*/ u8 someCollisionCheck;             // maybe
    /*0x14D*/ u8 furthestBaseForcedToGoToOnWalk; // steal related
    /*0x14E*/ u8 stealingStatus;
    /*0x14F*/ u8 framesSinceStealInput;
    /*0x150*/ u8 someCountdown_unused;
    /*0x151*/ u8 _151;
    /*0x152*/ u8 miniGamePlayerNum;
    /*0x153*/ u8 scoredOnGRD; // maybe
} InMemRunnerType;            // size: 0x154

typedef struct _CoordAndDist {
    VecXYZ pos;
    f32 dist;
} CoordAndDist;

typedef struct {
    /*0x000*/ VecXYZ startOfWallJumpChecksMaybe[33];
    artificial_padding(0, 0x198, VecXYZ[33]);
    /*0x198*/ VecXYZ velocity;
    /*0x1A4*/ VecXYZ acceleration;
    /*0x1B0*/ f32 gravity; // 0.00275f
    /*0x1B4*/ f32 threeFrameLookback[2];
    /*0x1BC*/ f32 twoFrameLookback[2];
    f32 throwYAtSelectedFuturePoint;
    VecXZ ballLandingSpotOrHeldSpot;
    f32 hitLandingSpotDistFromHome;
    CoordAndDist futureCoordsAndDist[360];
} pastAndFutureCoordsWallJumpRelated;

typedef struct _matchShorts {
    /*0x00*/ s16 framesAfterReceivingThrow; // unsure
    /*0x02*/ s16 ballAndFielderOnBaseFrames;
    /*0x04*/ s16 ballOverWallFrames;
    /*0x06*/ Orientationf ballSpinAngle;
    /*0x0C*/ s16 _0C;
    /*0x0E*/ s16 framesSinceFoulCalled;
    /*0x10*/ s16 framesSinceLastThrow; // unsure
    /*0x12*/ s16 framesOnGround;
    /*0x14*/ s16 bananaHitStartFrame;
    /*0x16*/ s16 bananaHitEndFrame;
    /*0x18*/ s16 garlicHitFramesUntilHitGroundForSplit;
    /*0x1A*/ s16 garlicHitFramesSinceSplit;
    /*0x1C*/ s16 framesInsidePlant;
} matchShorts;

typedef struct _InMemBallType {
    /*0x0000*/ VecXYZ AtBat_Contact_BallPos;
    /*0x000C*/ VecXYZ offsetWhilePickedUpHistory[4];
    /*0x003C*/ VecXYZ pastCoordinates[27];
    /*0x0180*/ pastAndFutureCoordsWallJumpRelated physicsSubstruct;
    /*0x19D4*/ f32 maxYOfHit;
    /*0x19D8*/ VecXZ ballWillHitBallPos;
    /*0x19E0*/ f32 wallAndBallIntersectionDistFromHome;
    /*0x19E4*/ f32 _19E4;
    /*0x19E8*/ f32 _19E8;
    /*0x19EC*/ VecXYZ throwDestination;
    /*0x19F8*/ VecXZ throwTarget;
    /*0x1A00*/ f32 ballDistanceFromHome;
    /*0x1A04*/ f32 ballDistanceFromBase[4];
    /*0x1A14*/ f32 ballVelocity;
    /*0x1A18*/ VecXZ ballVelocityPercent;
    /*0x1A20*/ f32 ballHitGrroundDistanceFromHome;
    /*0x1A24*/ f32 distLandingSpotToMound;
    /*0x1A28*/ f32 groundYForBounces;
    /*0x1A2C*/ VecXZ landingSpotLocation;
    /*0x1A34*/ VecXZ throwStartingLocation;
    /*0x1A3C*/ VecXYZ deadballLastLoc;
    /*0x1A48*/ f32 someYCoord;
    /*0x1A4C*/ f32 throwDistance;
    /*0x1A50*/ VecXZ ballPickedUpCaught;
    /*0x1A58*/ f32 ballEnergy;
    /*0x1A5C*/ f32 hittingAddedGravityFactor;
    /*0x1A60*/ VecXZ savedVelocity;
    /*0x1A68*/ VecXYZ warioStarHitCoords[15];
    /*0x1B1C*/ VecXZ peachDaisyStarHitFielderLoc;
    /*0x1B24*/ VecXZ unused_garlicHitRelated;
    /*0x1B2C*/ VecXYZ diveCatchLocationOffset;
    /*0x1B38*/ VecXYZ fielderActionCatchCoords;
    /*0x1B44*/ s32 collisionCode;
    /*0x1B48*/ f32 maybeCollisionRelated;
    /*0x1B4C*/ s32 StaticRandomInt1;
    /*0x1B50*/ s32 StaticRandomInt2;
    /*0x1B54*/ u32 StaticRandomInt1_prePitch;
    /*0x1B58*/ s16 airResistance; // 40
    /*0x1B5A*/ s16 _1B58;
    /*0x1B5C*/ s16 frameBallWillHitWall; // unused
    /*0x1B5E*/ s16 hangtimeOfHit;
    /*0x1B60*/ s16 framesUntilBallHitsGround;
    /*0x1B62*/ s16 framesUntilThrowReachesDest;
    /*0x1B64*/ s16 totalFramesAtPlay;
    /*0x1B66*/ s16 framesSinceHit; // +100 For Pickoff
    /*0x1B68*/ s16 pitchHangtimeCounter;
    /*0x1B6A*/ s16 postPitchResultCounter;
    /*0x1B6C*/ s16 framesSinceThrowStarted;
    /*0x1B6E*/ s16 timeSinceBallPickedUp;
    /*0x1B70*/ s16 framesSincePickOff;
    /*0x1B72*/ s16 framesSinceBallHitGroundOrWasCaught;
    /*0x1B74*/ s16 framesSinceLastBounce;
    /*0x1B76*/ s16 framesSinceBallHitWall;
    /*0x1B78*/ s16 fielderWBallIndex;
    /*0x1B7A*/ E(s16, BALL_RESULT_TYPE) AtBat_ContactResult;
    /*0x1B7C*/ s16 fairBallInd;
    /*0x1B7E*/ s16 fielderAboutToGetBall_hasBall;
    /*0x1B80*/ sAng ballAngleFromHome;
    /*0x1B82*/ sAng ballTravelAngle;
    /*0x1B84*/ s16 landingSpotAngle;
    /*0x1B86*/ s16 baseBallAndFielderAreOn;
    /*0x1B88*/ s16 _1B88; // -1
    /*0x1B8A*/ s16 fielderBeingThrownTo;
    /*0x1B8C*/ s16 ballBounceState; // unsure
    /*0x1B8E*/ s16 fielderWithBallIndexStored;
    /*0x1B90*/ s16 fielderWithBallIndexStored2;
    /*0x1B92*/ s16 fielderWhoGotLastOut; // unsure
    /*0x1B94*/ s16 throwingFielder;      // unsure, Maybe Cut off Related
    /*0x1B96*/ s16 collisionRelated;
    /*0x1B98*/ s16 _1B98; // unused_frameHitGetsAboveY=1.3
    /*0x1B9A*/ sAng Hit_VerticalAngle;
    /*0x1B9C*/ sAng Hit_HorizontalAngle;
    /*0x1B9E*/ s16 Hit_HorizontalPower; // bODBasePoints
    /*0x1BA0*/ matchShorts matchFramesAndBallAngle;
    /*0x1BBE*/ u8 ballZoneAwayFromHome;
    /*0x1BBF*/ u8 landingSpotZoneAwayFromHome;
    /*0x1BC0*/ u8 _1BC0; // seeminglyAlways1_ballCollideWallRelated;
    /*0x1BC1*/ u8 someCollisionVariable;
    /*0x1BC2*/ u8 thrownBallHasHitGround;
    /*0x1BC3*/ u8 hitClassification1;
    /*0x1BC4*/ u8 hitClassification2;
    /*0x1BC5*/ u8 hitClassification3;
    /*0x1BC6*/ u8 ballInitialHitDoneInd;
    /*0x1BC7*/ u8 howFoulTheBallWillBe;
    /*0x1BC8*/ u8 always0_fairFoulRelated;
    /*0x1BC9*/ u8 ballState;
    /*0x1BCA*/ u8 ballIsLooseInd_unused;
    /*0x1BCB*/ u8 IsAntichemistryThrow;
    /*0x1BCC*/ u8 hitWallInd;
    /*0x1BCD*/ u8 homeRunClassification;
    /*0x1BCE*/ u8 lineDriveThroughPitcherInd;
    /*0x1BCF*/ u8 maybeBuntInd;
    /*0x1BD0*/ u8 groundRuleDoubleInd; // unsure
    /*0x1BD1*/ u8 deadBallReason;
    /*0x1BD2*/ u8 deadBallRBIsAddedInd;
    /*0x1BD3*/ u8 maybebuntOn2Strikes;
    /*0x1BD4*/ u8 ballOnMoundInd;
    /*0x1BD5*/ u8 throwHasLastedEstimatedNOfFrames;
    /*0x1BD6*/ u8 framesOnGroundUntilPickedUp;
    /*0x1BD7*/ u8 numberOfThrowsDuringPlay;
    /*0x1BD8*/ u8 numFieldersWhoHandledBallDuringPlay;
    /*0x1BD9*/ u8 numThrowsDuringPlay;
    /*0x1BDA*/ u8 bobbleLocation_1fair_2foul;
    /*0x1BDB*/ u8 _1BDB;
    /*0x1BDC*/ u8 homeRunInd;
    /*0x1BDD*/ u8 ballZoneWhenCaught;
    /*0x1BDE*/ u8 ballStoppingCode1ReallySlow2Stopped;
    /*0x1BDF*/ u8 _1BDF;
    /*0x1BE0*/ u8 throwTimeEstimatesCompleteInd;
    /*0x1BE1*/ u8 looseBall_codeForHowLongUntilSomeoneWillGetIt; // unsure
    /*0x1BE2*/ u8 ballZoneFromHomeAtStartOfThrow;
    /*0x1BE3*/ u8 looseBall_5FrameCountdown;
    /*0x1BE4*/ u8 ballIsRollingIndicator;
    /*0x1BE5*/ u8 inAirOrBefore2ndBounceOrLowBallEnergy;
    /*0x1BE6*/ u8 hardHitIndicator;
    /*0x1BE7*/ u8 currentStarSwing;
    /*0x1BE8*/ u8 currentStarSwing2;
    /*0x1BE9*/ u8 directionOfBananaHit;
    /*0x1BEA*/ u8 knockoutProcessedFlag;
    /*0x1BEB*/ u8 warioWaluGarlicIsActive;
    /*0x1BEC*/ u8 warioWaluStarHitDirection;
    /*0x1BED*/ u8 autoFielderAvoidDropSpotForPeachesStarHit;
    /*0x1BEE*/ u8 bODQualifyingHitInd;
    /*0x1BEF*/ u8 fielderActionOccuring;
    /*0x1BF0*/ u8 catchAnimationTotalFrames;
    /*0x1BF1*/ u8 someCollisionInd;
    /*0x1BF2*/ u8 hitNoteBlockInd;
    /*0x1BF3*/ u8 pauseBallMovementWhenInPlant;
    /*0x1BF4*/ u8 ballCughtByPlantInd;
    /*0x1BF5*/ u8 frameCountdownAfterLeavingPlant;
} InMemBallType; // size: 0x1BF8

typedef struct _InMemPitcherType {
    /*0x000*/ VecXYZ ballCurrentPosition;
    /*0x00C*/ VecXYZ ballLastPosition;
    /*0x018*/ VecXYZ ballVelocity;
    /*0x024*/ f32 pitchStartingPosition_AIMaxCurve; // unsure
    /*0x028*/ f32 eggBallBounceYHeight;             // unsure
    /*0x02C*/ f32 frontOfPlateZ;                    // unsure
    /*0x030*/ VecXYZ _30;
    /*0x03C*/ f32 _3C[6];
    /*0x054*/ VecXZ estimatedEndingPos;
    /*0x05C*/ f32 pitchCurveVeloV1;
    /*0x060*/ f32 _60;
    /*0x064*/ f32 pitchCurveVeloV2;
    /*0x068*/ f32 _68;
    /*0x06C*/ VecXZ pitcherCoord;
    /*0x074*/ VecXZ centerOfStrikeZone;
    /*0x07C*/ f32 strikeZoneLeft;
    /*0x080*/ f32 strikeZoneRight;
    /*0x084*/ f32 beginningOfStrikeCheckZ; // 1.05
    /*0x088*/ f32 endingOfStrikeCheckZ;    // 0.5
    /*0x08C*/ VecXZ pitcher;
    /*0x094*/ VecXYZ pitchRelease;
    /*0x0A0*/ f32 pitchLinearInterpolateX;
    /*0x0A4*/ f32 pitchLinearInterpolateY;
    /*0x0A8*/ f32 frontOfPlateZ2;
    /*0x0AC*/ f32 pitchSpeedScaler;
    /*0x0B0*/ f32 pitchZ_whenAirResistanceStarts;
    /*0x0B4*/ f32 airResistance_veloAdj;
    /*0x0B8*/ f32 moundZ; // 18.44
    /*0x0BC*/ f32 decelerationFactor;
    /*0x0C0*/ f32 verticalOffsetParabolaMidpoint;
    /*0x0C4*/ f32 verticalGlobalParabolicVelo;
    /*0x0C8*/ f32 pitchY_parabolicAdjustment;
    /*0x0CC*/ f32 horizontalOffsetParabolaMidpoint;
    /*0x0D0*/ f32 horizontalGlobalParabolicVelo;
    /*0x0D4*/ f32 pitchX_parabolicAdjustment;
    /*0x0D8*/ f32 pitchXPosition;  // unsure
    /*0x0DC*/ f32 pitchXPosition2; // unsure
    /*0x0E0*/ f32 pitchChargeUp;
    /*0x0E4*/ f32 pitchChargeUpAnimationProportion; // unsure
    /*0x0E8*/ VecXYZ starPitchPositionAdjustment;
    /*0x0F4*/ VecXYZ _F4;
    /*0x100*/ f32 bulletPitchLoopAngleRadians;
    /*0x104*/ f32 _104;
    /*0x108*/ f32 eggBallBounceZLoc;
    /*0x10C*/ f32 _10C[3];
    /*0x118*/ s16 rosterID;
    /*0x11A*/ E(s16, CHAR_ID) charID;
    /*0x11C*/ s16 scoutFlagRelated;
    /*0x11E*/ s16 pitchTotalTimeCounter;
    /*0x120*/ s16 currentStateFrameCounter;
    /*0x122*/ s16 frameWhenUnhittable;
    /*0x124*/ s16 _122;
    /*0x126*/ s16 windupCountdownUntilBallReleased;
    /*0x128*/ s16 pitchWindUpCountDown;
    /*0x12A*/ s16 curvePitchWindupFrames;
    /*0x12C*/ s16 _12C;
    /*0x12E*/ s16 pickOffLoc; // 4noThrow/5Steal;
    /*0x130*/ s16 framesSinceAtBatEnded;
    /*0x132*/ s16 framesUntilBallReachesBatterZ;
    /*0x134*/ s16 framesUntilPitchGetsToBatter;
    /*0x136*/ s16 unknownFrameCounter;
    /*0x138*/ s16 framesSinceFirstEggBounce;
    /*0x13A*/ s16 ballBouncePeakZ;
    /*0x13C*/ s16 warioWaluStarHasPlayedSound;
    /*0x13E*/ u8 pitcherActionState;
    /*0x13F*/ u8 AIInd; // unsure
    /*0x140*/ u8 aiLevel;
    /*0x141*/ u8 handedness;
    /*0x142*/ u8 curveBallSpeed;
    /*0x143*/ u8 fastBallSpeed;
    /*0x144*/ u8 cursedBallStat;
    /*0x145*/ u8 curveControlStat;
    /*0x146*/ u8 curveStat;
    /*0x147*/ u8 captainStarPitch;
    /*0x148*/ u8 nonCaptainStarPitch;
    /*0x149*/ E(u8, CHARACTER_CLASS) charClass;
    /*0x14A*/ u8 pitchSpeed;
    /*0x14B*/ u8 calced_cursedBall;
    /*0x14C*/ u8 calced_curveControl;
    /*0x14D*/ u8 calced_curve;
    /*0x14E*/ u8 strikeOutOrWalk;
    /*0x14F*/ u8 strikeZoneProcessNumber;
    /*0x150*/ u8 specialPitchTypeCode;
    /*0x151*/ u8 frameBallCanStartBeingControlled; // unsure
    /*0x152*/ u8 pitchInAirInd;
    /*0x153*/ u8 cancelParabolicAdjustmentInd;
    /*0x154*/ u8 pitchDidntResultInLiveBallInd;
    /*0x155*/ u8 framesUntilUnhittable;
    /*0x156*/ u8 calledStrikeInd;
    /*0x157*/ u8 strikeInd;
    /*0x158*/ u8 miniGameRelated;
    /*0x159*/ u8 nPitchesThisAB;
    /*0x15A*/ u8 nPickoffAttempts;
    /*0x15B*/ u8 _15B;
    /*0x15C*/ u8 _15C;
    /*0x15D*/ u8 _15D;
    /*0x15E*/ u8 pitchDeliveryAnimationPlaying;
    /*0x15F*/ u8 ChargePitchType;
    /*0x160*/ u8 framesAHeldForChargePitches;
    /*0x161*/ u8 TypeOfPitch;
    /*0x162*/ u8 ballHaloTrainInd_unused;
    /*0x163*/ u8 overChargeInd;
    /*0x164*/ u8 starPitchInd;
    /*0x165*/ u8 starPitchType;
    /*0x166*/ u8 warioWaluStarAnimationStage;
    /*0x167*/ u8 warioWaluStarPitchRightLeft;
    /*0x168*/ u8 peachDaisyStarAnimationOn;
    /*0x169*/ u8 peachDaisyAnimationHappened;
    /*0x16A*/ u8 anyCurveInput;
    /*0x16B*/ u8 bulletPitchStageCode;
    /*0x16C*/ u8 bulletPitchFrameCounter;
    /*0x16D*/ u8 bulletPitchLoopFrames;
    /*0x16E*/ u8 bulletPitchLoopStartingFrame;
    /*0x16F*/ u8 _16F;
    /*0x170*/ u8 _170;
    /*0x171*/ u8 eggBallBounceNumber;
    /*0x172*/ u8 pitcherOffCenter;
    /*0x173*/ u8 playStartOfGameAnimation; // unsure
    /*0x174*/ u8 nonCaptainStarPitchTriggeredType;
    /*0x175*/ u8 walkedInRunInd;
    /*0x176*/ u8 unused_pitcherIsFielder; // unsure
} InMemPitcherType;                       // size: 0x178

extern InMemPitcherType inMemPitcher;
extern InMemBallType inMemBall;
extern InMemRunnerType inMemRunner[4];
extern InMemBatterType inMemBatter;

extern VecXYZ maybeInitialBatPos;
extern f32 charSizeMultipliers[NUM_CHOOSABLE_CHARACTERS][2];

#define FRAME_COUNT_HIT_WINDOW 15

// 0 is slap, 1 is charge
extern s16 hittableFrameInd[2][FRAME_COUNT_HIT_WINDOW];
/*
= {
    { // slap
        false, false, true, true, // frames 0-3
        true, true, true, true,   // frames 4-7
        true, true, true, false,  // frames 8-11
        false, false, false,      // frames 12-14
    },
    { // charge/star
        false, false, false, true, // frames 0-3
        true, true, true, true,    // frames 4-7
        true, true, false, false,  // frames 8-11
        false, false, false,       // frames 12-14
    },
};
*/

// indexed by trimmed bat
typedef struct _BatNearFarReach {
    f32 near, far;
} BatNearFarReach;

extern BatNearFarReach batContactRange[2];
/*
= {
    // full-length bat
    { -0.85f, 0.15f },
    // trimmed bat
    { -0.35f, 0.35f },
}
*/

typedef struct _HitFloats {
    /*0x00*/ f32 minStarHitVelo;
    /*0x04*/ f32 DKStarAngleDelta;
    /*0x08*/ f32 DKStarHangtimePercentStart;
    /*0x0C*/ f32 DKStarHangtimePercentEnd;
    /*0x10*/ f32 eggVeloMaintainedOnBounce;
    /*0x14*/ f32 bulletStarEnergyMultiplier;
    /*0x18*/ f32 garlicSpreadLower;
    /*0x1C*/ f32 garlicSpreadUpper;
    /*0x20*/ f32 overChargeLerpBtwnSlapAndChargePower;
    /*0x24*/ f32 regularBoxSpeedMult;
    /*0x28*/ f32 centeringBoxSpeedMult;
    /*0x2C*/ f32 moonshotMult;
} HitFloats;

extern HitFloats g_hitFloats;
/*
 = {
    0.05f,  // minStarHitVelo
    0.008f, // DKStarAngleDelta
    0.25f,  // DKStarHangtimePercentStart
    0.95f,  // DKStarHangtimePercentEnd
    0.8f,   // eggVeloMaintainedOnBounce
    4.f,    // bulletStarEnergyMultiplier
    0.2f,   // garlicSpreadLower
    0.3f,   // garlicSpreadUpper
    0.5f,   // overChargeLerpBtwnSlapAndChargePower
    1.f,    // regularBoxSpeedMult
    3.f,    // centeringBoxSpeedMult
    1.5f    // moonshotMult
};
*/

typedef struct _HitShorts {
    /*0x00*/ s16 frameChargeDownEnds;
    /*0x02*/ s16 framesUntilChargeIsEnabled;
    /*0x04*/ s16 _04;
    /*0x06*/ s16 framesOnGroundBeforeCheckingMinStarVelo;
    /*0x08*/ s16 fireballStunFrames;
    /*0x0A*/ s16 unused_alternateFireballStunFrames;
    /*0x0C*/ s16 NEggBounces;
    /*0x0E*/ s16 _0C;
    /*0x10*/ s16 framesBeforeGroundWhenGarlicSplits;
    /*0x12*/ s16 FrameWhenPeachHitAppearsAgain;
    /*0x14*/ s16 FrameWhenDaisyHitAppearsAgain; // unsure
    /*0x16*/ s16 _16;
    /*0x18*/ s16 _18;
    /*0x1A*/ s16 regularHitGamePause;
    /*0x1C*/ s16 captainStarHitGamePause;
    /*0x1E*/ s16 moonshotGamePause;
} HitShorts;

extern HitShorts g_hitShorts;
/*
= {
    180, // frameChargeDownEnds
    12,  // framesUntilChargeIsEnabled
    6,   // _04
    3,   // framesOnGroundBeforeCheckingMinStarVelo
    90,  // fireballStunFrames
    60,  // unused_alternateFireballStunFrames
    2,   // NEggBounces
    30,  // _0C
    120, // framesBeforeGroundWhenGarlicSplits
    1,   // FrameWhenPeachHitAppearsAgain
    1,   // FrameWhenDaisyHitAppearsAgain
    160, // _16
    160, // _18
    1,   // regularHitGamePause
    50,  // captainStarHitGamePause
    50   // moonshotGamePause
};
*/

typedef enum _GAME_TYPE {
    GAME_TYPE_EXHIBITION_GAME = 0,
    GAME_TYPE_PRACTICE = 2,
    GAME_TYPE_DEMO = 4,
    GAME_TYPE_CHALLENGE = 5,
    GAME_TYPE_TOY_FIELD = 6,
    GAME_TYPE_MINIGAMES = 7,
} GAME_TYPE;

typedef enum _STADIUM_ID {
    STADIUM_ID_MARIO_STADIUM,
    STADIUM_ID_BOWSERS_CASTLE,
    STADIUM_ID_WARIO_PALACE,
    STADIUM_ID_YOHSI_PARK,
    STADIUM_ID_PEACH_GARDEN,
    STADIUM_ID_DK_JUNGLE,
    STADIUM_ID_TOY_FIELD,
} STADIUM_ID;

typedef struct _GameInitVariables {
    /*0x00*/ int _00;
    /*0x04*/ u16 FrameCountWhileNotAtMainMenu;
    /*0x06*/ u8 _06; // 1 Menu,2 InGame/Minigame/Practice/etc
    /*0x07*/ E(u8, GAME_TYPE) GameModeSelected;
    /*0x08*/ bool exhibitionMatchInd;
    /*0x09*/ E(u8, STADIUM_ID) StadiumID;
    /*0x0A*/ u8 miniGameStadiumIndicator;
    /*0x0B*/ u8 _0B;
    /*0x0C*/ u8 maybeHomeAway;
    /*0x0D*/ u8 maybeHomeAway2;
    /*0x0E*/ u8 home_AwaySetting;
    artificial_padding(0x0e, 0x11, u8);
    /*0x11*/ bool minigamesEnabled;
    artificial_padding(0x11, 0x20, bool);
    /*0x20*/ s16 _20[4][2];
    /*0x30*/ s16 challengeMinigame_baseCoinsEarned;
    /*0x32*/ u8 bJMatchRelated;
    /*0x33*/ u8 _33;
    /*0x34*/ u8 humanTeamNumber;
    /*0x35*/ u8 _35;
    /*0x36*/ u8 _36;
    /*0x37*/ u8 challengeDifficulty; // unsure
    /*0x38*/ u8 _38;
    /*0x39*/ u8 bJMatchInd;
    /*0x3A*/ u8 _3A;
    /*0x3B*/ u8 someChallengeModeFlag;
    /*0x3C*/ u8 challengeCaptainStarBought[18];
    /*0x4E*/ u8 _4E;
    /*0x4F*/ u8 _4F;
    /*0x50*/ u8 PlayerPorts[2];
    /*0x52*/ u8 _52;
    /*0x53*/ u8 _53;
    /*0x54*/ E(u8, STADIUM_ID) _54;
    /*0x55*/ u8 _55;
    /*0x56*/ u8 _56;
    /*0x57*/ u8 _57;
} GameInitVariables; // size: 0x58

extern GameInitVariables gameInitVariables;

typedef struct _GameControlOptions
{
    /*0x000*/ bool autoRunning;
    /*0x001*/ bool autoFielding;
    /*0x002*/ bool dropSpot;
    /*0x003*/ bool _3;
    /*0x004*/ bool easyBatting;
    /*0x005*/ bool _5;
    /*0x006*/ bool _6;
} GameControlOptions; // size: 0x7


typedef struct _GameInitOptions
{
    /*0x000*/ u8 inningSetting;
    /*0x001*/ u8 _1;
    /*0x002*/ u8 _2;
    /*0x003*/ u8 _3;
    /*0x004*/ u8 starSkillsSetting;
    /*0x005*/ u8 runsNeededForMercy;
    /*0x006*/ u8 _6;
    /*0x007*/ u8 _7;
    /*0x008*/ u8 _8;
    /*0x009*/ GameControlOptions controlOptions[4];
    /*0x025*/ u8 _25;
    /*0x026*/ u8 _26;
    /*0x027*/ u8 _27;
    /*0x028*/ u8 _28;
    /*0x029*/ u8 _29;
    /*0x02A*/ s16 _2A;
    /*0x02C*/ s16 _2C;
    /*0x02E*/ s16 _2E;
    /*0x030*/ s16 _30;
    /*0x032*/ u8 _32;
    /*0x033*/ u8 _33;
    /*0x034*/ u8 _34;
    /*0x035*/ u8 _35;
    /*0x036*/ u8 _36;
    /*0x037*/ u8 _37;
    /*0x038*/ u8 _38;
    /*0x039*/ u8 _39;
    /*0x03A*/ u8 _3A;
    /*0x03B*/ u8 _3B;
    /*0x03C*/ u8 _3C;
    /*0x03D*/ u8 _3D;
    /*0x03E*/ u8 _3E;
    /*0x03F*/ u8 _3F;
    /*0x040*/ u8 _40;
    /*0x041*/ u8 _41;
    /*0x042*/ u8 _42;
    /*0x043*/ u8 _43;
    /*0x044*/ u8 _44;
    /*0x045*/ u8 _45;
    /*0x046*/ u8 _46;
    /*0x047*/ u8 _47;
    /*0x048*/ u8 _48;
    /*0x049*/ u8 _49;
    /*0x04A*/ u8 _4A;
    /*0x04B*/ u8 _4B;
    /*0x04C*/ u8 _4C;
    /*0x04D*/ u8 _4D;
    /*0x04E*/ u8 _4E;
    /*0x04F*/ u8 _4F;
    /*0x050*/ u8 _50;
    /*0x051*/ u8 _51;
    /*0x052*/ u8 _52;
    /*0x053*/ u8 _53;
    /*0x054*/ u8 _54;
    /*0x055*/ u8 _55;
    /*0x056*/ u8 _56;
    /*0x057*/ u8 _57;
    /*0x058*/ u8 _58;
    /*0x059*/ u8 _59;
    /*0x05A*/ u8 _5A;
    /*0x05B*/ u8 _5B;
    /*0x05C*/ u8 _5C;
    /*0x05D*/ u8 _5D;
    /*0x05E*/ u8 _5E;
    /*0x05F*/ u8 _5F;
} GameInitOptions; // size: 0x60

extern GameInitOptions gameInitOptions;

typedef struct _InputStruct {
    /*0x00*/ sAng controlStickAngle;
    /*0x02*/ s16 controlStickMagnitude;
    /*0x04*/ s16 buttonInput;    // uses pad defines
    /*0x06*/ s16 newButtonInput; // pressed this frame
    /*0x08*/ s16 _08;
    /*0x0A*/ s8 right_left;
    /*0x0B*/ s8 up_down;
    /*0x0C*/ s8 rightTriggerDistance;
    /*0x0D*/ s8 leftTriggerDistance;
    /*0x0E*/ s16 _0E; // padding
} InputStruct;        // size: 0x10

typedef enum _PRACTICE_TYPE {
    PRACTICE_TYPE_PITCHING,
    PRACTICE_TYPE_BATTING,
    PRACTICE_TYPE_FIELDING,
    PRACTICE_TYPE_BASERUNNING,
    PRACTICE_TYPE_FREEPLAY
} PRACTICE_TYPE;

typedef enum _PRACTICE_STATE {
    PRACTICE_STATE_0,
    PRACTICE_STATE_1,
    PRACTICE_STATE_2,
    PRACTICE_STATE_3,
    PRACTICE_STATE_4,
    PRACTICE_STATE_5,
    PRACTICE_STATE_6,
    PRACTICE_STATE_7,
    PRACTICE_STATE_8
    /*
        0_switchToTopMenu4/switchToSubMenu+loadfield,goTo1/newAB?=0,
        1_/checkSomething,delay,goTo2=1,
        2_changeScene,goTo3/subMenuControl=2,
        3_delay30Frames,goTo4/switchToField,goTo4=3,
        4_mainMenuControl/loadPracticeOnField=4,
        5_checkSomething,goTo6/returnToTopMenu,goTo6=5,
        6_setPracticeConsts,goTo0/returnToTopMenu2,goTo7=6,
        7_exitToMenuQuestion/returnToTopMenu3,goTo0=7,
        8_exitToMainMenu=8
    */
} PRACTICE_STATE;

typedef enum _TUTORIAL_STATE {
    TUTORIAL_STATE_0,
    TUTORIAL_STATE_1,
    TUTORIAL_STATE_2,
    TUTORIAL_STATE_3,
    /*
        menu/na=0,
        givingInstruction=1,
        transitionToPlayerControl=2,
        playerHasControl=3
    */
} TUTORIAL_STATE;

typedef struct _PracticeStruct {
    /*0x000*/ InputStruct inputs[2];
    /*0x020*/ void* commandList; // unsure
    /*0x024*/ int homeAway;
    artificial_padding(0x24, 0x140, int);
    /*0x140*/ frame_t totalFrames;
    /*0x142*/ frame_t framesInCurrTransitionState;
    /*0x144*/ frame_t framesSincePracticeMenuDefaultTransition;
    /*0x146*/ frame_t practiceMenu_framesOnCurrMenuScreen;
    /*0x148*/ frame_t frames_sinceMovedToFromMenu;
    /*0x14A*/ frame_t _14A;
    /*0x14C*/ frame_t frames_sinceTimeCalled;
    /*0x14E*/ frame_t frames_onPauseScreen;
    /*0x150*/ frame_t frames_onPauseScreen2; // used For Allowing Cursor To Move
    /*0x152*/ frame_t _152;                  // count To 60 then Set Hit Vars For Fielding Practice
    /*0x154*/ frame_t _154;
    /*0x156*/ frame_t _156;
    /*0x158*/ frame_t _158;
    /*0x15A*/ frame_t someCharID1;
    /*0x15C*/ frame_t someCharID2;
    /*0x15E*/ frame_t someCharID3;
    /*0x160*/ frame_t someCharID4;
    /*0x162*/ frame_t framesOnAllInstructions; // unsure
    /*0x164*/ frame_t _164;
    /*0x166*/ frame_t framesOnCurrInstruction;
    /*0x168*/ frame_t laukituTextChannelIndex;
    /*0x16A*/ frame_t lakituTextIndex;
    /*0x16C*/ frame_t lakituTextIndex_stored;
    /*0x16E*/ frame_t diagramTextChannelIndex;
    /*0x170*/ frame_t diagramTitleTextIndex;
    /*0x172*/ frame_t diagramTitleTextIndex_stored;
    /*0x174*/ frame_t cpuCommandDuration;
    /*0x176*/ frame_t maybeInputResetCountdown;
    /*0x178*/ frame_t maybePreviousInput;
    /*0x17A*/ frame_t _17A;
    /*0x17C*/ frame_t cpu_inputDuration;
    /*0x17E*/ frame_t _17E;
    /*0x180*/ frame_t practice_hitHorizontalPower;
    /*0x182*/ frame_t practice_hitVerticalAngle;
    /*0x184*/ frame_t practice_hitHorizontalAngle;
    /*0x186*/ frame_t _186;
    /*0x188*/ frame_t _188;
    /*0x18A*/ s16 maybeCommandData[4]; // unsure
    /*0x192*/ u8 _192;
    /*0x193*/ E(u8, PRACTICE_TYPE) practiceType_2;
    /*0x194*/ u8 practiceLevel;
    /*0x195*/ E(u8, PRACTICE_STATE) practiceState;
    /*0x196*/ E(u8, TUTORIAL_STATE) tutorialState;
    /*0x197*/ u8 _197;
    /*0x198*/ E(u8, PRACTICE_TYPE) practiceType_1;
    /*0x199*/ E(u8, PRACTICE_TYPE) practiceType;
    /*0x19A*/ u8 subMenuCursor;
    artificial_padding(0x19a, 0x19e, u8);
    /*0x19E*/ u8 pauseMenuLoading; // unsure
    artificial_padding(0x19e, 0x1a1, u8);
    /*0x1A1*/ u8 aIEnabled;
    /*0x1A2*/ u8 practiceBatterHandedness; // unsure
    /*0x1A3*/ u8 freePracticeInd_writeOnly;
    artificial_padding(0x1a3, 0x1aa, u8);
    /*0x1AA*/ u8 transitioningIndicator; // unsure
    artificial_padding(0x1aa, 0x1af, u8);
    /*0x1AF*/ u8 returnToPracticeMenuState;
    artificial_padding(0x1af, 0x1c2, u8);
    /*0x1C2*/ s8 instructionNumber;
    /*0x1C3*/ u8 readyToMoveToNextInstruction; // unsure
    /*0x1C4*/ u8 practice_runner_countInputForMashing;
    /*0x1C5*/ u8 practice_fielding_enableSprinting;
    /*0x1C6*/ u8 _1C6;
    /*0x1C7*/ u8 _1C7;
    /*0x1C8*/ u8 guidedPracticeCompletionRelated;
    /*0x1C9*/ u8 guidedPracticeCompletionRelated2;
    /*0x1CA*/ u8 guidedPracticeCounter;
    /*0x1CB*/ u8 _1CB;
    /*0x1CC*/ u8 currentMessageDoneTyping;
    /*0x1CD*/ u8 commandIndex;
    /*0x1CE*/ u8 allInstructionsComplete;
    /*0x1CF*/ u8 instructionComplete_readyToAdvance;
    /*0x1D0*/ u8 allowPlayToEndIndicator;
    /*0x1D1*/ u8 maybeControlFlag1;
    /*0x1D2*/ u8 textRelatedIndicator;
    /*0x1D3*/ u8 maybeControlFlag2;
    /*0x1D4*/ u8 loadingGuidedPractice; // unsure
    /*0x1D5*/ u8 _1D5;
    /*0x1D6*/ u8 practiceLevel_2;
    artificial_padding(0x1d6, 0x1e1, u8);
    /*0x1E1*/ u8 hitVariablesSetIndicator;
    /*0x1E2*/ u8 _1E2;
    /*0x1E3*/ u8 aiBuntIndicator; // unsure
    artificial_padding(0x1e3, 0x1ef, u8);
    /*0x1EF*/ u8 rosterID; // unsure
} PracticeStruct;          // size: 0x1f0

extern PracticeStruct practiceStruct;

extern u8 buntPower[5][2];

extern f32 s_ballCurveData[2][3];
/*
 =
{
    {
        0.5f, 0.001f, 0.003f
    },
    {
        0.25f, 0.006f, 0.008f
    }
}
*/

extern const f32 contactQual; // = 100.f;

#define ANG4K_DEG(v) (s16)((v) * (360.f / 4096.f))
#define ANG4K_RAD(v) (s16)((v) * ((2.f * PI) / 4096.f))

typedef enum _FIELDING_ABILITY {
    FIELDING_ABILITY_WALL_SPLAT,
    FIELDING_ABILITY_WALL_JUMP,
    FIELDING_ABILITY_WALL_CLAMBER,
    FIELDING_ABILITY_SLIDING_CATCH,
    FIELDING_ABILITY_LASER,
    FIELDING_ABILITY_QUICK_THROW,
    FIELDING_ABILITY_SUPER_JUMP,
    FIELDING_ABILITY_MAGICAL_CATCH,
    FIELDING_ABILITY_TONGUE_CATCH,
    FIELDING_ABILITY_SUCTION,
    FIELDING_ABILITY_SUPER_CATCH,
    FIELDING_ABILITY_BALL_DASH,
    FIELDING_ABILITY_BODY_CHECK,
    FIELDING_ABILITY_SUPER_CURVE,
} FIELDING_ABILITY;

typedef enum _GAME_STATUS {
    GAME_STATUS_DEFAULT,
    GAME_STATUS_AT_BAT,
    GAME_STATUS_LIVE_BALL,
    GAME_STATUS_INNING_TRANSITION,
    GAME_STATUS_LOAD_GAME,
    GAME_STATUS_GAME_START_MOVIE,
    GAME_STATUS_TRANSITION_TO_MINIGAME_START,
    GAME_STATUS_TRANSITION_PREPARE_NEXT_PLAY,
    GAME_STATUS_TRANSITION,
    GAME_STATUS_END_OF_GAME, // unsure
    GAME_STATUS_0xA,
    GAME_STATUS_PAUSED,
    GAME_STATUS_0xC,
    GAME_STATUS_HOW_TO_PLAY_SCREEN,
    GAME_STATUS_MVP_END_GAME,
    GAME_STATUS_TRANSITION_MINIGAME_POSTGAME,
    GAME_STATUS_0x10,
    GAME_STATUS_0x11,
    GAME_STATUS_0x12,
    GAME_STATUS_HOMERUN_END,
    GAME_STATUS_HOMERUN_LAP,
    GAME_STATUS_BATTER_CELEBRATION, // post replay
    GAME_STATUS_STAR_CHANCE_VS,
    GAME_STATUS_CHAMPIONSHIP,
    GAME_STATUS_0x18,
    GAME_STATUS_MINIGAME_NEW_ROUND,             // unsure
    GAME_STATUS_TRANSITION_MINIGAME_TO_BATTING, // unsure
    GAME_STATUS_0x1B,
    GAME_STATUS_MINIGAME_SELECT,
    GAME_STATUS_TOY_STADIUM_LOAD,
    GAME_STATUS_TOY_STADIUM_CHARACTER_SELECT,
    GAME_STATUS_0x1F,
    GAME_STATUS_0x20,
    GAME_STATUS_MINIGAME_READY,
    GAME_STATUS_MINIGAME_POST_MENU,
} GAME_STATUS;

typedef enum _SCENE_ID {
    SCENE_ID_0,
    SCENE_ID_AT_BAT,
    SCENE_ID_LIVE_BALL,
    SCENE_ID_REPLAY_AT_BAT,
    SCENE_ID_REPLAY_LIVE_BALL,
} SCENE_ID;

typedef enum _SECONDARY_GAME_MODE {
    SECONDARY_GAME_MODE_NONE, // also exhibition
    SECONDARY_GAME_MODE_TOY_FIELD,
    SECONDARY_GAME_MODE_MINI_GAME_MENU,
    SECONDARY_GAME_MODE_BOBOMB_DERBY,
    SECONDARY_GAME_MODE_WALLBALL,
    SECONDARY_GAME_MODE_BARREL_BATTER,
    SECONDARY_GAME_MODE_CHAINCHOMP_SPRINT,
    SECONDARY_GAME_MODE_PIRANHA_PANIC,
    SECONDARY_GAME_MODE_STAR_DASH,
    SECONDARY_GAME_MODE_0x9,
    SECONDARY_GAME_MODE_PRACTICE_MENU,
    SECONDARY_GAME_MODE_PRACTICE_PITCHING,
    SECONDARY_GAME_MODE_PRACTICE_BATTING,
    SECONDARY_GAME_MODE_PRACTICE_FIELDING,
    SECONDARY_GAME_MODE_PRACTICE_BASERUNNING,
    SECONDARY_GAME_MODE_FREE_BAT_AND_RUNNING,
    SECONDARY_GAME_MODE_FREE_FIELDING,
    SECONDARY_GAME_MODE_RETURN_TO_MENU,
    SECONDARY_GAME_MODE_LOAD_PRACTICE_SCREEN,
} SECONDARY_GAME_MODE;

typedef enum _TRANSITION_CALCULATION_TYPE {
    TRANSITION_CALCULATION_TYPE_0,
    TRANSITION_CALCULATION_TYPE_1,
    TRANSITION_CALCULATION_TYPE_2,
    TRANSITION_CALCULATION_TYPE_3,
    TRANSITION_CALCULATION_TYPE_4,
    TRANSITION_CALCULATION_TYPE_5,
    TRANSITION_CALCULATION_TYPE_6,
    TRANSITION_CALCULATION_TYPE_7,
    TRANSITION_CALCULATION_TYPE_8,
    TRANSITION_CALCULATION_TYPE_9,
    TRANSITION_CALCULATION_TYPE_10,
    TRANSITION_CALCULATION_TYPE_11,
} TRANSITION_CALCULATION_TYPE;

typedef enum _WIN_TYPE {
    WIN_TYPE_0,
    WIN_TYPE_1,
    WIN_TYPE_2,
    WIN_TYPE_3,
    WIN_TYPE_4,
    WIN_TYPE_5,
    WIN_TYPE_6,
} WIN_TYPE;

extern bool checkFieldingStat(int, int, E(int, FIELDING_ABILITY));

typedef struct _LogoInfoStruct {
    int ID;
    int variationID;
    int captain;  // unsure
} LogoInfoStruct; // size: 0xC

typedef struct _GameControlsStruct {
    /*0x000*/ s16 homeTeamInd[2];
    /*0x004*/ int teamBatting;
    /*0x008*/ int teamFielding;
    /*0x00C*/ int homeTeamBattingInd_fieldingTeam;
    /*0x010*/ int awayTeamBattingInd_battingTeam;
    /*0x014*/ u32 AIDifficulty0Special3Weak[2];
    artificial_padding(0x14, 0x24, u32[2]);
    /*0x024*/ LogoInfoStruct logo[2];
    /*0x03C*/ int battingOrderAndPositionMapping[2][10][2]; // first Pair Is Pitcher
    /*0x0DC*/ int currentBatterPerTeam[2];
    /*0x0E4*/ u32 runnerAIInd[2];
    /*0x0EC*/ u32 teams[2]; // vague name, unsure
    /*0x0F4*/ u32 Team_CaptainRosterLoc[2];
    /*0x0FC*/ s16 FrameCountOfCurrentPitch;
    /*0x0FE*/ s16 FrameCountOfCurrentAtBat_Copy;
    /*0x100*/ s16 CountdownUntilFade;
    /*0x102*/ s16 _102;
    /*0x104*/ s16 framePlayEnd;
    /*0x106*/ s16 _106;
    /*0x108*/ s16 _108;
    /*0x10A*/ s16 bOD_framesInLiveBallScene;
    /*0x10C*/ s16 frameCountdownAtBeginningOfAtBatLockout;
    /*0x10E*/ s16 scoreBook_teamDisplayed;
    /*0x110*/ s16 scoreBook_batter_pitcherStatsDisplayed;
    /*0x112*/ s16 scoreBook_scrollIndex;
    /*0x114*/ s16 _114;
    /*0x116*/ s16 scoreBook_logoFadeDirectionLeft_Right;
    /*0x118*/ s16 _118;
    /*0x11A*/ s16 _11A;
    /*0x11C*/ s16 _11C;
    /*0x11E*/ E(u8, GAME_STATUS) gameStatus;
    /*0x11F*/ E(u8, GAME_STATUS) gameStatus_prev;
    /*0x120*/ E(u8, SCENE_ID) sceneID;
    /*0x121*/ E(u8, SECONDARY_GAME_MODE) secondaryGameMode;
    /*0x122*/ u8 framesOfExitingToMenu;
    /*0x123*/ u8 _123;
    /*0x124*/ u8 _124;
    /*0x125*/ E(u8, TRANSITION_CALCULATION_TYPE) _125;
    /*0x126*/ u8 homeRunWordAnimationCompletedInd;
    /*0x127*/ u8 EventTriggers_EndOfGame;
    /*0x128*/ u8 _128;
    /*0x129*/ u8 EventTriggers_GameHasStarted; // unsure
    /*0x12A*/ u8 playBatterWalkupAnimation;
    /*0x12B*/ u8 pre_PostMiniGameInd;        // unsure
    /*0x12C*/ u8 minigameLastTurnSuccessInd; // unsure
    /*0x12D*/ u8 hudElementLoadingInd;
    /*0x12E*/ u8 hudLoadingRelated; // unsure
    /*0x12F*/ _12F;
    /*0x130*/ E(u8, WIN_TYPE) winType;
    artificial_padding(0x130, 0x135, u8);
    /*0x135*/ u8 _135;
    /*0x136*/ u8 _136;
    /*0x137*/ u8 playOverFadeOutStarted;
    /*0x138*/ u8 playOver;
    /*0x139*/ u8 playOverInd;
    /*0x13A*/ u8 walkOffWinInd; // unsure
    /*0x13B*/ u8 gameOverInd;
    /*0x13C*/ u8 scoutFlag_VsScreenInd;
    artificial_padding(0x13C, 0x142, u8);
    /*0x142*/ u8 teamAIInd[2];
    /*0x144*/ u8 autoFielding[2];
    /*0x146*/ u8 batterHandedness[2];
    /*0x148*/ u8 battingAIInd[2]; // unsure
    /*0x14A*/ u8 TeamStars[2];
    /*0x14C*/ u8 IsStarChance;
    /*0x14D*/ u8 stadiumStarObtained;
    /*0x14E*/ u8 PauseSimulationFrameCount;
    /*0x14F*/ u8 freeFieldingPracticeInd;
    /*0x150*/ frame_t frame_exitMenuShowing;
    /*0x152*/ frame_t frames_memoryCardWriteOnMVP; // unsure
    /*0x154*/ u8 endGameStage;
    /*0x155*/ u8 rosterLoc_skippingCap; // unsure
    /*0x156*/ u8 _156;
    /*0x157*/ u8 _157;
} GameControlsStruct;                   // size: 0x158

extern GameControlsStruct gameControls;

typedef enum _CHALLENGE_CAPTAIN {
    CHALLENGE_CAPTAIN_MARIO,
    CHALLENGE_CAPTAIN_PEACH,
    CHALLENGE_CAPTAIN_WARIO,
    CHALLENGE_CAPTAIN_DK,
    CHALLENGE_CAPTAIN_YOSHI,
    CHALLENGE_CAPTAIN_BOWSER,
} CHALLENGE_CAPTAIN;

typedef enum _CHARACTER_VARIANT_CLASSIFICATION {
    CHARACTER_VARIANT_CLASSIFICATION_PRIMARY_CAPTAIN,
    CHARACTER_VARIANT_CLASSIFICATION_SECONDARY_CAPTAIN,
    CHARACTER_VARIANT_CLASSIFICATION_UNIQUE_NON_CAPTAIN,
    CHARACTER_VARIANT_CLASSIFICATION_PRIMARY_VARIANT,
    CHARACTER_VARIANT_CLASSIFICATION_SECONDARY_VARIANT,
} CHARACTER_VARIANT_CLASSIFICATION;

typedef enum _STAR_MISSION_TRACKING {
    STAR_MISSION_TRACKING_NOT_COMPLETED = 0,
    STAR_MISSION_TRACKING_TRACK1_COMPLETION = 1,
    STAR_MISSION_TRACKING_COMPLETED_SAVED = 254,
    STAR_MISSION_TRACKING_COMPLETED_CONDITIONALLY = 255,
} STAR_MISSION_TRACKING;

typedef enum _CHALLENGE_RECRUITMENT_CD {
    CHALLENGE_RECRUITMENT_CD_NOT_RECRUITED,
    CHALLENGE_RECRUITMENT_CD_RECRUITED,
    CHALLENGE_RECRUITMENT_CD_ON_BJ_TEAM,
} CHALLENGE_RECRUITMENT_CD;

typedef enum _FIELDING_ABILITIES {
    FIELDING_ABILITIES_WALL_SPLAT = 0x1,
    FIELDING_ABILITIES_WALL_JUMP = 0x2,
    FIELDING_ABILITIES_CLAMBER = 0x4,
    FIELDING_ABILITIES_SLIDING_CATCH = 0x8,
    FIELDING_ABILITIES_LASER = 0x10,
    FIELDING_ABILITIES_QUICK_THROW = 0x20,
    FIELDING_ABILITIES_SUPER_JUMP = 0x40,
    FIELDING_ABILITIES_MAGICAL_CATCH = 0x80,
    FIELDING_ABILITIES_TONGUE_CATCH = 0x100,
    FIELDING_ABILITIES_SUCTION = 0x200,
    FIELDING_ABILITIES_SUPER_CATCH = 0x400,
    FIELDING_ABILITIES_BALL_DASH = 0x800,
    FIELDING_ABILITIES_BODY_CHECK = 0x1000,
    FIELDING_ABILITIES_SUPER_CURVE = 0x2000,
    FIELDING_ABILITIES_UNASSIGNED = 0x4000,
    FIELDING_ABILITIES_UNASSIGNED2 = 0x8000,
} FIELDING_ABILITIES;

typedef struct _starMissionTrackingPair {
    /*0x000*/ E(u8, STAR_MISSION_TRACKING) starMissionStatus;
    /*0x001*/ u8 shownOnPauseMenu;
} starMissionTrackingPair;

// This is pack disabled in ghidra.
typedef struct _ChallengeTrackingStruct {
    /*0x000*/ void* scoutFlagPointer;
    /*0x004*/ E(u8, CHALLENGE_CAPTAIN) challengeCaptain; // Created by retype action
    /*0x005*/ E(s8, CHARACTER_VARIANT_CLASSIFICATION) variantClassification;
    /*0x006*/ u8 scoutFlagsAchieved;
    /*0x007*/ u8 _7;
    /*0x008*/ u8 _8;
    /*0x009*/ starMissionTrackingPair inGameMissionTracker[10];
    /*0x01D*/ starMissionTrackingPair saveFileMissionTracker[10];
    /*0x031*/ E(u8, CHALLENGE_RECRUITMENT_CD) _31;
    /*0x032*/ u8 _32;
    /*0x033*/ u8 _33;
} ChallengeTrackingStruct; // size: 0x34

typedef struct _StatTable {
    /*0x000*/ u8 CurveBallSpeed;
    /*0x001*/ u8 FastBallSpeed;
    /*0x002*/ u8 cursedBall;
    /*0x003*/ u8 Curve;
    /*0x004*/ u8 curveControl;
    /*0x005*/ u8 UnusedBytes[27];
    /*0x020*/ E(u32, FIELDING_ABILITIES) FieldingStats;
    /*0x024*/ s16 CharID;
    /*0x026*/ u8 FieldingArm;
    /*0x027*/ u8 BattingStance;
    /*0x028*/ u8 SlapContactSize;
    /*0x029*/ u8 ChargeContactSize;
    /*0x02A*/ u8 SlapHitPower;
    /*0x02B*/ u8 ChargeHitPower;
    /*0x02C*/ u8 BuntingContactSize;
    /*0x02D*/ u8 HitTrajectoryPushPull;
    /*0x02E*/ u8 HitTrajectoryHighLow;
    /*0x02F*/ u8 Speed;
    /*0x030*/ u8 ThrowingArm;
    /*0x031*/ E(u8, CHARACTER_CLASS) CharacterClass;
    /*0x032*/ u8 Weight;
    /*0x033*/ u8 Captain;
    /*0x034*/ u8 CaptainStarHitPitch;
    /*0x035*/ u8 NonCaptainStarSwing;
    /*0x036*/ u8 NonCaptainStarPitch;
    /*0x037*/ u8 BattingStatBar;
    /*0x038*/ u8 PitchingStatBar;
    /*0x039*/ u8 RunningStatBar;
    /*0x03A*/ u8 FieldingStatBar;
} StatTable; // size: 0x3B

typedef struct _ChemistryTable {
    /*0x000*/ u8 Mario;
    /*0x001*/ u8 Luigi;
    /*0x002*/ u8 DK;
    /*0x003*/ u8 Diddy;
    /*0x004*/ u8 Peach;
    /*0x005*/ u8 Daisy;
    /*0x006*/ u8 Yoshi;
    /*0x007*/ u8 BabyMario;
    /*0x008*/ u8 BabyLuigi;
    /*0x009*/ u8 Bowser;
    /*0x00A*/ u8 Wario;
    /*0x00B*/ u8 Waluigi;
    /*0x00C*/ u8 RedKoopa;
    /*0x00D*/ u8 RedToad;
    /*0x00E*/ u8 Boo;
    /*0x00F*/ u8 Toadette;
    /*0x010*/ u8 RedShyGuy;
    /*0x011*/ u8 Birdo;
    /*0x012*/ u8 Monty;
    /*0x013*/ u8 BowserJr;
    /*0x014*/ u8 RedParatroopa;
    /*0x015*/ u8 BluePianta;
    /*0x016*/ u8 RedPianta;
    /*0x017*/ u8 YellowPianta;
    /*0x018*/ u8 BlueNoki;
    /*0x019*/ u8 RedNoki;
    /*0x01A*/ u8 GreenNoki;
    /*0x01B*/ u8 HammerBro;
    /*0x01C*/ u8 Toadsworth;
    /*0x01D*/ u8 BlueToad;
    /*0x01E*/ u8 YellowToad;
    /*0x01F*/ u8 GreenToad;
    /*0x020*/ u8 PurpleToad;
    /*0x021*/ u8 BlueMagikoopa;
    /*0x022*/ u8 RedMagikoopa;
    /*0x023*/ u8 GreenMagikoopa;
    /*0x024*/ u8 YellowMagikoopa;
    /*0x025*/ u8 KingBoo;
    /*0x026*/ u8 Petey;
    /*0x027*/ u8 Dixie;
    /*0x028*/ u8 Goomba;
    /*0x029*/ u8 Paragoomba;
    /*0x02A*/ u8 GreenKoopa;
    /*0x02B*/ u8 GreenParatroopa;
    /*0x02C*/ u8 BlueShyGuy;
    /*0x02D*/ u8 YellowShyGuy;
    /*0x02E*/ u8 GreenShyGuy;
    /*0x02F*/ u8 BlackShyGuy;
    /*0x030*/ u8 GrayDryBones;
    /*0x031*/ u8 GreenDryBones;
    /*0x032*/ u8 RedDryBones;
    /*0x033*/ u8 BlueDryBones;
    /*0x034*/ u8 FireBro;
    /*0x035*/ u8 BoomerangBro;
} ChemistryTable; // size: 0x36

typedef struct _CharacterStats {

    /*0x000*/ StatTable stats;
    /*0x03B*/ ChemistryTable chemistry;
    /*0x071*/ u8 BytesAfterChemistry[3];
    /*0x074*/ s16 UnusedShorts[21];
} CharacterStats;

extern CharacterStats inMemRoster[TEAMS_PER_GAME][PLAYERS_PER_TEAM];

typedef struct _UnkSimulationRelatedStruct
{
    /*0x00*/ u32 _00; // frameCountRelated
    /*0x04*/ u8 _04; // loadingStateRelated?
    /*0x05*/ u8 _05; // highLevelSimulationIndicator
    /*0x06*/ u8 _06; // highLevelSimulationSomething
    /*0x07*/ u8 _07;
    /*0x08*/ u8 _08; // endDemo?
    /*0x09*/ u8 _09;
    // padding 2 bytes
} UnkSimulationRelatedStruct; // size: 0xC

extern UnkSimulationRelatedStruct unkSimulationRelatedStruct;

extern f32 vecDotProduct(VecXYZ* a, VecXYZ* b);

extern ChallengeTrackingStruct starMissionCompletionTracker[54];

typedef struct _SomeStarDashStruct
{
    /*0x000*/ s16 _0[2];
    /*0x004*/ u8 _4[6];
    /*0x00A*/ u8 _pad_A[2]; // 2 bytes of padding for f32 alignment
    /*0x00C*/ f32 _A;
    /*0x00E*/ u8 pad[24];
} SomeStarDashStruct; // size: 0x28

typedef struct _UnkStarDashFloatStruct
{
    /*0x000*/ f32 _0;
    /*0x004*/ f32 _4;
    /*0x008*/ f32 _8;
    /*0x00C*/ f32 _C;
    /*0x010*/ f32 _10;
    /*0x014*/ f32 _14;
    /*0x018*/ f32 _18;
    /*0x01C*/ f32 _1C;
    /*0x020*/ s16 _20;
} UnkStarDashFloatStruct; // size: 0x22

typedef struct _MaybeWallBallStruct
{
    /*0x000*/ f32 _0; // start of wall ball struct?
    /*0x004*/ f32 _4;
    /*0x008*/ f32 zPositionOfSomeWall;
    /*0x00C*/ f32 _C;
    /*0x010*/ f32 _10;
    /*0x014*/ f32 _14;
    /*0x018*/ f32 _18;
    /*0x01C*/ f32 _1C;
    /*0x020*/ f32 _20;
    /*0x024*/ s16 _24;
    /*0x026*/ s16 _26;
    /*0x028*/ u8 _28;
    /*0x029*/ u8 coinGenerationCategory;
    /*0x02A*/ u8 coinRelated[26];
} MaybeWallBallStruct; // size: 0x44

typedef struct _BB_barrelStruct
{
    /*0x000*/ VecXYZ currentPos;
    /*0x00C*/ VecXYZ desiredPos;
    /*0x018*/ VecXYZ posAtStartOfTurn;
    /*0x024*/ f32 velo_bounceUpOnBlowUp;
    /*0x028*/ f32 _28;
    /*0x02C*/ s16 animationCounter;
    /*0x02E*/ s16 delayUntilBlownUp;
    /*0x030*/ u8 barrelState;
    /*0x031*/ u8 barrelColour;
    /*0x032*/ u8 replacingBarrelInd;
    /*0x033*/ u8 pad;
} BB_barrelStruct; // size: 0x34

typedef struct _UnkStructMinigame
{
    /*0x000*/ VecXYZ _0;
    /*0x00C*/ VecXYZ _C;
    /*0x018*/ s32 _18;
    /*0x01C*/ s32 _1C;
    /*0x020*/ u8 pad4[28];
    /*0x03C*/ s32 _3C;
    /*0x040*/ u8 _40[4];
    /*0x044*/ VecXYZ _44;
    /*0x050*/ VecXYZ _50;
    /*0x05C*/ VecXYZ _5C;
} UnkStructMinigame; // size: 0x68

typedef struct _MiniGameControlStruct
{
    /*0x000*/ s8 characterIndex[4];
    /*0x004*/ u8 _4;
    /*0x005*/ u8 _5;
    /*0x006*/ u8 _6;
    /*0x007*/ u8 _7;
    /*0x008*/ u8 _8;
    /*0x009*/ u8 _9;
    /*0x00A*/ u8 _A;
    /*0x00B*/ u8 _B;
    /*0x00C*/ u8 battingHandedness[4];
    /*0x010*/ u8 aIStrength[4];
    /*0x014*/ u8 _14;
    /*0x015*/ u8 _15;
} MiniGameControlStruct; // size: 0x16

/*foul	1	
caught	2	
groundRuleDouble	4	
homerun	6	*/
typedef enum _TOY_FIELD_RESULT
{
    TOY_FIELD_RESULT_FOUL = 1,
    TOY_FIELD_RESULT_CAUGHT = 2,
    TOY_FIELD_RESULT_GROUND_RULE_DOUBLE = 4,
    TOY_FIELD_RESULT_HOMERUN = 6
} TOY_FIELD_RESULT;

/*None	0	
BobOmbDerby	1	
WallBall	2	
BarrelBatter	3	
ChainChompSprint	4	
PiranhaPanic	5	
StarDash	6	
MarioGrandPrix	7	*/

typedef enum _MINI_GAME_ID
{
    MINI_GAME_ID_NONE = 0,
    MINI_GAME_ID_BOBOMB_DERBY = 1,
    MINI_GAME_ID_WALLBALL = 2,
    MINI_GAME_ID_BARREL_BATTER = 3,
    MINI_GAME_ID_CHAINCHOMP_SPRINT = 4,
    MINI_GAME_ID_PIRANHA_PANIC = 5,
    MINI_GAME_ID_STAR_DASH = 6,
    MINI_GAME_ID_MARIO_GRAND_PRIX = 7
} MINI_GAME_ID;

/*multiplayer/ChallengeEasy	0	
ChallengeNormal	1	
challengeHard	2	
soloNonChallenge	3	*/
typedef enum _MINIGAME_DIFFICULTY
{
    MINIGAME_DIFFICULTY_MULTIPLAYER_CHALLENGE_EASY = 0,
    MINIGAME_DIFFICULTY_MULTIPLAYER_CHALLENGE_NORMAL = 1,
    MINIGAME_DIFFICULTY_MULTIPLAYER_CHALLENGE_HARD = 2,
    MINIGAME_DIFFICULTY_SOLO_NON_CHALLENGE = 3
} MINIGAME_DIFFICULTY;

/*calculateNewWalls	0	
dropInNewWalls	1	
pitchOrWaitingForPitch	2	*/

typedef enum _WALL_BALL_GAME_STATE
{
    WALL_BALL_GAME_STATE_CALCULATE_NEW_WALLS = 0,
    WALL_BALL_GAME_STATE_DROP_IN_NEW_WALLS = 1,
    WALL_BALL_GAME_STATE_PITCH_OR_WAITING_FOR_PITCH = 2
} WALL_BALL_GAME_STATE;

typedef struct _MiniGameStruct
{
    /*0x000*/ u8 _0;
    /*0x001*/ u8 _1;
    /*0x002*/ u8 _2;
    /*0x003*/ u8 _3;
    /*0x004*/ u8 _4;
    /*0x005*/ u8 _5;
    /*0x006*/ u8 _6;
    /*0x007*/ u8 _7;
    /*0x008*/ u8 _8;
    /*0x009*/ u8 _9;
    /*0x00A*/ u8 _A;
    /*0x00B*/ u8 _B;
    /*0x00C*/ u8 _C;
    /*0x00D*/ u8 _D;
    /*0x00E*/ u8 _E;
    /*0x00F*/ u8 _F;
    /*0x010*/ u8 _10;
    /*0x011*/ u8 _11;
    /*0x012*/ u8 _12;
    /*0x013*/ u8 _13;
    /*0x014*/ u8 _14;
    /*0x015*/ u8 _15;
    /*0x016*/ u8 _16;
    /*0x017*/ u8 _17;
    /*0x018*/ u8 _18;
    /*0x019*/ u8 _19;
    /*0x01A*/ u8 _1A;
    /*0x01B*/ u8 _1B;
    /*0x01C*/ s16 _1C;
    /*0x01E*/ s16 _1E;
    /*0x020*/ u8 _20;
    /*0x021*/ u8 _21;
    /*0x022*/ u8 _22;
    /*0x023*/ u8 _23;
    /*0x024*/ u8 _24;
    /*0x025*/ u8 _25;
    /*0x026*/ u8 _26;
    /*0x027*/ u8 _27;
    /*0x028*/ u8 _28;
    /*0x029*/ u8 _29;
    /*0x02A*/ u8 _2A;
    /*0x02B*/ u8 _2B;
    /*0x02C*/ u8 _2C;
    /*0x02D*/ u8 _2D;
    /*0x02E*/ u8 _2E;
    /*0x02F*/ u8 _2F;
    /*0x030*/ u8 _30;
    /*0x031*/ u8 _31;
    /*0x032*/ u8 _32;
    /*0x033*/ u8 _33;
    /*0x034*/ u8 _34;
    /*0x035*/ u8 _35[3];
    /*0x038*/ f32 _38;
    /*0x03C*/ f32 _3C;
    /*0x040*/ f32 _40[4];
    /*0x050*/ s16 _50;
    /*0x052*/ s16 _52;
    /*0x054*/ s16 _54;
    /*0x056*/ s16 _56;
    /*0x058*/ s16 _58;
    /*0x05A*/ s16 _5A[4];
    /*0x062*/ u8 _62;
    /*0x063*/ u8 _63;
    /*0x064*/ u8 _64;
    /*0x065*/ u8 _65;
    /*0x066*/ u8 _66;
    /*0x067*/ u8 _67;
    /*0x068*/ u8 _68;
    /*0x069*/ u8 _69;
    /*0x06A*/ u8 _6A;
    /*0x06B*/ u8 _6B[5];
    /*0x070*/ u8 _70[26];
    /*0x08A*/ s16 _8A[8];
    /*0x09A*/ u8 _9A[14];
    /*0x0A8*/ f32 _A8;
    /*0x0AC*/ f32 _AC;
    /*0x0B0*/ f32 _B0;
    /*0x0B4*/ f32 _B4;
    /*0x0B8*/ f32 _B8;
    /*0x0BC*/ f32 _BC;
    /*0x0C0*/ f32 _C0;
    /*0x0C4*/ s32 _C4;
    
    /*0x0CA*/ SomeStarDashStruct _CA;
    /*0x0F2*/ SomeStarDashStruct _F2;
    /*0x11A*/ SomeStarDashStruct _11A;
    /*0x142*/ SomeStarDashStruct _142;
    /*0x16A*/ SomeStarDashStruct _16A;
    /*0x192*/ SomeStarDashStruct _192;
    /*0x1BA*/ SomeStarDashStruct _1BA;
    /*0x1E2*/ SomeStarDashStruct _1E2;
    /*0x20A*/ SomeStarDashStruct _20A;
    /*0x230*/ s16 _230;
    /*0x232*/ s16 _232[2];
    /*0x236*/ u8 _236[2];
    /*0x238*/ UnkStarDashFloatStruct _238;
    /*0x25A*/ SomeStarDashStruct _25A;
    /*0x282*/ SomeStarDashStruct _282;
    /*0x2AA*/ SomeStarDashStruct _2AA;
    /*0x2D2*/ SomeStarDashStruct _2D2;
    /*0x2FA*/ SomeStarDashStruct _2FA;
    /*0x322*/ SomeStarDashStruct _322;
    /*0x34A*/ u8 pad[2];
    /*0x34E*/ u8 _34E[40];
    /*0x376*/ u8 _376[40];
    /*0x39E*/ u8 _39E[40];
    /*0x3C6*/ u8 _3C6[40];
    /*0x3EE*/ u8 _3EE[40];
    /*0x416*/ u8 _416[40];
    /*0x43E*/ u8 _43E[40];
    /*0x466*/ u8 _466[40];
    /*0x48E*/ u8 _48E[40];
    /*0x4B6*/ u8 _4B6[40];
    /*0x4DE*/ u8 _4DE[40];
    /*0x506*/ u8 _506[40];
    /*0x52E*/ u8 _52E[40];
    /*0x556*/ u8 _556[40];
    /*0x57E*/ u8 _57E[40];
    /*0x5A6*/ u8 _5A6[40];
    /*0x5CE*/ u8 _5CE[40];
    /*0x5F6*/ u8 _5F6[40];
    /*0x61E*/ u8 _61E[40];
    /*0x646*/ u8 _646[40];
    /*0x66E*/ u8 _66E[40];
    /*0x696*/ u8 _696[40];
    /*0x6BE*/ u8 _6BE[40];
    /*0x6E6*/ u8 _6E6[2];
    /*0x6E8*/ f32 _6E8;
    /*0x6EC*/ f32 _6EC;
    /*0x6F0*/ f32 _6F0;
    /*0x6F4*/ f32 _6F4;
    /*0x6F8*/ f32 _6F8;
    /*0x6FC*/ f32 _6FC;
    /*0x700*/ f32 _700;
    /*0x704*/ f32 _704;
    /*0x708*/ f32 _708;
    /*0x70C*/ f32 _70C;
    /*0x710*/ f32 _710;
    /*0x714*/ f32 _714;
    /*0x718*/ u8 pad2[12];
    /*0x724*/ s16 _724;
    /*0x726*/ s16 _726;
    /*0x728*/ s16 _728;
    /*0x72A*/ u8 _72A;
    /*0x72B*/ u8 _72B;
    /*0x72C*/ MaybeWallBallStruct wallBallStruct;
    /*0x770*/ u8 _770[16];
    /*0x780*/ u8 _780;
    /*0x781*/ u8 _781;
    /*0x782*/ u8 _782;
    /*0x783*/ u8 _783;
    /*0x784*/ u8 _784;
    /*0x785*/ u8 _785;
    /*0x786*/ u8 _786;
    /*0x787*/ u8 _787;
    /*0x788*/ u8 _788;
    /*0x789*/ u8 _789;
    /*0x78A*/ u8 _78A;
    /*0x78B*/ u8 _78B;
    /*0x78C*/ u8 _78C;
    /*0x78D*/ u8 _78D;
    /*0x78E*/ u8 _78E;
    /*0x78F*/ u8 _78F;
    /*0x790*/ u8 _790;
    /*0x791*/ u8 _791;
    /*0x792*/ u8 _792;
    /*0x793*/ u8 _793;
    /*0x794*/ u8 _794;
    /*0x795*/ u8 _795;
    /*0x796*/ u8 _796;
    /*0x797*/ u8 _797;
    /*0x798*/ u8 _798;
    /*0x799*/ u8 _799;
    /*0x79A*/ u8 _79A;
    /*0x79B*/ u8 _79B;
    /*0x79C*/ u8 _79C;
    /*0x79D*/ u8 _79D;
    /*0x79E*/ u8 _79E;
    /*0x79F*/ u8 _79F;
    /*0x7A0*/ u8 _7A0;
    /*0x7A1*/ u8 _7A1;
    /*0x7A2*/ u8 _7A2;
    /*0x7A3*/ u8 _7A3;
    /*0x7A4*/ u8 _7A4;
    /*0x7A5*/ u8 _7A5;
    /*0x7A6*/ u8 _7A6;
    /*0x7A7*/ u8 _7A7;
    /*0x7A8*/ u8 _7A8;
    /*0x7A9*/ u8 _7A9;
    /*0x7AA*/ u8 _7AA;
    /*0x7AB*/ u8 _7AB;
    /*0x7AC*/ u8 _7AC;
    /*0x7AD*/ u8 _7AD;
    /*0x7AE*/ u8 _7AE;
    /*0x7AF*/ u8 _7AF;
    /*0x7B0*/ u8 _7B0;
    /*0x7B1*/ u8 _7B1;
    /*0x7B2*/ u8 _7B2;
    /*0x7B3*/ u8 _7B3;
    /*0x7B4*/ u8 _7B4;
    /*0x7B5*/ u8 _7B5;
    /*0x7B6*/ u8 _7B6;
    /*0x7B7*/ u8 _7B7;
    /*0x7B8*/ u8 _7B8;
    /*0x7B9*/ u8 _7B9;
    /*0x7BA*/ u8 _7BA;
    /*0x7BB*/ u8 _7BB;
    /*0x7BC*/ u8 _7BC;
    /*0x7BD*/ u8 _7BD;
    /*0x7BE*/ u8 _7BE;
    /*0x7BF*/ u8 _7BF;
    /*0x7C0*/ u8 _7C0;
    /*0x7C1*/ u8 _7C1;
    /*0x7C2*/ u8 _7C2;
    /*0x7C3*/ u8 _7C3;
    /*0x7C4*/ u8 _7C4;
    /*0x7C5*/ u8 _7C5;
    /*0x7C6*/ u8 _7C6;
    /*0x7C7*/ u8 _7C7;
    /*0x7C8*/ u8 _7C8;
    /*0x7C9*/ u8 _7C9;
    /*0x7CA*/ u8 _7CA;
    /*0x7CB*/ u8 _7CB;
    /*0x7CC*/ u8 _7CC;
    /*0x7CD*/ u8 _7CD;
    /*0x7CE*/ u8 _7CE;
    /*0x7CF*/ u8 _7CF;
    /*0x7D0*/ u8 _7D0;
    /*0x7D1*/ u8 _7D1;
    /*0x7D2*/ u8 _7D2;
    /*0x7D3*/ u8 _7D3;
    /*0x7D4*/ u8 _7D4;
    /*0x7D5*/ u8 _7D5;
    /*0x7D6*/ u8 _7D6;
    /*0x7D7*/ u8 _7D7;
    /*0x7D8*/ u8 _7D8;
    /*0x7D9*/ u8 _7D9;
    /*0x7DA*/ u8 _7DA;
    /*0x7DB*/ u8 _7DB;
    /*0x7DC*/ u8 _7DC;
    /*0x7DD*/ u8 _7DD;
    /*0x7DE*/ u8 _7DE;
    /*0x7DF*/ u8 _7DF;
    /*0x7E0*/ u8 _7E0;
    /*0x7E1*/ u8 _7E1;
    /*0x7E2*/ u8 _7E2;
    /*0x7E3*/ u8 _7E3;
    /*0x7E4*/ u8 _7E4;
    /*0x7E5*/ u8 _7E5;
    /*0x7E6*/ u8 _7E6;
    /*0x7E7*/ u8 _7E7;
    /*0x7E8*/ u8 _7E8;
    /*0x7E9*/ u8 _7E9;
    /*0x7EA*/ u8 _7EA;
    /*0x7EB*/ u8 _7EB;
    /*0x7EC*/ u8 _7EC;
    /*0x7ED*/ u8 _7ED;
    /*0x7EE*/ u8 _7EE;
    /*0x7EF*/ u8 _7EF;
    /*0x7F0*/ u8 _7F0;
    /*0x7F1*/ u8 _7F1;
    /*0x7F2*/ u8 _7F2;
    /*0x7F3*/ u8 _7F3;
    /*0x7F4*/ u8 _7F4;
    /*0x7F5*/ u8 _7F5;
    /*0x7F6*/ u8 _7F6;
    /*0x7F7*/ u8 _7F7;
    /*0x7F8*/ u8 _7F8;
    /*0x7F9*/ u8 _7F9;
    /*0x7FA*/ u8 _7FA;
    /*0x7FB*/ u8 _7FB;
    /*0x7FC*/ u8 _7FC;
    /*0x7FD*/ u8 _7FD;
    /*0x7FE*/ u8 _7FE;
    /*0x7FF*/ u8 _7FF;
    /*0x800*/ u8 _800;
    /*0x801*/ u8 _801;
    /*0x802*/ u8 _802;
    /*0x803*/ u8 _803;
    /*0x804*/ u8 _804;
    /*0x805*/ u8 _805;
    /*0x806*/ u8 _806;
    /*0x807*/ u8 _807;
    /*0x808*/ u8 _808;
    /*0x809*/ u8 _809;
    /*0x80A*/ u8 _80A;
    /*0x80B*/ u8 _80B;
    /*0x80C*/ u8 _80C;
    /*0x80D*/ u8 _80D;
    /*0x80E*/ u8 _80E;
    /*0x80F*/ u8 _80F;
    /*0x810*/ u8 _810;
    /*0x811*/ u8 _811;
    /*0x812*/ u8 _812;
    /*0x813*/ u8 _813;
    /*0x814*/ u8 _814;
    /*0x815*/ u8 _815;
    /*0x816*/ u8 _816;
    /*0x817*/ u8 _817;
    /*0x818*/ u8 _818;
    /*0x819*/ u8 _819;
    /*0x81A*/ u8 _81A;
    /*0x81B*/ u8 _81B;
    /*0x81C*/ u8 _81C;
    /*0x81D*/ u8 _81D;
    /*0x81E*/ u8 _81E;
    /*0x81F*/ u8 _81F;
    /*0x820*/ u8 _820;
    /*0x821*/ u8 _821;
    /*0x822*/ u8 _822;
    /*0x823*/ u8 _823;
    /*0x824*/ u8 _824;
    /*0x825*/ u8 _825;
    /*0x826*/ u8 _826;
    /*0x827*/ u8 _827;
    /*0x828*/ u8 _828;
    /*0x829*/ u8 _829;
    /*0x82A*/ u8 _82A;
    /*0x82B*/ u8 _82B;
    /*0x82C*/ u8 _82C;
    /*0x82D*/ u8 _82D;
    /*0x82E*/ u8 _82E;
    /*0x82F*/ u8 _82F;
    /*0x830*/ u8 _830;
    /*0x831*/ u8 _831;
    /*0x832*/ u8 _832;
    /*0x833*/ u8 _833;
    /*0x834*/ u8 _834;
    /*0x835*/ u8 _835;
    /*0x836*/ u8 _836;
    /*0x837*/ u8 _837;
    /*0x838*/ u8 _838;
    /*0x839*/ u8 _839;
    /*0x83A*/ u8 _83A;
    /*0x83B*/ u8 _83B;
    /*0x83C*/ u8 _83C;
    /*0x83D*/ u8 _83D;
    /*0x83E*/ u8 _83E;
    /*0x83F*/ u8 _83F;
    /*0x840*/ u8 _840;
    /*0x841*/ u8 _841;
    /*0x842*/ u8 _842;
    /*0x843*/ u8 _843;
    /*0x844*/ u8 _844;
    /*0x845*/ u8 _845;
    /*0x846*/ u8 _846;
    /*0x847*/ u8 _847;
    /*0x848*/ u8 _848;
    /*0x849*/ u8 _849;
    /*0x84A*/ u8 _84A;
    /*0x84B*/ u8 _84B;
    /*0x84C*/ u8 _84C;
    /*0x84D*/ u8 _84D;
    /*0x84E*/ u8 _84E;
    /*0x84F*/ u8 _84F;
    /*0x850*/ u8 _850;
    /*0x851*/ u8 _851;
    /*0x852*/ u8 _852;
    /*0x853*/ u8 _853;
    /*0x854*/ u8 _854;
    /*0x855*/ u8 _855;
    /*0x856*/ u8 _856;
    /*0x857*/ u8 _857;
    /*0x858*/ u8 _858;
    /*0x859*/ u8 _859;
    /*0x85A*/ u8 _85A;
    /*0x85B*/ u8 _85B;
    /*0x85C*/ s16 _85C;
    /*0x85E*/ u8 wallBallWall2Pos;
    /*0x85F*/ u8 pad3;
    /*0x860*/ BB_barrelStruct barrels[15];
    /*0xB6C*/ UnkStructMinigame _B6C;
    /*0xBD4*/ VecXYZ _BD4;
    /*0xBE0*/ f32 _BE0;
    /*0xBE4*/ f32 _BE4;
    /*0xBE8*/ s16 _BE8;
    /*0xBEA*/ s16 _BEA;
    /*0xBEC*/ u8 _BEC;
    /*0xBED*/ u8 _BED;
    /*0xBEE*/ u8 _BEE;
    /*0xBEF*/ u8 _BEF[62];
    /*0xC2D*/ u8 _C2D[7];
    /*0xC34*/ s16 _C34;
    /*0xC36*/ s16 _C36;
    /*0xC38*/ u8 _C38;
    /*0xC39*/ u8 _C39[52];
    /*0xC6D*/ u8 _C6D[64];
    /*0xCAD*/ u8 _CAD;
    /*0xCAE*/ u8 pad4[2];
    /*0xCB0*/ f32 _CB0;
    /*0xCB4*/ f32 _CB4;
    /*0xCB8*/ f32 _CB8;
    /*0xCBC*/ f32 _CBC;
    /*0xCC0*/ f32 _CC0;
    /*0xCC4*/ f32 _CC4;
    /*0xCC8*/ f32 _CC8;
    /*0xCCC*/ s16 _CCC;
    /*0xCCE*/ u8 _CCE[2];
    /*0xCD0*/ VecXYZ wallBall_coinCoordinates;
    /*0xCDC*/ u8 pad5[4];
    /*0xCE0*/ f32 _CE0;
    /*0xCE4*/ u8 pad6[160];
    /*0xD84*/ VecXYZ _D84;
    /*0xD90*/ u8 pad7[1008];
    /*0x1180*/ VecXYZ wallBall_coinVelocity;
    /*0x118C*/ u8 pad8[168];
    /*0x1234*/ VecXYZ _1234;
    /*0x1240*/ u8 pad9[1008];
    /*0x1630*/ f32 _1630;
    /*0x1634*/ u8 pad10[396];
    /*0x17C0*/ s32 _17C0;
    /*0x17C4*/ s32 _17C4;
    /*0x17C8*/ s16 wallBall_coinsVisibleFrameCounter;
    /*0x17CA*/ s16 _17CA;
    /*0x17CC*/ s16 _17CC;
    /*0x17CE*/ s16 _17CE;
    /*0x17D0*/ s16 _17D0;
    /*0x17D2*/ s16 _17D2;
    /*0x17D4*/ s16 _17D4;
    /*0x17D6*/ s16 _17D6;
    /*0x17D8*/ s16 _17D8;
    /*0x17DA*/ s16 _17DA;
    /*0x17DC*/ s16 _17DC;
    /*0x17DE*/ s16 _17DE;
    /*0x17E0*/ s16 _17E0;
    /*0x17E2*/ s16 _17E2;
    /*0x17E4*/ s16 _17E4;
    /*0x17E6*/ s16 _17E6;
    /*0x17E8*/ s16 _17E8;
    /*0x17EA*/ s16 _17EA;
    /*0x17EC*/ s16 _17EC;
    /*0x17EE*/ s16 _17EE;
    /*0x17F0*/ s16 _17F0;
    /*0x17F2*/ s16 _17F2;
    /*0x17F4*/ s16 _17F4;
    /*0x17F6*/ s16 _17F6;
    /*0x17F8*/ s16 _17F8;
    /*0x17FA*/ s16 _17FA;
    /*0x17FC*/ s16 _17FC;
    /*0x17FE*/ s16 _17FE;
    /*0x1800*/ s16 _1800;
    /*0x1802*/ s16 _1802;
    /*0x1804*/ s16 _1804;
    /*0x1806*/ s16 _1806;
    /*0x1808*/ u8 pad11[136];
    /*0x1890*/ s16 miniGameCurrentPoints[4];
    /*0x1898*/ s16 miniGameLatestPoints[4];
    /*0x18A0*/ s16 _18A0;
    /*0x18A2*/ s16 bB_bombBarrelHitInd;
    /*0x18A4*/ s16 bB_bombBarrelID;
    /*0x18A6*/ s16 _18A6;
    /*0x18A8*/ s16 bODControllerInputAllowedInd;
    /*0x18AA*/ s16 framesSincePanelHit;
    /*0x18AC*/ u8 pad12[12];
    /*0x18B8*/ s16 _18B8;
    /*0x18BA*/ s16 _18BA;
    /*0x18BC*/ s32 minigamePoints_current_Latest[4];
    /*0x18CC*/ MiniGameControlStruct minigameControlStruct[2];
    /*0x18F8*/ u8 minigameFielderIndex[4];
    /*0x18FC*/ u8 _18FC[4];
    /*0x1900*/ u8 _1900;
    /*0x1901*/ u8 _1901;
    /*0x1902*/ u8 _1902;
    /*0x1903*/ u8 _1903;
    /*0x1904*/ u8 minigamePlayerSelectedOrder;
    /*0x1905*/ s8 minigamesRosterIDOfCurrentPlayer;
    /*0x1906*/ u8 miniGameNumberOfParticipants;
    /*0x1907*/ u8 _1907;
    /*0x1908*/ u8 _1908;
    /*0x1909*/ u8 multiPlayerInd;
    /*0x190A*/ u8 _190A;
    /*0x190B*/ u8 turnOverStatus;
    /*0x190C*/ u8 turnNumberWithinRound;
    /*0x190D*/ u8 _190D;
    /*0x190E*/ u8 _190E;
    /*0x190F*/ u8 _190F;
    /*0x1910*/ u8 _1910;
    /*0x1911*/ u8 _1911;
    /*0x1912*/ u8 pointsTargetReachedInd;
    /*0x1913*/ u8 _1913;
    /*0x1914*/ u8 _1914;
    /*0x1915*/ u8 _1915;
    /*0x1916*/ u8 _1916;
    /*0x1917*/ u8 _1917;
    /*0x1918*/ u8 _1918;
    /*0x1919*/ u8 _1919;
    /*0x191A*/ u8 _191A;
    /*0x191B*/ u8 _191B;
    /*0x191C*/ u8 _191C;
    /*0x191D*/ u8 _191D;
    /*0x191E*/ u8 _191E;
    /*0x191F*/ u8 _191F;
    /*0x1920*/ E(u8, TOY_FIELD_RESULT) toyFieldBallStateResult2;
    /*0x1921*/ u8 _1921;
    /*0x1922*/ u8 _1922;
    /*0x1923*/ u8 minigameRelatedIndex;
    /*0x1924*/ u8 runnerNum;
    /*0x1925*/ u8 _1925;
    /*0x1926*/ u8 toyFieldStateInd_collisionRelated;
    /*0x1927*/ u8 _1927;
    /*0x1928*/ s16 _1928;
    /*0x192A*/ u8 _192A;
    /*0x192B*/ s16 _192B;
    /*0x192E*/ u8 _192E;
    /*0x192F*/ u8 _192F;
    /*0x1930*/ u8 _1930;
    /*0x1931*/ u8 _1931;
    /*0x1932*/ s16 _1932;
    /*0x1934*/ u8 _1934;
    /*0x1935*/ u8 _1935;
    /*0x1936*/ u8 _1936;
    /*0x1937*/ u8 _1937;
    /*0x1938*/ u8 _1938;
    /*0x1939*/ u8 _1939;
    /*0x193A*/ u8 wallBall_coinsVisibleInd;
    /*0x193B*/ u8 _193B;
    /*0x193C*/ u8 _193C;
    /*0x193D*/ u8 _193D;
    /*0x193E*/ u8 _193E;
    /*0x193F*/ u8 _193F;
    /*0x1940*/ u8 _1940;
    /*0x1941*/ u8 _1941;
    /*0x1942*/ u8 _1942;
    /*0x1943*/ u8 _1943;
    /*0x1944*/ u8 _1944;
    /*0x1945*/ u8 _1945;
    /*0x1946*/ u8 _1946;
    /*0x1947*/ u8 _1947;
    /*0x1948*/ u8 _1948;
    /*0x1949*/ u8 _1949;
    /*0x194A*/ u8 _194A;
    /*0x194B*/ u8 _194B;
    /*0x194C*/ u8 _194C;
    /*0x194D*/ u8 _194D;
    /*0x194E*/ u8 _194E;
    /*0x194F*/ u8 _194F;
    /*0x1950*/ u8 _1950;
    /*0x1951*/ u8 _1951;
    /*0x1952*/ u8 _1952;
    /*0x1953*/ u8 _1953;
    /*0x1954*/ u8 _1954;
    /*0x1955*/ u8 _1955;
    /*0x1956*/ u8 _1956;
    /*0x1957*/ u8 _1957;
    /*0x1958*/ u8 _1958;
    /*0x1959*/ u8 _1959;
    /*0x195A*/ u8 _195A;
    /*0x195B*/ u8 _195B;
    /*0x195C*/ u8 _195C;
    /*0x195D*/ u8 _195D;
    /*0x195E*/ u8 _195E;
    /*0x195F*/ u8 _195F;
    /*0x1960*/ u8 _1960;
    /*0x1961*/ u8 _1961;
    /*0x1962*/ u8 _1962;
    /*0x1963*/ u8 _1963;
    /*0x1964*/ u8 _1964;
    /*0x1965*/ u8 _1965;
    /*0x1966*/ u8 _1966;
    /*0x1967*/ u8 _1967;
    /*0x1968*/ u8 _1968;
    /*0x1969*/ u8 _1969;
    /*0x196A*/ u8 _196A;
    /*0x196B*/ u8 _196B;
    /*0x196C*/ u8 _196C;
    /*0x196D*/ u8 _196D;
    /*0x196E*/ u8 _196E;
    /*0x196F*/ u8 _196F;
    /*0x1970*/ u8 _1970;
    /*0x1971*/ u8 _1971;
    /*0x1972*/ u8 _1972;
    /*0x1973*/ u8 _1973;
    /*0x1974*/ u8 _1974;
    /*0x1975*/ u8 _1975;
    /*0x1976*/ u8 _1976;
    /*0x1977*/ u8 _1977;
    /*0x1978*/ u8 _1978;
    /*0x1979*/ u8 _1979;
    /*0x197A*/ u8 _197A;
    /*0x197B*/ u8 _197B;
    /*0x197C*/ u8 _197C;
    /*0x197D*/ u8 _197D;
    /*0x197E*/ u8 _197E;
    /*0x197F*/ u8 _197F;
    /*0x1980*/ u8 _1980;
    /*0x1981*/ u8 _1981;
    /*0x1982*/ u8 _1982;
    /*0x1983*/ u8 _1983;
    /*0x1984*/ u8 _1984;
    /*0x1985*/ u8 _1985;
    /*0x1986*/ u8 _1986;
    /*0x1987*/ u8 _1987;
    /*0x1988*/ u8 _1988;
    /*0x1989*/ u8 _1989;
    /*0x198A*/ u8 _198A;
    /*0x198B*/ u8 _198B;
    /*0x198C*/ u8 _198C;
    /*0x198D*/ u8 _198D;
    /*0x198E*/ u8 _198E;
    /*0x198F*/ u8 _198F;
    /*0x1990*/ u8 _1990;
    /*0x1991*/ u8 _1991;
    /*0x1992*/ u8 _1992;
    /*0x1993*/ u8 _1993;
    /*0x1994*/ u8 _1994;
    /*0x1995*/ u8 _1995;
    /*0x1996*/ u8 _1996;
    /*0x1997*/ u8 _1997;
    /*0x1998*/ u8 _1998;
    /*0x1999*/ u8 _1999;
    /*0x199A*/ u8 pad13[4];
    /*0x199E*/ u8 _199E;
    /*0x199F*/ u8 panelHitInd;
    /*0x19A0*/ u8 _19A0;
    /*0x19A1*/ u8 _19A1;
    /*0x19A2*/ u8 _19A2;
    /*0x19A3*/ u8 _19A3;
    /*0x19A4*/ bool TF_ballDespawnedInd;
    /*0x19A5*/ u8 _19A5;
    /*0x19A6*/ u8 _19A6;
    /*0x19A7*/ u8 _19A7;
    /*0x19A8*/ u8 challenge_minigame_haven_tWonYetIndicator;
    /*0x19A9*/ u8 _19A9;
    /*0x19AA*/ u8 _19AA;
    /*0x19AB*/ u8 _19AB;
    /*0x19AC*/ f32 lastKnownBallPosX;
    /*0x19B0*/ f32 lastKnownBallPosZ;
    /*0x19B4*/ s16 toyField_selectedTurns;
    /*0x19B6*/ s16 toyField_turnNumber;
    /*0x19B8*/ s16 TF_framesSinceHittingPanel;
    /*0x19BA*/ s16 _19BA;
    /*0x19BC*/ s16 _19BC;
    /*0x19BE*/ u8 _19BE;
    /*0x19BF*/ u8 _19BF;
    /*0x19C0*/ s16 _19C0;
    /*0x19C2*/ s16 _19C2;
    /*0x19C4*/ s16 _19C4;
    /*0x19C6*/ u8 _19C6;
    /*0x19C7*/ u8 _19C7;
    /*0x19C8*/ u8 maybeTFCollisionResultState;
    /*0x19C9*/ E(u8, TOY_FIELD_RESULT) toyFieldBallStateResult;
    /*0x19CA*/ u8 toyField_next_CoinsX2_TurnNumber;
    /*0x19CB*/ u8 toyField_pointMultiplier;
    /*0x19CC*/ u8 toyfield_waitFor_CoinsX2_AnimationToEnd;
    /*0x19CD*/ u8 _19CD;
    /*0x19CE*/ u8 _19CE;
    /*0x19CF*/ u8 _19CF;
    /*0x19D0*/ s16 _19D0;
    /*0x19D2*/ s16 _19D2;
    /*0x19D4*/ s16 _19D4;
    /*0x19D6*/ s16 _19D6;
    /*0x19D8*/ s16 _19D8;
    /*0x19DA*/ u8 _19DA;
    /*0x19DB*/ u8 _19DB;
    /*0x19DC*/ u8 _19DC;
    /*0x19DD*/ u8 _19DD;
    /*0x19DE*/ u8 _19DE;
    /*0x19DF*/ u8 _19DF;
    /*0x19E0*/ u8 _19E0;
    /*0x19E1*/ u8 _19E1;
    /*0x19E2*/ u8 _19E2;
    /*0x19E3*/ u8 _19E3;
    /*0x19E4*/ s16 _19E4;
    /*0x19E6*/ u8 _19E6;
    /*0x19E7*/ u8 _19E7;
    /*0x19E8*/ u8 _19E8;
    /*0x19E9*/ u8 _19E9;
    /*0x19EA*/ u8 _19EA;
    /*0x19EB*/ u8 _19EB;
    /*0x19EC*/ u8 _19EC;
    /*0x19ED*/ u8 _19ED;
    /*0x19EE*/ u8 _19EE;
    /*0x19EF*/ u8 _19EF;
    /*0x19F0*/ u8 _19F0;
    /*0x19F1*/ u8 _19F1;
    /*0x19F2*/ u8 _19F2;
    /*0x19F3*/ u8 _19F3;
    /*0x19F4*/ u8 _19F4;
    /*0x19F5*/ u8 _19F5;
    /*0x19F6*/ u8 pad14[2];
    /*0x19F8*/ u8 _19F8;
    /*0x19F9*/ u8 _19F9;
    /*0x19FA*/ u8 _19FA;
    /*0x19FB*/ u8 _19FB;
    /*0x19FC*/ u8 _19FC;
    /*0x19FD*/ u8 _19FD;
    /*0x19FE*/ u8 _19FE;
    /*0x19FF*/ u8 pad15[2];
    /*0x1A01*/ u8 _1A01;
    /*0x1A02*/ u8 _1A02;
    /*0x1A03*/ u8 _1A03;
    /*0x1A04*/ u8 _1A04;
    /*0x1A05*/ u8 _1A05;
    /*0x1A06*/ u8 _1A06;
    /*0x1A07*/ u8 _1A07;
    /*0x1A08*/ u8 pad16[2];
    /*0x1A0A*/ u8 _1A0A;
    /*0x1A0B*/ u8 _1A0B;
    /*0x1A0C*/ u8 _1A0C;
    /*0x1A0D*/ u8 _1A0D;
    /*0x1A0E*/ u8 _1A0E;
    /*0x1A0F*/ u8 _1A0F;
    /*0x1A10*/ u8 _1A10;
    /*0x1A11*/ u8 _1A11;
    /*0x1A12*/ u8 _1A12;
    /*0x1A13*/ u8 _1A13;
    /*0x1A14*/ u8 _1A14;
    /*0x1A15*/ u8 _1A15;
    /*0x1A16*/ u8 _1A16;
    /*0x1A17*/ u8 battingHandedness[9];
    /*0x1A20*/ u8 _1A20;
    /*0x1A21*/ u8 _1A21;
    /*0x1A22*/ u8 _1A22;
    /*0x1A23*/ u8 _1A23;
    /*0x1A24*/ u8 _1A24[2];
    /*0x1A26*/ s16 pointsReqToWin_challenge;
    /*0x1A28*/ s16 someGraphicFrameCountdown;
    /*0x1A2A*/ E(u8, MINI_GAME_ID) GameMode_MiniGame;
    /*0x1A2B*/ E(u8,MINIGAME_DIFFICULTY) soloMinigameDifficulty;
    /*0x1A2C*/ u8 _1A2C;
    /*0x1A2D*/ u8 miniGameTurnCounter;
    /*0x1A2E*/ u8 bB_pitchesRemainingInTurn;
    /*0x1A2F*/ u8 bODRoundStartingNumPitches;
    /*0x1A30*/ u8 pad17[7];
    /*0x1A37*/ u8 _1A37;
    /*0x1A38*/ u8 _1A38;
    /*0x1A39*/ u8 _1A39;
    /*0x1A3A*/ u8 _1A3A;
    /*0x1A3B*/ u8 pauseInd;
    /*0x1A3C*/ u8 _1A3C;
    /*0x1A3D*/ u8 _1A3D;
    /*0x1A3E*/ u8 _1A3E;
    /*0x1A3F*/ u8 _1A3F;
    /*0x1A40*/ u8 _1A40;
    /*0x1A41*/ u8 _1A41;
    /*0x1A42*/ u8 bODRelated;
    /*0x1A43*/ u8 _1A43;
    /*0x1A44*/ u8 _1A44;
    /*0x1A45*/ u8 _1A45;
    /*0x1A46*/ u8 _1A46[2];
    /*0x1A48*/ f32 wallBallSomeXPos;
    /*0x1A4C*/ f32 wallBallSomeZPos;
    /*0x1A50*/ f32 wallBallWaitingLocations[4];
    /*0x1A70*/ s16 wallBallPitcherRotationCounter;
    /*0x1A72*/ s16 postBallStoppedCounter;
    /*0x1A74*/ s16 wallBallPitchPower;
    /*0x1A76*/ s16 wallBallPitchPowerRemaining;
    /*0x1A78*/ s16 _1A78;
    /*0x1A7A*/ s16 wallBall_UnknownAlways0;
    /*0x1A7C*/ u8 wallBallSpecialWallPos;
    /*0x1A7D*/ u8 _1A7D;
    /*0x1A7E*/ u8 wallBallRotatePitchersInd;
    /*0x1A7F*/ E(u8,WALL_BALL_GAME_STATE) wallBallGameState;
    /*0x1A80*/ u8 _1A80;
    /*0x1A81*/ bool ballStoppedBreakingWallsInd;
    /*0x1A82*/ u8 wallIndexTracker[7];
    /*0x1A89*/ u8 wallBall_hitNoteBlock;
    /*0x1A8A*/ u8 wallBall_hitBowserWall;
    /*0x1A8B*/ u8 _1A8B;
    /*0x1A8C*/ u8 _1A8C[2];
    /*0x1A8E*/ s16 bOD_HitPowerOfEachChar[4];
    /*0x1A96*/ u8 bODRelated5;
    /*0x1A97*/ u8 bODRelated6;
    /*0x1A98*/ u8 bODUnusedBytes[8];
    /*0x1AA0*/ u8 _1AA0[3];
    /*0x1AA3*/ u8 _1AA3;
    /*0x1AA4*/ u8 _1AA4;
    /*0x1AA5*/ u8 _1AA5;
    /*0x1AA6*/ u8 _1AA6;
    /*0x1AA7*/ u8 _1AA7;
    /*0x1AA8*/ u8 _1AA8;
    /*0x1AA9*/ u8 _1AA9;
    /*0x1AAA*/ u8 pad18[30];
    /*0x1AC8*/ u8 bODRelated2;
    /*0x1AC9*/ u8 barrelBatter_BODPitchSelectionType;
    /*0x1ACA*/ u8 minigamePitchSpeedAdjustment;
    /*0x1ACB*/ u8 bOD_HRStreak;
    /*0x1ACC*/ u8 bODCharacterHRStreakTracker[4];
    /*0x1AD4*/ u8 bODRelated3;
    /*0x1AD5*/ u8 bOD_KingBombInd;
    /*0x1AD6*/ u8 bODAngleIndexBasedOnHitPower;
    /*0x1AD7*/ u8 bOD_hitFinishedInd;
    /*0x1AD8*/ u8 _1AD8;
    /*0x1AD9*/ u8 _1AD9;
    /*0x1ADA*/ u8 barrelBatterChargeMeter;
    /*0x1ADB*/ u8 barrelBatter_scoreCalculatedInd;
    /*0x1ADC*/ u8 barrelBatter_hitBarrelID;
    /*0x1ADD*/ u8 barrelBatter_barrelsHit;
    /*0x1ADE*/ u8 _1ADE[2];
    /*0x1AE0*/ f32 _1AE0;
    /*0x1AE4*/ f32 _1AE4;
    /*0x1AE8*/ f32 _1AE8;
    /*0x1AEC*/ f32 _1AEC;
    /*0x1AF0*/ f32 _1AF0;
    /*0x1AF4*/ f32 _1AF4;
    /*0x1AF8*/ s16 _1AF8;
    /*0x1AFA*/ s16 _1AFA;
    /*0x1AFC*/ s16 _1AFC;
    /*0x1AFE*/ u8 _1AFE[2];
    /*0x1B00*/ s16 _1B00;
    /*0x1B02*/ s16 _1B02;
    /*0x1B04*/ s16 _1B04;
    /*0x1B06*/ s16 _1B06;
    /*0x1B08*/ s16 _1B08;
    /*0x1B0A*/ s16 _1B0A;
    /*0x1B0C*/ s16 _1B0C;
    /*0x1B0E*/ s16 _1B0E;
    /*0x1B10*/ s16 _1B10;
    /*0x1B12*/ s16 _1B12;
    /*0x1B14*/ u8 _1B14;
    /*0x1B15*/ u8 _1B15[4];
    /*0x1B19*/ u8 _1B19;
    /*0x1B1A*/ u8 _1B1A;
    /*0x1B1B*/ u8 _1B1B;
    /*0x1B1C*/ u8 _1B1C;
    /*0x1B1D*/ u8 _1B1D;
    /*0x1B1E*/ u8 _1B1E;
    /*0x1B1F*/ u8 _1B1F;
    /*0x1B20*/ u8 _1B20;
    /*0x1B21*/ u8 _1B21;
    /*0x1B22*/ u8 _1B22;
    /*0x1B23*/ u8 _1B23;
    /*0x1B24*/ u8 _1B24;
    /*0x1B25*/ u8 _1B25;
    /*0x1B26*/ u8 _1B26;
    /*0x1B27*/ u8 _1B27;
    /*0x1B28*/ u8 _1B28;
    /*0x1B29*/ u8 _1B29;
    /*0x1B2A*/ u8 _1B2A;
    /*0x1B2B*/ u8 _1B2B;
    /*0x1B2C*/ u8 _1B2C;
    /*0x1B2D*/ u8 _1B2D;
    /*0x1B2E*/ u8 _1B2E;
    /*0x1B2F*/ u8 _1B2F;
    /*0x1B30*/ u8 _1B30;
    /*0x1B31*/ u8 _1B31;
    /*0x1B32*/ u8 _1B32;
    /*0x1B33*/ u8 _1B33;
    /*0x1B34*/ s16 _1B34;
    /*0x1B36*/ u8 _1B36[6];
    /*0x1B3C*/ s16 _1B3C;
    /*0x1B3E*/ u8 _1B3E[6];
    /*0x1B44*/ s16 _1B44;
    /*0x1B46*/ u8 _1B46[6];
    /*0x1B4C*/ s16 _1B4C;
    /*0x1B4E*/ s16 _1B4E;
    /*0x1B50*/ s16 _1B50;
    /*0x1B52*/ u8 _1B52[6];
    /*0x1B58*/ u8 _1B58;
    /*0x1B59*/ u8 _1B59;
    /*0x1B5A*/ u8 _1B5A;
    /*0x1B5B*/ u8 _1B5B;
    /*0x1B5C*/ u8 _1B5C;
    /*0x1B5D*/ u8 _1B5D;
    /*0x1B5E*/ u8 _1B5E;
    /*0x1B5F*/ u8 _1B5F;
    /*0x1B60*/ u8 _1B60;
    /*0x1B61*/ u8 _1B61;
    /*0x1B62*/ u8 _1B62[30];
    /*0x1B80*/ u8 _1B80[4];
    /*0x1B84*/ u8 _1B84[1];
    /*0x1B85*/ u8 _1B85[49];
    /*0x1BB6*/ u8 _1BB6;
    /*0x1BB7*/ u8 _1BB7;
    /*0x1BB8*/ u8 _1BB8;
    /*0x1BB9*/ u8 _1BB9;
    /*0x1BBA*/ u8 _1BBA;
    /*0x1BBB*/ u8 _1BBB;
    /*0x1BBC*/ u8 _1BBC;
    /*0x1BBD*/ u8 _1BBD;
    /*0x1BBE*/ u8 _1BBE;
    /*0x1BBF*/ u8 _1BBF;
    /*0x1BC0*/ u8 _1BC0;
    /*0x1BC1*/ u8 _1BC1;
    /*0x1BC2*/ u8 _1BC2;
    /*0x1BC3*/ u8 _1BC3;
    /*0x1BC4*/ u8 _1BC4;
    /*0x1BC5*/ u8 _1BC5;
    /*0x1BC6*/ u8 _1BC6;
    /*0x1BC7*/ u8 _1BC7;
    /*0x1BC8*/ u8 _1BC8;
    /*0x1BC9*/ u8 _1BC9;
    /*0x1BCA*/ u8 _1BCA;
    /*0x1BCB*/ u8 _1BCB;
    /*0x1BCC*/ u8 _1BCC;
    /*0x1BCD*/ u8 _1BCD;
    /*0x1BCE*/ u8 _1BCE;
    /*0x1BCF*/ u8 _1BCF;
    /*0x1BD0*/ u8 _1BD0;
    /*0x1BD1*/ u8 _1BD1;
    /*0x1BD2*/ u8 _1BD2;
    /*0x1BD3*/ u8 _1BD3;
    /*0x1BD4*/ u8 _1BD4;
    /*0x1BD5*/ u8 _1BD5;
    /*0x1BD6*/ u8 _1BD6[18];
    /*0x1BE8*/ u8 _1BE8[50];
    /*0x1C1A*/ u8 _1C1A[50];
    /*0x1C4C*/ u8 _1C4C[50];
    /*0x1C7E*/ u8 _1C7E;
    /*0x1C7F*/ u8 _1C7F;
    /*0x1C80*/ u8 _1C80;
    /*0x1C81*/ u8 _1C81[9];
    /*0x1C8A*/ u8 _1C8A[4];
    /*0x1C8E*/ u8 _1C8E[4];
    /*0x1C92*/ u8 _1C92[8];
    /*0x1C9A*/ u8 _1C9A;
    /*0x1C9B*/ u8 _1C9B;
    /*0x1C9C*/ s16 _1C9C;
    /*0x1C9E*/ u8 _1C9E;
    /*0x1C9F*/ u8 _1C9F;
    /*0x1CA0*/ u8 _1CA0[2];
    /*0x1CA2*/ u8 _1CA2;
    /*0x1CA3*/ u8 _1CA3;
    /*0x1CA4*/ u8 _1CA4;
    /*0x1CA5*/ u8 _1CA5[4];
    /*0x1CA9*/ u8 _1CA9;
    /*0x1CAA*/ u8 _1CAA;
    /*0x1CAB*/ u8 _1CAB;
    /*0x1CAC*/ u8 _1CAC;
    /*0x1CAD*/ u8 _1CAD[4];
    /*0x1CB1*/ u8 _1CB1[7];
    /*0x1CB8*/ f32 _1CB8;
    /*0x1CBC*/ f32 _1CBC;
    /*0x1CC0*/ f32 _1CC0;
    /*0x1CC4*/ u8 _1CC4[36];
    /*0x1CE8*/ VecXYZ _1CE8;
    /*0x1CF4*/ VecXYZ _1CF4;
    /*0x1D00*/ u8 _1D00[4];
    /*0x1D04*/ VecXYZ _1D04;
    /*0x1D10*/ VecXYZ _1D10;
    /*0x1D1C*/ VecXYZ _1D1C;
    /*0x1D28*/ VecXYZ _1D28;
    /*0x1D34*/ VecXYZ _1D34;
    /*0x1D40*/ f32 _1D40;
    /*0x1D44*/ f32 _1D44;
    /*0x1D48*/ f32 _1D48;
    /*0x1D4C*/ f32 starDashRelated_0_5Or1_5;
    /*0x1D50*/ s16 _1D50;
    /*0x1D52*/ s16 _1D52;
    /*0x1D54*/ s16 _1D54;
    /*0x1D56*/ s16 _1D56;
    /*0x1D58*/ s16 _1D58;
    /*0x1D5A*/ s16 _1D5A;
    /*0x1D5C*/ u8 _1D5C[6];
    /*0x1D62*/ s16 _1D62;
    /*0x1D64*/ s16 _1D64;
    /*0x1D66*/ s16 _1D66;
    /*0x1D68*/ s16 _1D68;
    /*0x1D6A*/ s16 _1D6A;
    /*0x1D6C*/ u8 _1D6C;
    /*0x1D6D*/ u8 _1D6D;
    /*0x1D6E*/ u8 starDashStunType[4];
    /*0x1D72*/ u8 _1D72;
    /*0x1D73*/ u8 _1D73;
    /*0x1D74*/ u8 playerIDWithPowerup[2];
    /*0x1D76*/ s16 _1D76;
    /*0x1D78*/ u8 _1D78;
    /*0x1D79*/ u8 _1D79;
    /*0x1D7A*/ u8 _1D7A;
    /*0x1D7B*/ u8 _1D7B;
} MiniGameStruct; // size: 0x1D7C


extern MiniGameStruct minigameStruct;

typedef struct _BatterReachStruct
{
    /*0x000*/ f32 HorizontalRangeNear;
    /*0x004*/ f32 HorizontalRangeFar;
    /*0x008*/ f32 VerticalRangeFront;
    /*0x00C*/ f32 VerticalRangeBack;
    /*0x010*/ f32 batOffsetFromBatterX;
    /*0x014*/ f32 batOffsetFromBatterZ;
    /*0x018*/ f32 BoxMoveSpeed;
    /*0x01C*/ f32 PitchingHeight;
    /*0x020*/ f32 TrimmedBat;
} BatterReachStruct; // size: 0x24

extern BatterReachStruct BatterHitbox[54];


#endif // !__UNKNOWN_HOMES_H_
