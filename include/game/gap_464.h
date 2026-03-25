#ifndef __GAME_gap_464_H_
#define __GAME_gap_464_H_

#include "mssbTypes.h"
#include "static/UnknownHomes_Static.h"
#include "game/rep_1E08.h"

typedef struct { u8 _pad[0x5C]; } Data118Sub;
typedef struct {
    u8 _pad0[0x5C];
    Data118Sub sub;
} Data118;

extern Data118 lbl_3_data_118;
void fn_80036C88(Data118* arg0, Data118Sub* arg1);
void fn_800B0D28(Data118Sub* arg0);
void fn_8004B270(void);

void fn_800BF038(s32 arg0, GameInitVariables* arg1);
void _epilog(void);
void _prolog(void);
void manageStadiumLoading(void);

#endif // !__GAME_gap_464_H_
