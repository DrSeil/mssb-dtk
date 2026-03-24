#ifndef __GAME_gap_CABB4_H_
#define __GAME_gap_CABB4_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0x34];
    u8 unk34;
} CabB4Object;

typedef struct {
    CabB4Object* ptr0;
    CabB4Object* ptr4;
} Data17D10;

extern Data17D10 lbl_3_data_17D10;

void fn_3_CABB4(void);

#endif // !__GAME_gap_CABB4_H_
