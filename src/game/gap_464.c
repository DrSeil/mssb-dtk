#include "game/gap_464.h"

void fn_3_5AE9C(void);
void fn_3_C0824(void);

// .text:0x00000464 size:0x38 mapped:0x8063F4F8
void _epilog(void) {
    g_d_GameSettings._55 = 0;
    fn_800BF038(0, &g_d_GameSettings);
    fn_3_BF20C();
}

// .text:0x0000049C size:0x58 mapped:0x8063F530
void _prolog(void) {
    fn_800B0A5C_insertQueue(fn_3_5AE9C, 1);
    fn_3_C0824();
    fn_80036C88(&lbl_3_data_118, &lbl_3_data_118.sub);
    fn_800B0D28(&lbl_3_data_118.sub);
    fn_8004B270();
}

// .text:0x000004F4 size:0x168 mapped:0x8063F588
void manageStadiumLoading(void) {
    return;
}

