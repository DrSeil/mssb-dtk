#include "game/gap_106DFC.h"
#include "game/UnknownHomes_Game.h"

void fn_3_90064(s32 param);

// .text:0x00106DFC size:0x54 mapped:0x80745E90
void fn_3_106DFC(void) {
    g_Camera.unkAB0 = _OSAllocFromHeap(4, 0x8000);
    g_Camera.unk146C = _OSAllocFromHeap(4, 0x8000);
}

// .text:0x00106E50 size:0x60 mapped:0x80745EE4
void fn_3_106E50(void) {
    return;
}

// .text:0x00106EB0 size:0x24 mapped:0x80745F44
void fn_3_106EB0(void) {
    fn_3_90064(0x30B);
}

// .text:0x00106ED4 size:0x1A4 mapped:0x80745F68
void fn_3_106ED4(void) {
    return;
}

// .text:0x00107078 size:0x2C mapped:0x8074610C

void fn_3_107078(void) {
    if (*((u8*) &g_Minigame + (s8) g_Minigame._1908 + 0x18E8) == 1) {
        g_Minigame._1A3F = 1;
    }
}

// .text:0x001070A4 size:0x188 mapped:0x80746138
void fn_3_1070A4(void) {
    return;
}

// .text:0x0010722C size:0x320 mapped:0x807462C0
void fn_3_10722C(void) {
    return;
}

// .text:0x0010754C size:0x140 mapped:0x807465E0
void fn_3_10754C(void) {
    return;
}

// .text:0x0010768C size:0xF8 mapped:0x80746720
void fn_3_10768C(void) {
    return;
}

// .text:0x00107784 size:0x174 mapped:0x80746818
void fn_3_107784(void) {
    return;
}

// .text:0x001078F8 size:0x90 mapped:0x8074698C
void fn_3_1078F8(void) {
    return;
}

// .text:0x00107988 size:0x40 mapped:0x80746A1C
s32 fn_3_107988(u32 arg0) {
    MiniGameStruct* s = &g_Minigame;
    u32 count = s->field_1E26[4] - 1;
    if (count <= 0) {
        return 0;
    }
    do {
        if (s->_1DFC[0x20] == arg0) {
            return 1;
        }
        s = (MiniGameStruct*)((u8*)s + 1);
    } while (--count != 0);
    return 0;
}

// .text:0x001079C8 size:0x1D4 mapped:0x80746A5C
void fn_3_1079C8(void) {
    return;
}

// .text:0x00107B9C size:0x34 mapped:0x80746C30
s32 fn_3_107B9C(u8* a, u8* b) {
    u8 ib = *b;
    u8 ia = *a;
    u8 keyB = g_Minigame.field_1E26[ib];
    u8 keyA = g_Minigame.field_1E26[ia];
    if (keyB == keyA) {
        return ia - ib;
    }
    return keyB - keyA;
}

// .text:0x00107BD0 size:0x34 mapped:0x80746C64
s32 fn_3_107BD0(u8* a, u8* b) {
    u8 ib = *b;
    u8 ia = *a;
    u8 keyB = g_Minigame.field_1E22[ib];
    u8 keyA = g_Minigame.field_1E22[ia];
    if (keyB == keyA) {
        return ia - ib;
    }
    return keyB - keyA;
}

// .text:0x00107C04 size:0x3C mapped:0x80746C98
s32 fn_3_107C04(u8* a, u8* b) {
    u8 ib = *b;
    u8 ia = *a;
    s16 keyB = g_Minigame.miniGameCurrentPoints[ib];
    s16 keyA = g_Minigame.miniGameCurrentPoints[ia];
    if (keyB == keyA) {
        return ia - ib;
    }
    return keyB - keyA;
}

// .text:0x00107C40 size:0x48 mapped:0x80746CD4
s32 fn_3_107C40(void) {
    s16* p = &g_Minigame.miniGameCurrentPoints[1];
    u8 count = g_Minigame.miniGameNumberOfParticipants;
    s16 first = g_Minigame.miniGameCurrentPoints[0];
    s32 n = count - 1;
    if (count > 1) {
        do {
            if (*p != first) return 0;
            p++;
        } while (--n);
    }
    return 1;
}

