#include "game/gap_759BC.h"

#include "game/UnknownHomes_Game.h"

typedef struct {
    u8 _pad[0xa0];
    s8 charIDs[0x32];
    u8 _pad2[0x36];
} Lbl80353A90Struct; // size: 0x108

extern Lbl80353A90Struct lbl_80353A90;

// .text:0x000759BC size:0x7B8
void fn_3_759BC(void) {
    return;
}

// .text:0x00076174 size:0x3E4
void fn_3_76174(void) {
    return;
}

// .text:0x00076558 size:0x544
void fn_3_76558(void) {
    return;
}

// .text:0x00076A9C size:0x1DC
void fn_3_76A9C(void) {
    return;
}

// .text:0x00076C78 size:0x90
void fn_3_76C78(void) {
    s32 i;
    for (i = 0; i < 0x32; i++) {
        if (lbl_80353A90.charIDs[i] == -1) {
            if (g_GameLogic.teamBatting == 0) {
                lbl_80353A90.charIDs[i] = g_Batter.rosterID;
                return;
            }
            lbl_80353A90.charIDs[i] = (s8)g_Batter.rosterID + 9;
            return;
        }
    }
}

// .text:0x00076D08 size:0xC0C
void fn_3_76D08(void) {
    return;
}
