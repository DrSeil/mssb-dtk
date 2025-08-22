#ifndef __UNKNOWN_HOMES_H_
#define __UNKNOWN_HOMES_H_

#include "mssbTypes.h"
#include "Dolphin/pad.h"

#define SINF(x) ((f32)sin(x))
#define COSF(x) ((f32)cos(x))

#define BATTER_MAX_CHARGE (1.f)
#define BATTER_MIN_CHARGE (0.f)

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
    /*0x1B50*/ u32 StaticRandomInt2;
    /*0x1B54*/ u32 StaticRandomInt1_prePitch;
    /*0x1B58*/ s16 airResistance; // 40
    /*0x1B5A*/ s16 _1B58;
    /*0x1B5C*/ s16 frameBallWillHitWall; // unused
    /*0x1B5E*/ s16 hangtimeOfHit;
    /*0x1B60*/ s16 framesUntilBallHitsGround;
    /*0x1B62*/ s16 framesUntilThrowReachesDest;
    /*0x1B64*/ u16 totalFramesAtPlay;
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

extern int RandomInt_Game(int max);

typedef enum _GAME_TYPE {
    GAME_TYPE_EXHIBITION_GAME = 0,
    GAME_TYPE_PRACTICE = 2,
    GAME_TYPE_DEMO = 4,
    GAME_TYPE_CHALLENGE = 5,
    GAME_TYPE_TOY_FIELD = 6,
    GAME_TYPE_MINIGAMES = 7,
} GAME_TYPE;

typedef enum _STADIUM_ID{
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
    /*0x06*/ u8 _06;          // 1 Menu,2 InGame/Minigame/Practice/etc
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

typedef struct _InputStruct
{
    /*0x00*/ sAng controlStickAngle;
    /*0x02*/ s16 controlStickMagnitude;
    /*0x04*/ s16 buttonInput; // uses pad defines
    /*0x06*/ s16 newButtonInput; // pressed this frame
    /*0x08*/ s16 _08;
    /*0x0A*/ s8 right_left;
    /*0x0B*/ s8 up_down;
    /*0x0C*/ s8 rightTriggerDistance;
    /*0x0D*/ s8 leftTriggerDistance;
    /*0x0E*/ s16 _0E; // padding
} InputStruct; // size: 0x10

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
    /*0x152*/ frame_t _152; // count To 60 then Set Hit Vars For Fielding Practice
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
} PracticeStruct; // size: 0x1f0

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

extern void estimateAndSetFutureCoords(int);
extern f32 angle4096_toRad(int angle);

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
    GAME_STATUS_MINIGAME_NEW_ROUND, // unsure
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

typedef enum _WIN_TYPE{
    WIN_TYPE_0,
    WIN_TYPE_1,
    WIN_TYPE_2,
    WIN_TYPE_3,
    WIN_TYPE_4,
    WIN_TYPE_5,
    WIN_TYPE_6,
} WIN_TYPE;

extern bool checkFieldingStat(int, int, E(int, FIELDING_ABILITY));

typedef struct _LogoInfoStruct{
    int ID;
    int variationID;
    int captain; // unsure
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
    /*0x12B*/ u8 pre_PostMiniGameInd; // unsure
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
    /*0x13A*/ u8 walkOffWinInd; //unsure
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
} GameControlsStruct; // size: 0x158

extern GameControlsStruct gameControls;
#endif // !__UNKNOWN_HOMES_H_
