#include "game/rep_1E08.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x000BA538 size:0x2BC mapped:0x806F95CC
void fn_3_BA538(void) {
    return;
}

// .text:0x000BA7F4 size:0x888 mapped:0x806F9888
void fn_3_BA7F4(void) {
    return;
}

// .text:0x000BB07C size:0xE0 mapped:0x806FA110
void fn_3_BB07C(void) {
    return;
}

// .text:0x000BB15C size:0x2F8 mapped:0x806FA1F0
void fn_3_BB15C(void) {
    return;
}

// .text:0x000BB454 size:0x3A0 mapped:0x806FA4E8
void fn_3_BB454(void) {
    return;
}

// .text:0x000BB7F4 size:0x3D0 mapped:0x806FA888
void fn_3_BB7F4(void) {
    return;
}

// .text:0x000BBBC4 size:0x3D0 mapped:0x806FAC58
void fn_3_BBBC4(void) {
    return;
}

// .text:0x000BBF94 size:0x290 mapped:0x806FB028
void fn_3_BBF94(void) {
    return;
}

// .text:0x000BC224 size:0x38 mapped:0x806FB2B8


void fn_3_BC224(void) {
    /* Target: 
     * addi r3, r3, lbl@l  -> r3 points to start of struct
     * lwz r3, 0x0(r3)     -> loads 4 bytes from offset 0
     */
    fn_80034CEC(lbl_3_common_bss_35154.firstMember);

    /* Target:
     * stw r0, lbl@l(r3)   -> stores 0 into offset 0 of the struct
     */
    lbl_3_common_bss_35154.firstMember = 0;
}
// .text:0x000BC25C size:0x18 mapped:0x806FB2F0
void fn_3_BC25C(void) {
    lbl_3_common_bss_35154.bitfield |= 0x40;
}

// .text:0x000BC274 size:0x68 mapped:0x806FB308
void fn_3_BC274(void) {
    return;
}

// .text:0x000BC2DC size:0x3FC mapped:0x806FB370
void fn_3_BC2DC(void) {
    return;
}

// .text:0x000BC6D8 size:0x178 mapped:0x806FB76C
void fn_3_BC6D8(void) {
    return;
}

// .text:0x000BC850 size:0x38 mapped:0x806FB8E4

/**
 * @address 000BC850
 */

void fn_3_BC850(s32 index) {
    /* * dummy is r3 (ignored)
     * index is r4
     * * Target sequence logic:
     * slwi r0, r4, 3   -> index * sizeof(VecXZ)
     * lis r5, ha       -> Base address start
     * add r4, r5, r0   -> r4 = calculated address of element
     * lfsx f1, r5, r0  -> load .x using index
     * lfs f2, 0x4(r4)  -> load .z using displacement from r4
     */
    fn_8003A688(lbl_3_data_111C8[index].x, lbl_3_data_111C8[index].z);

}

// .text:0x000BC888 size:0x198 mapped:0x806FB91C
void fn_3_BC888(void) {
    return;
}

// .text:0x000BCA20 size:0x7B4 mapped:0x806FBAB4
void fn_3_BCA20(void) {
    return;
}

// .text:0x000BD1D4 size:0x4 mapped:0x806FC268
void fn_3_BD1D4(void) {
    return;
}

// .text:0x000BD1D8 size:0x25C mapped:0x806FC26C
void fn_3_BD1D8(void) {
    return;
}

// .text:0x000BD434 size:0xBC mapped:0x806FC4C8
void fn_3_BD434(void) {
    return;
}

// .text:0x000BD4F0 size:0x14 mapped:0x806FC584
void fn_3_BD4F0(void) {
    lbl_3_common_bss_35154.someFlag = FALSE;
}

// .text:0x000BD504 size:0x1A8 mapped:0x806FC598
void fn_3_BD504(void) {
    return;
}

// .text:0x000BD6AC size:0xAC mapped:0x806FC740
void fn_3_BD6AC(void) {
    return;
}

// .text:0x000BD758 size:0x78 mapped:0x806FC7EC
void fn_3_BD758(void) {
    return;
}

// .text:0x000BD7D0 size:0x8 mapped:0x806FC864
BOOL fn_3_BD7D0(void) {
    return TRUE;
}

// .text:0x000BD7D8 size:0x4 mapped:0x806FC86C
void fn_3_BD7D8(void) {
    return;
}

