#include "game/gap_DEB90.h"

#include "UnknownHeaders.h"
#include "game/rep_1838.h"
#include "game/gap_E1964.h"
#include "game/gap_5985C.h"
extern s32 fn_3_90DD8(void);
#include "game/rep_3090.h"
extern void fn_3_FBD58(void);
extern void fn_3_FBD70(void);
#include "static/UnknownHomes_Static.h"

extern void fn_8001D0D0();
extern f32 lbl_3_data_18D98[];
extern void fn_3_10C81C(void);
extern u8 lbl_3_data_18980[];
extern u8 lbl_3_data_18944[];
extern u8 lbl_3_data_189A4[];
extern void fn_3_E8AC8(void);
extern s32 fn_800203E0(s32, s32);
extern s32 fn_80016F7C(void);
extern s32 fn_3_6C938(s32, s32);
extern void changeScene(s32, s32);

typedef struct {
    u8 _pad[0x12];
    u8 _12;
    u8 _13;
} Lbl8037169C;
extern Lbl8037169C lbl_8037169C;

typedef struct GapDEB90Entry2D94 {
    u8 _pad0[0x14];
    f32 field_0x14;
} GapDEB90Entry2D94;

// .text:0x000DEB90 size:0x47C mapped:0x8071DC24
void fn_3_DEB90(void) {
    return;
}

// .text:0x000DF00C size:0x250 mapped:0x8071E0A0
void fn_3_DF00C(void) {
    return;
}

// .text:0x000DF25C size:0x17C mapped:0x8071E2F0
void fn_3_DF25C(void) {
    return;
}

// .text:0x000DF3D8 size:0x230 mapped:0x8071E46C
void fn_3_DF3D8(void) {
    switch (g_GameLogic._125) {
    case 0:
        fn_3_E8AC8();
        fn_800203E0(7, 0);
        g_Minigame.toyField_selectedTurns = (s16)lbl_3_data_189A4[g_Minigame._1A24[0]];
        if (g_d_GameSettings.exhibitionMatchInd == 0) {
            g_Minigame.toyField_selectedTurns = (s16)lbl_3_data_189A4[5];
        }
        g_hugeAnimStruct.renderStadiumFlag = 1;
        g_hugeAnimStruct.field209_0x2d8e = 0;
        g_hugeAnimStruct.field269_0x307a = 3;
        if (g_Minigame._190A != 0) {
            g_GameLogic._125 = 1;
            return;
        }
        fn_3_59B20();
        g_GameLogic._125 = 2;
        break;
    case 1:
        fn_3_10C81C();
        if ((s8)g_Minigame._1A0F >= 0) {
            return;
        }
        fn_3_59B20();
        g_GameLogic._125 = 2;
        break;
    case 2:
        if (fn_3_59AE4() == 0) return;
        g_GameLogic._125 = 3;
        if (fn_80016F7C() == 0) return;
        if (g_Minigame._1A38 != 0) {
            g_GameLogic._125 = 6;
        } else {
            g_GameLogic._125 = 4;
        }
        lbl_3_common_bss_34C58._2C = 0;
        break;
    case 3:
        if (fn_3_90DD8() == 0) return;
        g_GameLogic._125 = 6;
        break;
    case 4:
        if (lbl_8037169C._12 == 0) return;
        if (g_GameLogic.FrameCountOfCurrentPitch < 0xF0) {
            if (fn_3_FD9FC() == 0) return;
        }
        if (fn_3_6C938(1, 0x1300) == 0) return;
        g_GameLogic._125 = 7;
        break;
    case 6:
        changeScene(3, 6);
        if (lbl_8037169C._13 == 0) return;
        fn_3_FBD70();
        fn_3_FBD58();
        g_GameLogic._125 = 8;
        break;
    case 7:
    case 8:
        fn_3_E1964();
        g_Minigame._1A38 = 0;
        g_Minigame._19A2 = 0;
        g_hugeAnimStruct.field269_0x307a = 1;
        fn_3_5A6D4(6);
        break;
    }
}

// .text:0x000DF608 size:0xCC mapped:0x8071E69C
void fn_3_DF608(void) {
    MiniGameStruct* mg;
    u8* p;
    u8 idx;
    idx = g_d_GameSettings._35;
    (&g_Minigame._19DA)[idx] = 0;
    mg = &g_Minigame;
    p = lbl_3_data_18980;
    if (idx != 0) {
        mg->minigameControlStruct[0].aIStrength[0] = *p;
        p++;
    }
    if (idx != 1) {
        mg->minigameControlStruct[0].aIStrength[1] = *p;
        p++;
    }
    if (idx != 2) {
        mg->minigameControlStruct[0].aIStrength[2] = *p;
        p++;
    }
    if (idx != 3) {
        mg->minigameControlStruct[0].aIStrength[3] = *p;
    }
    mg->_1907 = 1;
    mg->multiPlayerInd = 1;
    mg->miniGameNumberOfParticipants = lbl_3_data_18944[mg->GameMode_MiniGame] + 1;
    g_Scores.threshold = 1;
}

// .text:0x000DF6D4 size:0x14C mapped:0x8071E768
void fn_3_DF6D4(void) {
    return;
}

// .text:0x000DF820 size:0xB4 mapped:0x8071E8B4
void fn_3_DF820(void) {
    s32 count;
    s32 i;
    fn_3_10C81C();
    count = 0;
    for (i = 0; i < 4; i++) {
        if ((s8)g_Minigame.teamEntries[i].fieldA != (s8)g_Minigame.teamEntries[i].fieldB) break;
        count++;
    }
    if (count >= 4) {
        fn_3_5A6D4(0x1d);
    }
}

// .text:0x000DF8D4 size:0x14C mapped:0x8071E968
void fn_3_DF8D4(void) {
    return;
}

// .text:0x000DFA20 size:0x18C mapped:0x8071EAB4
void fn_3_DFA20(void) {
    return;
}

// .text:0x000DFBAC size:0xABC mapped:0x8071EC40
void fn_3_DFBAC(void) {
    return;
}

// .text:0x000E0668 size:0xF0 mapped:0x8071F6FC
void fn_3_E0668(void) {
    return;
}

// .text:0x000E0758 size:0x84 mapped:0x8071F7EC
void fn_3_E0758(void) {
    s32 offset;
    GapDEB90Entry2D94* entry;
    s32 i;

    i = 0;
    offset = 0;
    do {
        entry = (GapDEB90Entry2D94*)((u8*)g_hugeAnimStruct.entries_2D94 + offset);
        fn_8001D0D0(i, lbl_3_data_18D98[0]);
        {
            u32 r = (u32)rand();
            u32 shifted = r << 20;
            r >>= 31;
            shifted -= r;
            shifted = (shifted << 12) | (shifted >> 20);
            shifted += r;
            entry->field_0x14 = shortAngleToRad_Capped((s16)shifted);
        }
        i++;
        offset += 0x28;
    } while (i < 0x1E);
}

// .text:0x000E07DC size:0xA04 mapped:0x8071F870
void fn_3_E07DC(void) {
    return;
}
