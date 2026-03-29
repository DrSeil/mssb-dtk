#include "game/gap_157DB8.h"

// .text:0x00157DB8 size:0x70 mapped:0x80796E4C
void fn_3_157DB8(void *arg0) {
    extern void fn_3_157AC4(void);
    Fn157DB8Struct *temp_r3 = (Fn157DB8Struct *)fn_80033A24(fn_3_157AC4, 0x80, 0, 1, 1, 0x16);
    if (temp_r3 != 0) {
        temp_r3->unk18 = 0;
        temp_r3->unk1C = arg0;
        temp_r3->unkC->unk4D = 0;
        temp_r3->unkC->unk40 = -1;
    }
}

// .text:0x00157E28 size:0x284 mapped:0x80796EBC
void fn_3_157E28(void) {
    return;
}

// .text:0x001580AC size:0x60 mapped:0x80797140
extern int lbl_3_data_BD90;

void fn_3_1580AC(void) {
    DrawingSceneStruct *p = lbl_803CC1B8;
    // NONMATCHING: fn_80034E20 is 3-arg but target assembly shows 2-arg call (no r5 setup)
    fn_80034E20((void (*)(void))p, &lbl_3_data_BD90, (void **)&lbl_803CC1B8);
    p->eventID = 0;
    p->fielderIndex = 0;
    lbl_803CC1B8->aFunctionPointer = (void *)fn_3_157E28;
}

// .text:0x0015810C size:0xF0 mapped:0x807971A0
void fn_3_15810C(void) {
    return;
}

// .text:0x001581FC size:0x68 mapped:0x80797290
void fn_3_1581FC(void) {
    fn_80034E20((void(*)(void))lbl_803CC1B8, (int*)&lbl_3_data_BD50, (void**)&lbl_803CC1B8);
    if ((u8)lbl_800EF808.unk398 == 1) {
        playSoundEffect(0x1AD);
    }
    lbl_803CC1B8->aFunctionPointer = fn_3_15810C;
}

// .text:0x00158264 size:0x148 mapped:0x807972F8
void fn_3_158264(void) {
    return;
}

// .text:0x001583AC size:0x304 mapped:0x80797440
void fn_3_1583AC(void) {
    return;
}

// .text:0x001586B0 size:0x314 mapped:0x80797744
void fn_3_1586B0(void) {
    return;
}

// .text:0x001589C4 size:0x1A0 mapped:0x80797A58
void fn_3_1589C4(void) {
    return;
}

// .text:0x00158B64 size:0x480 mapped:0x80797BF8
void fn_3_158B64(void) {
    return;
}

// .text:0x00158FE4 size:0xE4 mapped:0x80798078
void fn_3_158FE4(void) {
    return;
}

// .text:0x001590C8 size:0x4C mapped:0x8079815C
void fn_3_1590C8(void) {
    if (g_Practice.tutorialState == 0 && g_Practice.practiceState == 7) {
        fn_800B0A5C_insertQueue(fn_3_158FE4, 2);
    }
}

// .text:0x00159114 size:0x47C mapped:0x807981A8
void fn_3_159114(void) {
    return;
}

// .text:0x00159590 size:0x64 mapped:0x80798624
void fn_3_159590(void) {
    // NONMATCHING: target assembly shows 2-arg call (no r5 setup), but fn_80034E20 is
    // declared with 3 args globally; local redeclaration is rejected by CW
    DrawingSceneStruct *dss = lbl_803CC1B8;
    fn_80034E20((void*)dss, (int*)&lbl_3_data_B3F4, (void**)&lbl_803CC1B8);
    dss->unk18 = 0;
    dss->unk1a = 0;
    dss->eventID = 0;
    lbl_803CC1B8->aFunctionPointer = fn_3_159114;
}

// .text:0x001595F4 size:0x3C4 mapped:0x80798688
void fn_3_1595F4(void) {
    return;
}

// .text:0x001599B8 size:0x398 mapped:0x80798A4C
void fn_3_1599B8(void) {
    return;
}

// .text:0x00159D50 size:0x4F4 mapped:0x80798DE4
void fn_3_159D50(void) {
    return;
}

// .text:0x0015A244 size:0x204 mapped:0x807992D8
void fn_3_15A244(void) {
    return;
}

// .text:0x0015A448 size:0x314 mapped:0x807994DC
void fn_3_15A448(void) {
    return;
}

// .text:0x0015A75C size:0x298 mapped:0x807997F0
void fn_3_15A75C(void) {
    return;
}

// .text:0x0015A9F4 size:0x3A0 mapped:0x80799A88
void fn_3_15A9F4(void) {
    return;
}

// .text:0x0015AD94 size:0x40 mapped:0x80799E28
void fn_3_15AD94(void) {
    if (g_Pitcher.currentStateFrameCounter > lbl_3_data_FC1C) {
        fn_3_750C4(2);
    }
}

// .text:0x0015ADD4 size:0x60 mapped:0x80799E68

void fn_3_15ADD4(void) {
    fn_3_6B870();
    g_GameLogic._135 = 0;
    g_GameLogic._136 = 0;
    if ((g_GameLogic.secondaryGameMode == 0xF) && (g_Strikes.outs >= 3)) {
        g_Strikes.outs = 0;
    }
    fn_3_5A6D4(7);
}

// .text:0x0015AE34 size:0x144 mapped:0x80799EC8
void fn_3_15AE34(void) {
    return;
}

// .text:0x0015AF78 size:0x160 mapped:0x8079A00C
void fn_3_15AF78(void) {
    return;
}

// .text:0x0015B0D8 size:0x12C mapped:0x8079A16C
void fn_3_15B0D8(void) {
    return;
}

// .text:0x0015B204 size:0x290 mapped:0x8079A298
void fn_3_15B204(void) {
    return;
}

// .text:0x0015B494 size:0x17C mapped:0x8079A528
void fn_3_15B494(void) {
    return;
}

// .text:0x0015B610 size:0x18C mapped:0x8079A6A4
void fn_3_15B610(void) {
    return;
}