// .text:0x00107C88 size:0x48 mapped:0x80746D1C
s32 fn_3_107C88(void) {
    s32* p = &g_Minigame.minigamePoints_current_Latest[1];
    u8 count = g_Minigame.miniGameNumberOfParticipants;
    s16 first = (s16)(g_Minigame.minigamePoints_current_Latest[0] >> 16);
    s32 n = count - 1;
    if (count > 1) {
        do {
            if ((s16)(*p >> 16) != first) return 0;
            p++;
        } while (--n);
    }
    return 1;
}

// .text:0x00107CD0 size:0x64 mapped:0x80746D64
void fn_3_107CD0(void) {
    return;
}

// .text:0x00107D34 size:0x3C mapped:0x80746DC8
s32 fn_3_107D34(u8* a, u8* b) {
    u8 ib = *b;
    u8 ia = *a;
    s16 keyB = (s16)(g_Minigame.minigamePoints_current_Latest[ib] >> 16);
    s16 keyA = (s16)(g_Minigame.minigamePoints_current_Latest[ia] >> 16);
    if (keyB == keyA) {
        return ia - ib;
    }
    return keyB - keyA;
}

// .text:0x00107D70 size:0x44 mapped:0x80746E04
u8 fn_3_107D70(s8 arg0) {
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_MINIGAMES && arg0 >= 0 && arg0 < 4) {
        return g_Minigame.player_states[arg0];
    }
    return 0;
}

// .text:0x00107DB4 size:0x44 mapped:0x80746E48
u8 fn_3_107DB4(s8 arg0) {
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_MINIGAMES && arg0 >= 0 && arg0 < 4) {
        return g_Minigame.field_1DC4[arg0];
    }
    return 0;
}

// .text:0x00107DF8 size:0x44 mapped:0x80746E8C
u8 fn_3_107DF8(s8 arg0) {
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_MINIGAMES && arg0 >= 0 && arg0 < 4) {
        return g_Minigame.field_1DC0[arg0];
    }
    return 0;
}

// .text:0x00107E3C size:0x44 mapped:0x80746ED0
u8 minigame_checkIfAIInputIs_Algorithmic_Or_ControllerBased(s8 arg0) {
    if (g_d_GameSettings.GameModeSelected == GAME_TYPE_MINIGAMES && arg0 >= 0 && arg0 < 4) {
        return g_Minigame._1DBC[arg0];
    }
    return 0;
}

// .text:0x00107E80 size:0x3B0 mapped:0x80746F14
void fn_3_107E80(void) {
    return;
}

// .text:0x00108230 size:0x284 mapped:0x807472C4
void fn_3_108230(void) {
    return;
}

// .text:0x001084B4 size:0x3A0 mapped:0x80747548
void fn_3_1084B4(void) {
    return;
}

// .text:0x00108854 size:0x194 mapped:0x807478E8
void fn_3_108854(void) {
    return;
}

// .text:0x001089E8 size:0x26C mapped:0x80747A7C
void fn_3_1089E8(void) {
    return;
}

// .text:0x00108C54 size:0x600 mapped:0x80747CE8
void fn_3_108C54(void) {
    return;
}

// .text:0x00109254 size:0x2D8 mapped:0x807482E8
void fn_3_109254(void) {
    return;
}

// .text:0x0010952C size:0x7BC mapped:0x807485C0
void fn_3_10952C(void) {
    return;
}

// .text:0x00109CE8 size:0xA0 mapped:0x80748D7C
void fn_3_109CE8(void) {
    return;
}

// .text:0x00109D88 size:0x58 mapped:0x80748E1C
MinigameModeData* fn_3_109D88(void) {
    u8 mode;
    if (g_Minigame._1A3C != 0) {
        return &lbl_803616CC.modes[6];
    }
    mode = g_Minigame.GameMode_MiniGame;
    if (mode != 0) {
        return &lbl_803616CC.modes[mode - 1];
    }
    return &lbl_803616CC.base;
}

