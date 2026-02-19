#ifndef __GAME_rep_720_H_
#define __GAME_rep_720_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "game/rep_1838.h"
#include "static/UnknownHomes_Static.h"

typedef struct Camera {
    /* 0x000 */ u8 _pad0[0xA50];
    /* 0xA50 */ s32 unk_A50;
    /* 0xA54 */ u8 _pad1[0xA96 - 0xA54]; 
    /* 0xA96 */ s16 unkA96;       // Offset 0xA96: used in fn_3_14FC8
    /* 0xA98 */ u8 _pad2[0xAA4 - 0xA98];
    /* 0xAA4 */ s16 unkAA4;       // Offset 0xAA4: used in fn_3_14E84
    /* 0xAA6 */ s16 unkAA6;       // Offset 0xAA6: used in fn_3_14EE8
    /* 0xAA8 */ u8 _pad3[0xAC5 - 0xAA8];
    /* 0xAC5 */ u8 cameraFlag;    // Offset 0xAC5
    /* 0xAC6 */ u8 _pad4[0xAD8 - 0xAC6];
    /* 0xAD8 */ u8 cameraFlag2;   // Offset 0xAD8
} Camera;

typedef struct {
    u8 _pad[0xC];
    s32 unk_C; // Offset 0xC
} Strikes;

extern Camera* g_pCamera;
extern Strikes g_Strikes;

void fn_3_14BB4(void);
void fn_3_14E1C(void);
void fn_3_14E50(void);
void fn_3_14E84(void);
void fn_3_14EC0(void);
void fn_3_14EE8(void);
void fn_3_14F24(void);
void fn_3_14F68(void);
void fn_3_14FC8(void);
void fn_3_15004(void);
void fn_3_150C0(void);
void fn_3_15144(void);
void fn_3_15220(void);
void fn_3_1527C(void);
void fn_3_152E8(void);
void fn_3_1537C(void);
void fn_3_153F0(void);
void fn_3_15458(void);
void fn_3_154C4(void);
void fn_3_15524(void);
void fn_3_15A98(void);
void fn_3_15D28(void);
void fn_3_15D64(void);
void fn_3_15D7C(u8 value);
void fn_3_15D90(void);
void fn_3_15F18(void);
void fn_3_15F38(void);
void fn_3_15F74(void);
void fn_3_15F94(void);
void fn_3_15FD0(void);
void fn_3_15FF0(void);
void fn_3_16014(void);
void fn_3_16034(void);
void fn_3_16058(void);
void fn_3_161C0(void);
void fn_3_16328(void);
void fn_3_164A8(void);
void fn_3_1650C(void);
void fn_3_166F4(void);
void fn_3_16900(void);
void fn_3_16BA0(void);
void fn_3_16D6C(void);
void fn_3_16F04(void);
void fn_3_17110(void);
void fn_3_1731C(void);
void fn_3_17760(void);
void fn_3_17804(void);
void fn_3_1785C(void);
void fn_3_17AF0(void);
void fn_3_17B2C(void);
void fn_3_17DA0(void);
void fn_3_1850C(void);
void fn_3_18FF8(void);
void fn_3_193E8(void);
void fn_3_195BC(void);
void fn_3_19770(void);
void fn_3_197E8(void);
void fn_3_19B3C(void);
void fn_3_19CB0(void);
void fn_3_19FA4(void);
void fn_3_1AE44(void);
void fn_3_1AEB8(void);
void fn_3_1B17C(void);
void fn_3_1B370(void);
void fn_3_1BA18(void);
void fn_3_1C0B0(void);
void fn_3_1C130(void);
void fn_3_1C1B0(void);
void fn_3_1C684(void);
u8 getSceneID(void);
void fn_3_1C8A8(void);
void fn_3_1C8AC(void);
void fn_3_1CAC0(void);
void fn_3_1CBCC(void);
void fn_3_1CCC8(void);
void fn_3_1CE90(void);

extern s32 lbl_3_bss_44;
typedef struct Stats {
u8 _pad[0x24];
u32 unk24;
} Stats;
extern Stats g_Stats;
// No local headers needed.

#endif // !__GAME_rep_720_H_
