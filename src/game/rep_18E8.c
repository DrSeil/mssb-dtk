#include "game/rep_18E8.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x000A009C size:0x1C68 mapped:0x806DF130
void fn_3_A009C(void) {
    return;
}

// .text:0x000A1D04 size:0x9C mapped:0x806E0D98
void fn_3_A1D04(void) {
    return;
}

// .text:0x000A1DA0 size:0x19C mapped:0x806E0E34
void fn_3_A1DA0(void) {
    return;
}

// .text:0x000A1F3C size:0x10C mapped:0x806E0FD0
void fn_3_A1F3C(void) {
    return;
}

// .text:0x000A2048 size:0x1E4 mapped:0x806E10DC
void fn_3_A2048(void) {
    return;
}

// .text:0x000A222C size:0x1D8 mapped:0x806E12C0
void fn_3_A222C(void) {
    return;
}

// .text:0x000A2404 size:0x1C0 mapped:0x806E1498
void fn_3_A2404(void) {
    return;
}

// .text:0x000A25C4 size:0x398 mapped:0x806E1658
void fn_3_A25C4(void) {
    return;
}

// .text:0x000A295C size:0x210 mapped:0x806E19F0
void fn_3_A295C(void) {
    return;
}

// .text:0x000A2B6C size:0x130 mapped:0x806E1C00
void fn_3_A2B6C(void) {
    return;
}

// .text:0x000A2C9C size:0x140 mapped:0x806E1D30
void fn_3_A2C9C(void) {
    return;
}

// .text:0x000A2DDC size:0x1FC mapped:0x806E1E70
void fn_3_A2DDC(void) {
    return;
}

// .text:0x000A2FD8 size:0x210 mapped:0x806E206C
void fn_3_A2FD8(void) {
    return;
}

// .text:0x000A31E8 size:0xD0 mapped:0x806E227C
void fn_3_A31E8(void) {
    return;
}

// .text:0x000A32B8 size:0xBC mapped:0x806E234C
void fn_3_A32B8(void) {
    return;
}

// .text:0x000A3374 size:0x348 mapped:0x806E2408
void fn_3_A3374(void) {
    return;
}

// .text:0x000A36BC size:0x70 mapped:0x806E2750
void fn_3_A36BC(void) {
    return;
}

// .text:0x000A372C size:0x3C mapped:0x806E27C0

#include "types.h"

extern InMemBallType g_Ball;
extern f32 lbl_3_rodata_1998;

/**
 * @address 000A372C
 */
s32 fn_3_A372C(void) {
    /* * To match target:
     * 1. Constant address -> r3 (lis r3)
     * 2. Ball address -> r4 (lis r4)
     * 3. Load ball.x into f2 and update r4 (lfsu f2)
     * 4. Load constant from r3 into f1 (lfs f1)
     * 5. Load ball.z from updated r4 into f3 (lfs f3)
     */
    f32 ballX = g_Ball.AtBat_Contact_BallPos.x;

    f32 ballZ = g_Ball.AtBat_Contact_BallPos.z;
    f32 constant = lbl_3_rodata_1998;



    /* fadds f0, f1, f2 -> f1 must be first operand (constant + ballX) */
    if (ballZ < (constant + ballX)) {
        /* fsubs f0, f1, f2 -> f1 must be first operand (constant - ballX) */
        if (ballZ < (constant - ballX)) {
            return 1;
        }
    }

    return 0;
}

// .text:0x000A3768 size:0x54 mapped:0x806E27FC
void fn_3_A3768(void) {
    return;
}

// .text:0x000A37BC size:0x90 mapped:0x806E2850
void fn_3_A37BC(void) {
    return;
}

// .text:0x000A384C size:0x2E4 mapped:0x806E28E0
void fn_3_A384C(void) {
    return;
}

// .text:0x000A3B30 size:0xD0 mapped:0x806E2BC4
void fn_3_A3B30(void) {
    return;
}

// .text:0x000A3C00 size:0xC0 mapped:0x806E2C94
void fn_3_A3C00(void) {
    return;
}

// .text:0x000A3CC0 size:0x498 mapped:0x806E2D54
void fn_3_A3CC0(void) {
    return;
}

// .text:0x000A4158 size:0x90 mapped:0x806E31EC
void fn_3_A4158(void) {
    return;
}

// .text:0x000A41E8 size:0x4B8 mapped:0x806E327C
void fn_3_A41E8(void) {
    return;
}

