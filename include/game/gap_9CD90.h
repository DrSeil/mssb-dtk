#ifndef __GAME_gap_9CD90_H_
#define __GAME_gap_9CD90_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_Static.h"
#include "UnknownHeaders.h"

typedef struct {
    s16 unk0;           /* 0x00 */
    u8 _pad2[0x12 - 0x02];
    u8 unk12;           /* 0x12 */
    u8 _pad13[0x20 - 0x13];
} RunningLogicStruct;

extern RunningLogicStruct g_RunningLogic;

void fn_3_9CD90(void);
void fn_3_9CE78(void);
void fn_3_9D140(void);
void fn_3_9D374(void);
void fn_3_9D550(void);
void fn_3_9D594(void);
void fn_3_9D600(void);
void fn_3_9D6A4(void);
void fn_3_9DB5C(void);
void fn_3_9DBE4(void);
void fn_3_9DC18(void);
void fn_3_9E078(void);
void fn_3_9E368(void);
void fn_3_9E5A0(void);
void fn_3_9E7D4(s32 arg0);
void fn_3_9E834(void);
void fn_3_9EA1C(void);

#endif // !__GAME_gap_9CD90_H_