// .text:0x00109DE0 size:0x23C mapped:0x80748E74
void fn_3_109DE0(void) {
    return;
}

// .text:0x0010A01C size:0x84 mapped:0x807490B0
void fn_3_10A01C(void) {
    return;
}

// .text:0x0010A0A0 size:0xCA8 mapped:0x80749134
void fn_3_10A0A0(void) {
    return;
}

// .text:0x0010AD48 size:0xD0 mapped:0x80749DDC
void fn_3_10AD48(void) {
    return;
}

// .text:0x0010AE18 size:0xD8 mapped:0x80749EAC
void fn_3_10AE18(void) {
    return;
}

// .text:0x0010AEF0 size:0x310 mapped:0x80749F84
void fn_3_10AEF0(void) {
    return;
}

// .text:0x0010B200 size:0x7C mapped:0x8074A294
void fn_3_10B200(void) {
    return;
}

// .text:0x0010B27C size:0x654 mapped:0x8074A310
void fn_3_10B27C(void) {
    return;
}

// .text:0x0010B8D0 size:0x5AC mapped:0x8074A964
void fn_3_10B8D0(void) {
    return;
}

// .text:0x0010BE7C size:0x5D4 mapped:0x8074AF10
void fn_3_10BE7C(void) {
    return;
}

// .text:0x0010C450 size:0x13C mapped:0x8074B4E4
void fn_3_10C450(void) {
    return;
}

// .text:0x0010C58C size:0x218 mapped:0x8074B620
void fn_3_10C58C(void) {
    return;
}

// .text:0x0010C7A4 size:0x78 mapped:0x8074B838
void fn_3_10C7A4(void) {
    return;
}

// .text:0x0010C81C size:0x404 mapped:0x8074B8B0
void fn_3_10C81C(void) {
    return;
}

// .text:0x0010CC20 size:0x19EC mapped:0x8074BCB4
void fn_3_10CC20(void) {
    return;
}

// .text:0x0010E60C size:0x9A0 mapped:0x8074D6A0
void fn_3_10E60C(void) {
    return;
}

// .text:0x0010EFAC size:0x228 mapped:0x8074E040
void fn_3_10EFAC(void) {
    return;
}

// .text:0x0010F1D4 size:0x204 mapped:0x8074E268
void fn_3_10F1D4(void) {
    return;
}

// .text:0x0010F3D8 size:0x178 mapped:0x8074E46C
void fn_3_10F3D8(void) {
    return;
}

// .text:0x0010F550 size:0x14 mapped:0x8074E5E4
void fn_3_10F550(u8 arg0, s16 arg1) {
    g_Minigame._1A41 = arg0;
    g_Minigame.someGraphicFrameCountdown = arg1;  /* stb then sth */
}

// .text:0x0010F564 size:0x58 mapped:0x8074E5F8
void fn_3_10F564(void) {
    return;
}

// .text:0x0010F5BC size:0xC8 mapped:0x8074E650
void fn_3_10F5BC(void) {
    return;
}

// .text:0x0010F684 size:0x298 mapped:0x8074E718
void fn_3_10F684(void) {
    return;
}

// .text:0x0010F91C size:0x258 mapped:0x8074E9B0
void fn_3_10F91C(void) {
    return;
}

// .text:0x0010FB74 size:0x70 mapped:0x8074EC08
void fn_3_10FB74(void) {
    return;
}

// .text:0x0010FBE4 size:0x1E4 mapped:0x8074EC78
void fn_3_10FBE4(void) {
    return;
}

// .text:0x0010FDC8 size:0x6E0 mapped:0x8074EE5C
void fn_3_10FDC8(void) {
    return;
}

// .text:0x001104A8 size:0x2C mapped:0x8074F53C
void fn_3_1104A8(void) {
    s8 i = 0;
    do {
        g_Minigame._1DBC[i] = 0;
        i++;
    } while (i < 4);
}

// .text:0x001104D4 size:0x160 mapped:0x8074F568
void fn_3_1104D4(void) {
    return;
}

