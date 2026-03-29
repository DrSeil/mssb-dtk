#include "game/rep_4138.h"
#include "stl/mem.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x0016D810 size:0x1A0 mapped:0x807AC8A4
void fn_3_16D810(void) {
    return;
}

// .text:0x0016D9B0 size:0x1BC mapped:0x807ACA44
void fn_3_16D9B0(void) {
    return;
}

// .text:0x0016DB6C size:0x458 mapped:0x807ACC00
void fn_3_16DB6C(void) {
    return;
}

// .text:0x0016DFC4 size:0x1DC mapped:0x807AD058
void fn_3_16DFC4(void) {
    return;
}

// .text:0x0016E1A0 size:0x4C mapped:0x807AD234
void fn_3_16E1A0(void) {
    // Note: Local variables to hold loaded values.
    // This prevents the compiler from storing too early.
    s32 s1, s0, inn;
    s32 str, bal, out;

    // Register Steering: Initialization order anchors r6, r5, and r8.
    Scores* scores = &g_Scores;           // Anchors scores base to r6
    Strikes* strikes = &g_Strikes;        // Anchors strikes base to r5
    GameStateBss* dest = &lbl_3_bss_D6F0; // Anchors destination base to r8

    // PERFORM ALL LOADS FIRST
    // The sequence of 'lha' and 'lwz' must match the target register indices
    s1 = scores->ScoreTeam1; // lha r9, 0x2a(r6)
    s0 = scores->ScoreTeam0; // lha r7, 0x4(r6)
    inn = scores->highscore;    // lwz r6, 0x0(r6) -- Note r6 is overwritten here;

    str = strikes->strikes;  // lwz r4, 0x0(r5)
    bal = strikes->balls;    // lwz r3, 0x4(r5)
    out = strikes->outs;     // lwz r0, 0x8(r5)

    // PERFORM ALL STORES SECOND
    // stw r9, 0x0(r8) -> stw r7, 0x4(r8) ...
    dest->val0 = s1;
    dest->val1 = s0;
    dest->val2 = inn;
    dest->val3 = str;
    dest->val4 = bal;
    dest->val5 = out;
}

// .text:0x0016E1EC size:0x110 mapped:0x807AD280
void fn_3_16E1EC(void) {
    return;
}

// .text:0x0016E2FC size:0x2C mapped:0x807AD390
void fn_3_16E2FC(u16 *ptr, s32 index) {
    if (ptr == NULL) {
        return;
    }

    // lhz r5, 0(r3) -> subi r0, r5, 1 -> cmpw r0, r4
    if ((s32)(ptr[0] - 1) < index) {
        return;
    }

    // Direct access triggers lis/stw for each global
    lbl_3_bss_D6E4 = ptr;
    lbl_3_bss_D6E8 = index;
}

// .text:0x0016E328 size:0x10 mapped:0x807AD3BC
void fn_3_16E328(void) {
    lbl_3_bss_D6EC = TRUE;
}

// .text:0x0016E338 size:0x6C mapped:0x807AD3CC

void fn_3_16E338(u16 *arg0, s32 arg1) {
    if ((arg0 != NULL) && ((s32)(*arg0 - 1) >= arg1)) {
        D6E0_Struct *p = &lbl_3_bss_D6E0;
        p->unk4 = (s32)arg0;
        p->unk8 = arg1;
        p->unkC = 0;
        memset((u8 *)p + 0x10, 0, 0x18);
        fn_800B0A5C_insertQueue(fn_3_16E1EC, 5);
    }
}