// .text:0x000A46A0 size:0x370 mapped:0x806E3734
void fn_3_A46A0(void) {
    return;
}

// .text:0x000A4A10 size:0x540 mapped:0x806E3AA4
void fn_3_A4A10(void) {
    return;
}

// .text:0x000A4F50 size:0x48C mapped:0x806E3FE4
void fn_3_A4F50(void) {
    return;
}

// .text:0x000A53DC size:0x328 mapped:0x806E4470
void fn_3_A53DC(void) {
    return;
}

// .text:0x000A5704 size:0x448 mapped:0x806E4798
void fn_3_A5704(void) {
    return;
}

// .text:0x000A5B4C size:0x898 mapped:0x806E4BE0
void fn_3_A5B4C(void) {
    return;
}

// .text:0x000A63E4 size:0x404 mapped:0x806E5478
void fn_3_A63E4(void) {
    return;
}

// .text:0x000A67E8 size:0x28 mapped:0x806E587C
void fn_3_A67E8(s32 index) {
    lbl_3_bss_1838[index] = 9;
    lbl_3_bss_1828[index] = -1;
}

// .text:0x000A6810 size:0x2AC mapped:0x806E58A4
void fn_3_A6810(void) {
    return;
}

// .text:0x000A6ABC size:0x28C mapped:0x806E5B50
void fn_3_A6ABC(void) {
    return;
}

// .text:0x000A6D48 size:0x150 mapped:0x806E5DDC
void fn_3_A6D48(void) {
    return;
}

// .text:0x000A6E98 size:0x1A8 mapped:0x806E5F2C
void fn_3_A6E98(void) {
    return;
}

// .text:0x000A7040 size:0x674 mapped:0x806E60D4
void fn_3_A7040(void) {
    return;
}

// .text:0x000A76B4 size:0x5D4 mapped:0x806E6748
void fn_3_A76B4(void) {
    return;
}

// .text:0x000A7C88 size:0x270 mapped:0x806E6D1C
void fn_3_A7C88(void) {
    return;
}

// .text:0x000A7EF8 size:0x17C mapped:0x806E6F8C
void fn_3_A7EF8(void) {
    return;
}

// .text:0x000A8074 size:0x2C4 mapped:0x806E7108
void fn_3_A8074(void) {
    return;
}

// .text:0x000A8338 size:0x140 mapped:0x806E73CC
void fn_3_A8338(void) {
    return;
}

// .text:0x000A8478 size:0x150 mapped:0x806E750C
void fn_3_A8478(void) {
    return;
}

// .text:0x000A85C8 size:0x40C mapped:0x806E765C
void fn_3_A85C8(void) {
    return;
}

// .text:0x000A89D4 size:0x980 mapped:0x806E7A68
void fn_3_A89D4(void) {
    return;
}

// .text:0x000A9354 size:0x3A8 mapped:0x806E83E8
void fn_3_A9354(void) {
    return;
}

// .text:0x000A96FC size:0x288 mapped:0x806E8790
void fn_3_A96FC(void) {
    return;
}

// .text:0x000A9984 size:0x2F0 mapped:0x806E8A18
void fn_3_A9984(void) {
    return;
}

// .text:0x000A9C74 size:0xAC mapped:0x806E8D08
void fn_3_A9C74(void) {
    return;
}

// .text:0x000A9D20 size:0xD1C mapped:0x806E8DB4
void fn_3_A9D20(void) {
    return;
}

// .text:0x000AAA3C size:0x1BC mapped:0x806E9AD0
void fn_3_AAA3C(void) {
    return;
}

// .text:0x000AABF8 size:0x8C mapped:0x806E9C8C
void fn_3_AABF8(void) {
    return;
}

// .text:0x000AAC84 size:0x36C mapped:0x806E9D18
void fn_3_AAC84(void) {
    return;
}

// .text:0x000AAFF0 size:0x564 mapped:0x806EA084
void fn_3_AAFF0(void) {
    return;
}

// .text:0x000AB554 size:0x5C mapped:0x806EA5E8
void fn_3_AB554(void) {
    return;
}

// .text:0x000AB5B0 size:0x820 mapped:0x806EA644
void fn_3_AB5B0(void) {
    return;
}

// .text:0x000ABDD0 size:0xC28 mapped:0x806EAE64
void fn_3_ABDD0(void) {
    return;
}