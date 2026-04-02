#include "game/gap_167CC4.h"

#include "UnknownHeaders.h"
#include "static/UnknownHomes_Static.h"
#include "game/UnknownHomes_Game.h"
#include "game/rep_3E58.h"

// .text:0x00167CC4 size:0x88
void fn_3_167CC4(void) {
    if (g_d_GameSettings._55 != 0 || lbl_3_common_bss_35154.unk_479 != 0) {
        fn_800B0A14_removeQueue();
        return;
    }
    if (g_GameLogic.gameStatus != 2) {
        fn_800B0A14_removeQueue();
        return;
    }
    if (g_Ball.ballState == 2) {
        fn_3_16699C();
        return;
    }
    fn_800B0A14_removeQueue();
}

// .text:0x00167D4C size:0x1C8
void fn_3_167D4C(void) {
    return;
}

// .text:0x00167F14 size:0x1C0
void fn_3_167F14(void) {
    return;
}

// .text:0x001680D4 size:0x1D8
void fn_3_1680D4(void) {
    return;
}

// .text:0x001682AC size:0x168
void fn_3_1682AC(void) {
    return;
}
