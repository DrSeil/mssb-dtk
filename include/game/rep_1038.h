#ifndef __GAME_rep_1038_H_
#define __GAME_rep_1038_H_

#include "mssbTypes.h"

typedef struct {
    u8 _pad[0xC3];
    u8 someFlag; // 0xC3
} Scores;

extern Scores g_Scores;

void fn_3_6BEA4(void);
void fn_3_6C000(void);
void fn_3_6C0E0(void);
void fn_3_6C108(void);
void fn_3_6C13C(void);
void fn_3_6C150(void);
void fn_3_6C1D8(void);

// No local headers needed.
#endif // !__GAME_rep_1038_H_
