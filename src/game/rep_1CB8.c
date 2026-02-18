#include "game/rep_1CB8.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x000B79AC size:0x280 mapped:0x806F6A40
void fn_3_B79AC(void) {
    return;
}

// .text:0x000B7C2C size:0xB0 mapped:0x806F6CC0
void fn_3_B7C2C(void) {
    return;
}

// .text:0x000B7CDC size:0x90 mapped:0x806F6D70
void fn_3_B7CDC(void) {
    return;
}

// .text:0x000B7D6C size:0x6C mapped:0x806F6E00
void fn_3_B7D6C(void) {
    return;
}

// .text:0x000B7DD8 size:0x38 mapped:0x806F6E6C


/**
 * @address 000B7DD8
 */
s32 fn_3_B7DD8(f32 f1, f32 f2) {
    /* Target: lfs f3, 0x14(r3) */
    f32 limit = lbl_3_data_4444.unk14;
    
    /* Target: fadds f0, f1, f3 -> fcmpo cr0, f2, f0 -> ble .L_000B7DF8 */
    if (f2 > f1 + limit) {
        /* Target: li r3, 1 -> blr */
        return 1;
    }

    /* Target: fneg f0, f1 
       Target: fadds f0, f0, f3
       Target: fcmpo cr0, f2, f0
       Target: mfcr r0 -> extrwi r3, r0, 1, 1 
       
       Writing it exactly as (-f1 + limit) forces the compiler to negate f1 
       separately before adding, matching the fneg/fadds sequence. */
    return f2 > (-f1 + limit);
}

// .text:0x000B7E10 size:0x34 mapped:0x806F6EA4

/**
 * @address 000B7E10
 */

/**
 * @address 000B7E10
 */
s32 fn_3_B7E10(f32 f1, f32 f2) {
    /* * The first check (f2 < f1 - rodata) triggers a conditional branch.
     * li r3, 1 followed by blr is the standard return for the true case.
     */
    if (f2 < f1 - lbl_3_rodata_1D3C) {
        return 1;
    }

    /* * The second check (f2 < -f1 - rodata) uses branchless logic.
     * fneg f0, f1 -> fsubs f0, f0, f3 -> fcmpo -> mfcr -> srwi.
     * CodeWarrior generates this pattern for boolean normalization 
     * using the Condition Register.
     */
    return f2 < -f1 - lbl_3_rodata_1D3C;
}

// .text:0x000B7E44 size:0xAC mapped:0x806F6ED8
void fn_3_B7E44(void) {
    return;
}
