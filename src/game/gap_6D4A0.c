#include "game/gap_6D4A0.h"

// .text:0x0006D4A0 size:0xC4 mapped:0x806AC534
void fn_3_6D4A0(void) {
    return;
}

// .text:0x0006D564 size:0xA0 mapped:0x806AC5F8
void fn_3_6D564(void) {
    return;
}

// .text:0x0006D604 size:0x54 mapped:0x806AC698
BOOL checkFieldingStat(int teamIdx, int playerIdx, int statBit) {
    if (g_d_GameSettings.minigamesEnabled) {
        teamIdx = 0;
    }
    if (inMemRoster[teamIdx][playerIdx].stats.FieldingStats & (1U << statBit)) {
        return 1;
    }
    return 0;
}

// .text:0x0006D658 size:0x7C mapped:0x806AC6EC
void fn_3_6D658(void) {
    return;
}

