#ifndef __UNKNOWN_HOMES_STATIC_H_
#define __UNKNOWN_HOMES_STATIC_H_

#include "mssbTypes.h"

extern int fn_8001C67C_animation(int, int);

extern f32 LinearInterpolateToNewRange(f32 value, f32 prevMin, f32 prevMax, f32 nextMin, f32 nextMax);

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

extern GameInitVariables g_d_GameSettings;

typedef struct Camera {
    /* 0x000 */ u8 _pad0[0xA50];
    /* 0xA50 */ s32 unk_A50;
    /* 0xA54 */ u8 _pad1[0xA82 - 0xA54];
    /* 0xA82 */ s16 unkA82;
    /* 0xA84 */ u8 _pad1b[0xA96 - 0xA84];
    /* 0xA96 */ s16 unkA96;       // used in fn_3_14FC8
    /* 0xA98 */ s16 unkA98;       // Offset 0xA98
    /* 0xA9A */ s16 unkA9A;
    /* 0xA9C */ s16 unkA9C;       // Offset 0xA9C: used in fn_3_14F24
    /* 0xA9E */ u8 _pad3[0xAA4 - 0xA9E];
    /* 0xAA4 */ s16 unkAA4;       // used in fn_3_14E84
    /* 0xAA6 */ s16 unkAA6;       // used in fn_3_14EE8
    /* 0xAA8 */ u8 _pad4[0xABA - 0xAA8];
    /* 0xABA */ u8 unkABA;
    /* 0xABB */ u8 _pad4b[0xAC4 - 0xABB];
    /* 0xAC4 */ u8 unkAC4;
    /* 0xAC5 */ u8 cameraFlag;
    /* 0xAC6 */ u8 _pad5[0xAD8 - 0xAC6];
    /* 0xAD8 */ u8 cameraFlag2;
    /* 0xAD9 */ u8 _pad6[0x2840 - 0xAD9];
    /* 0x2840 */ f32 unk2840;
    /* 0x2844 */ f32 unk2844;
    /* 0x2848 */ f32 unk2848;
    /* 0x284C */ f32 unk284C;
    /* 0x2850 */ f32 unk2850;
    /* 0x2854 */ f32 unk2854;
    /* 0x2858 */ u8 _pad7[0x2878 - 0x2858];
    /* 0x2878 */ f32 unk2878;
} Camera;

typedef struct Strikes {
    /* 0x00 */ s32 strikes;      // Target for lwz r4, 0x0(r5)
    /* 0x04 */ s32 balls;        // Target for lwz r3, 0x4(r5)
    /* 0x08 */ s32 outs;
    s32 unk_C;           /* 0x0C */
    u8 _pad18[0x8];      /* 0x10 - Padding to reach 0x18 */
    s16 unk_18[3];       /* 0x18 - Array of strike values */
} Strikes; // Total size likely 0x1E or padded to 0x20

extern Camera* g_pCamera;
extern Strikes g_Strikes;


#endif // !__UNKNOWN_HOMES_STATIC_H_
