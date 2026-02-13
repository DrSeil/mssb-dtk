#ifndef __GAME_rep_3AE8_H_
#define __GAME_rep_3AE8_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0x10];
    u32 value;
} UnknownStruct_2709C;

extern UnknownStruct_2709C lbl_3_data_2709C;

void fn_3_15B79C(void);
void fn_3_15BAA0(void);
void fn_3_15C000(void);
u32 fn_3_15C014(void);
void fn_3_15C024(void);

#endif // !__GAME_rep_3AE8_H_
