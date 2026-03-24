#ifndef __GAME_gap_BA150_H_
#define __GAME_gap_BA150_H_

#include "mssbTypes.h"
#include "game/rep_1E08.h"

typedef struct {
    u8 _pad[0x4];
    f32 sortKey; // 0x04
} BA150SortItem;

void fn_3_BA150(void);
s32 fn_3_BA174(BA150SortItem* a, BA150SortItem* b);
void fn_3_BA1A0(void);
void fn_3_BA268(void);
void fn_3_BA3EC(void);

#endif // !__GAME_gap_BA150_H_
