#include "game/auto_00_000CB344_text.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x000CB344 size:0x68

void fn_3_CB344(unsigned char param_1) {
    if (((unsigned char)g_d_GameSettings.GameModeSelected != 7) || (((unsigned char)g_Minigame.GameMode_MiniGame != 1) && ((unsigned char)g_Minigame.GameMode_MiniGame != 3))) {
        fn_3_C1770();
    }
    lbl_3_common_bss_35154.unk_467 = param_1;
}