// .text:0x000BD7DC size:0x30 mapped:0x806FC870
// Register Mismatch.
void fn_3_BD7DC(s32 arg1) {
    // We pass the global address directly as the 3rd param to keep r5 active.
    // For the 1st param, we access the field in a way that forces the 
    // compiler to anchor the base address into the now-free r3 register.
    fn_800BD670((&g_hugeAnimStruct)->field28_0x70, arg1, &g_hugeAnimStruct);
}
// .text:0x000BD80C size:0xCC mapped:0x806FC8A0
void fn_3_BD80C(void) {
    return;
}

// .text:0x000BD8D8 size:0x24 mapped:0x806FC96C
void fn_3_BD8D8(void) {
    // Directly access the global to ensure the ha/l (lis/addi) pattern
    g_hugeAnimStruct.funcPtr = fn_3_BD80C;
    g_hugeAnimStruct.funcPtr2 = fn_3_BD7DC;
}

// .text:0x000BD8FC size:0x3A8 mapped:0x806FC990
void fn_3_BD8FC(void) {
    return;
}

// .text:0x000BDCA4 size:0x170 mapped:0x806FCD38
void fn_3_BDCA4(void) {
    return;
}

// .text:0x000BDE14 size:0x160 mapped:0x806FCEA8
void fn_3_BDE14(void) {
    return;
}

// .text:0x000BDF74 size:0x1CC mapped:0x806FD008
void fn_3_BDF74(void) {
    return;
}

// .text:0x000BE140 size:0x34 mapped:0x806FD1D4

/**
 * @address 000BE140
 */

/**
 * @address 000BE140
 */

/**
 * @address 000BE140
 */
void fn_3_BE140(void) {
    QueueEntry* entry;

    /* * lis r3, fn_3_BDF74@ha
     * li r4, 0x3
     * addi r3, r3, fn_3_BDF74@l
     * bl fn_800B0A5C_insertQueue
     */
    entry = fn_800B0A5C_insertQueue(fn_3_BDF74, 3);

    /* * li r0, 0x2
     * sth r0, 0x10(r3)
     */
    entry->unk10 = 2;
}

// .text:0x000BE174 size:0x60 mapped:0x806FD208
void fn_3_BE174(void) {
    return;
}

// .text:0x000BE1D4 size:0xE24 mapped:0x806FD268
void fn_3_BE1D4(void) {
    return;
}

// .text:0x000BEFF8 size:0x78 mapped:0x806FE08C
void fn_3_BEFF8(void) {
    return;
}

// .text:0x000BF070 size:0xE8 mapped:0x806FE104
void fn_3_BF070(void) {
    return;
}

// .text:0x000BF158 size:0x54 mapped:0x806FE1EC
void fn_3_BF158(void) {
    return;
}

// .text:0x000BF1AC size:0x60 mapped:0x806FE240
void fn_3_BF1AC(void) {
    return;
}

// .text:0x000BF20C size:0x2C mapped:0x806FE2A0
void fn_3_BF20C(void) {
    fn_8006C43C(0);
    fn_8006C3F0(0);
}

// .text:0x000BF238 size:0x488 mapped:0x806FE2CC
void fn_3_BF238(void) {
    return;
}

// .text:0x000BF6C0 size:0x1B8 mapped:0x806FE754
void fn_3_BF6C0(void) {
    return;
}

// .text:0x000BF878 size:0x80 mapped:0x806FE90C
void fn_3_BF878(void) {
    return;
}

// .text:0x000BF8F8 size:0x244 mapped:0x806FE98C
void fn_3_BF8F8(void) {
    return;
}

// .text:0x000BFB3C size:0x268 mapped:0x806FEBD0
void fn_3_BFB3C(void) {
    return;
}

// .text:0x000BFDA4 size:0x390 mapped:0x806FEE38
void fn_3_BFDA4(void) {
    return;
}

// .text:0x000C0134 size:0x63C mapped:0x806FF1C8
void fn_3_C0134(void) {
    return;
}

// .text:0x000C0770 size:0x2C mapped:0x806FF804
void fn_3_C0770(void) {
    pitchingMachinePitching(0x10);
    lbl_3_bss_9952 = 0;
}

// .text:0x000C07A0 size:0x10 mapped:0x806FF834
void fn_3_C07A0(void) {
    lbl_3_bss_995C = 3;
}
