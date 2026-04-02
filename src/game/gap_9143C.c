#include "game/gap_9143C.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_Static.h"
#include "UnknownHeaders.h"
#include "game/rep_3E58.h"

extern void fn_3_912B4(void);
extern void fn_3_EA340(void);
extern void fn_3_93D5C(void);

// .text:0x0009143C size:0xE4 mapped:0x806D04D0
void fn_3_9143C(void) {
    if (g_GameLogic.gameStatus == 2) {
        if (lbl_3_common_bss_32724._pad3a[0] == 0) {
            lbl_3_common_bss_32724._pad3a[0] = 1;
            if (g_d_GameSettings.GameModeSelected != 6) {
                fn_800B0A5C_insertQueue(fn_3_912B4, 2);
            }
            if (g_d_GameSettings.GameModeSelected == 6) {
                fn_800B0A5C_insertQueue(fn_3_EA340, 2);
            }
        }
    }
    
    if (g_d_GameSettings.GameModeSelected != GAME_TYPE_PRACTICE) {
        if (lbl_3_common_bss_32724._pad3a[2] != 0) {
            if (lbl_3_common_bss_32724._pad3a[1] == 0) {
                fn_800B0A5C_insertQueue(fn_3_93D5C, 2);
            } else {
                lbl_3_common_bss_32724._pad3a[1] = 2;
            }
        }
    }
}
