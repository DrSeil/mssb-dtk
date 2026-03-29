#include "game/auto_00_000CB344_text.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/rep_1F58.h"

// .text:0x000CB344 size:0x68
void fn_3_CB344(int arg0, u8 arg1) {
    if (g_d_GameSettings.GameModeSelected != 7 ||
        (g_Minigame.GameMode_MiniGame != 1 && g_Minigame.GameMode_MiniGame != 3)) {
        fn_3_C1770(arg0);
    }
    lbl_3_common_bss_35154.unk_467 = arg1;
}
