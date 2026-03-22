#include "game/rep_1E08.h"
#include "game/rep_2390.h"
#include "game/rep_3C28.h"
// Forward declaration for fn_3_160814 (not found in headers)
void fn_3_160814(void);
#include <string.h>
#include "game/rep_2390.h"
// Forward declaration for fn_3_160814 (not found in headers)
void fn_3_160814(void);
#include <string.h>
#include "game/rep_2390.h"
#include <string.h>
#include <string.h>
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/UnknownHomes_Game.h"
#include "static/UnknownHomes_static.h"
#include "game/rep_1FD8.h"
#include "game/sta_c2.h"
#include "game/sta_c4.h"
#include "game/rep_1F58.h"
#include "game/rep_F80.h"
#include "game/rep_3AE8.h"

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
s32 fn_3_BC274(Rep1E08Obj* arg0, VecXZ* arg1, VecXZ* arg2) {
    return (arg1->z - arg2->z) < (f32)lbl_3_data_17000[(s8)arg0->field_0x252] / lbl_3_rodata_1EC4;
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

void fn_3_BC850(s32 dummy, s32 index) {
    fn_8003A688(dummy, lbl_3_data_111C8[index].x, lbl_3_data_111C8[index].z);

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
void fn_3_BD434(s32 arg0, s32 arg1) {
    DataArrayElement* arr;
    s32 i;

    arr = lbl_3_data_12354[arg0 + arg1 * 7].elements;
    lbl_3_common_bss_35154.unk_3b8 = arr;
    i = 0;
    lbl_3_common_bss_35154.unk_3cc = 0x1518;
    lbl_3_common_bss_35154.unk_3c8 = 0;
    lbl_3_common_bss_35154.unk_3d0 = lbl_3_rodata_1F18;
    lbl_3_common_bss_35154.unk_3d4 = lbl_3_rodata_1F1C;
    lbl_3_common_bss_35154.unk_3d8 = lbl_3_rodata_1E64;

    while (arr->flag < 4) {
        arr++;
        i++;
    }

    lbl_3_common_bss_35154.unk_3e0 = 1;
    lbl_3_common_bss_35154.unk_3bc = lbl_3_common_bss_35154.unk_3b8[i].x;
    lbl_3_common_bss_35154.unk_3c0 = lbl_3_common_bss_35154.unk_3b8[i].y;
    lbl_3_common_bss_35154.unk_3c4 = lbl_3_common_bss_35154.unk_3b8[i].z;
    lbl_3_common_bss_35154.unk_3dc = lbl_3_rodata_1F20;
}

// .text:0x000BD4F0 size:0x14 mapped:0x806FC584
void fn_3_BD4F0(void) {
    lbl_3_common_bss_35154.someFlag = FALSE;
}

// .text:0x000BD504 size:0x1A8 mapped:0x806FC598
void fn_3_BD504(s32 flag, f32 f1, f32 f2, f32 f3) {
    // Check if lbl_80366158.unk28 == 0 (byte at offset 0x28)
    if (((u8*)&lbl_80366158)[0x28] == 0) {
        // Copy 3 floats (12 bytes) from unk_440 to unk_464 area
        memcpy(&lbl_3_common_bss_35154.unk_464, &lbl_3_common_bss_35154.unk_440, 0xC);

        // Store the input parameters to unk_440, unk_444, unk_448
        lbl_3_common_bss_35154.unk_440 = f1;
        lbl_3_common_bss_35154.unk_444 = f2;
        lbl_3_common_bss_35154.unk_448 = f3;
    }

    // Check someFlag
    if (lbl_3_common_bss_35154.someFlag != 0) {
        // flag parameter determines ball vs pitcher:
        // flag == 0 -> use g_Ball.currentStarSwing
        // flag != 0 -> use g_Pitcher.starPitchType
        s32 starType;
        if (flag == 0) {
            starType = g_Ball.currentStarSwing;
        } else {
            starType = g_Pitcher.starPitchType;
        }

        if (starType >= 0x0D) {
            return;
        }
        if (starType >= 0x0B) {
            fn_3_15BAA0(starType == 0xC);
            return;
        }

        // Implement the exact branching structure from the assembly
        // Based on the assembly analysis, this is the decision tree:

        if (starType < 5) {           // 0-4 range: 000BD54C-000BD5B8
            if (starType < 1) {       // 0: 000BD594-000BD5B8 (fall through to end)
                // Do nothing, fall through to end
            } else if (starType < 3) { // 1-2: 000BD5AC-000BD5B0
                fn_3_CB538();          // 000BD5B4-000BD5B8
            } else {                   // 3-4: 000BD5B0-000BD5B8
                fn_3_160814();         // 000BD5B8-000BD5BC (actually goes to 0xBD5F4 then 0xBD620)
            }
        } else if (starType < 9) {    // 5-8 range: 000BD5C0-000BD5F8
            if (starType < 7) {       // 5-6: 000BD5C8-000BD5F8
                fn_3_160814();         // 000BD5F8-000BD5FC
            } else {                   // 7-8: 000BD5FC-000BD604
                fn_3_15F574();         // 000BD604-000BD608
            }
        } else {                      // 9-C range: 000BD610-000BD664
            if (starType < 0xB) {     // 9-A: 000BD618-000BD664
                fn_3_15F574();         // 000BD620-000BD624
            } else {                   // B-C: 000BD624-000BD664
                // 0xBD654: subfic r0, r3, 0xc / cntlzw / srwi r3, r0, 5
                // This maps:
                // starType 0xB (11) -> 0xFFFFFFF5 -> cntlzw -> 0 -> srwi 5 -> 0
                // starType 0xC (12) -> 0xFFFFFFF4 -> cntlzw -> 1 -> srwi 5 -> 0
                // Both result in 0 being passed to fn_3_15B79C
                fn_3_15B79C();         // 000BD660-000BD664
            }
        }
    }
    // else: someFlag is 0, do nothing else

    // If we took the first branch (lbl_80366158.unk28 == 0), increment unk_464
    if (((u8*)&lbl_80366158)[0x28] == 0) {
        lbl_3_common_bss_35154.unk_464 += 1;
    }
}

// .text:0x000BD6AC size:0xAC mapped:0x806FC740
void fn_3_BD6AC(s32 arg0, f32 f1, f32 f2, f32 f3) {
    // Almost matches - bge+b+bl branch direction mismatch (CW codegen, not fixable from C)
    s32 starType;
    lbl_3_common_bss_35154.someFlag = 1;
    lbl_3_common_bss_35154.unk_440 = f1;
    lbl_3_common_bss_35154.unk_444 = f2;
    lbl_3_common_bss_35154.unk_448 = f3;
    lbl_3_common_bss_35154.unk_464 = 0;
    if (arg0 != 0) {
        starType = g_Ball.currentStarSwing;
        if (starType >= 0xd)
            return;
        if (starType >= 0xb)
            fn_3_15BAA0(starType == 0xc);
        return;
    }
    starType = g_Pitcher.starPitchType;
    if (starType >= 0xd)
        return;
    if (starType >= 0xb)
        fn_3_15BAA0(starType == 0xc);
}

// .text:0x000BD758 size:0x78 mapped:0x806FC7EC
void fn_3_BD758(void) {
    UnknownBss_803C6CF8* bss = &lbl_803C6CF8;
    DrawingSceneStruct* current = lbl_803CC1B8;
    
    if (bss->allowLoad == 1) {
        lbl_3_common_bss_35154.someFlag_418 = 0;
        lbl_3_data_11390[current->unkIndex](lbl_3_common_bss_35154.array_40c[current->unkIndex2]);
        fn_800B0A14_removeQueue();
    }
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
    fn_800BD670(g_hugeAnimStruct.field28_0x70, arg1);
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
void fn_3_BE174(s32 arg, f32 v1, f32 v2, f32 v3) {
    QueueEntry* entry;
    lbl_3_common_bss_35154.bitfield |= 1;
    lbl_3_common_bss_35154.unk_41C = v1;
    lbl_3_common_bss_35154.unk_420 = v2;
    lbl_3_common_bss_35154.unk_424 = v3;
    lbl_3_common_bss_35154.unk_419 = (u8)arg;
    if (arg == 4) {
        entry = fn_800B0A5C_insertQueue(fn_3_BDF74, 3);
        entry->unk10 = 2;
    }
}

// .text:0x000BE1D4 size:0xE24 mapped:0x806FD268
void fn_3_BE1D4(void) {
    return;
}

// .text:0x000BEFF8 size:0x78 mapped:0x806FE08C
void fn_3_BEFF8(void) {
    DrawingSceneStruct* scene = lbl_803CC1B8;
    lbl_3_common_bss_35154.firstMember = (u32)scene;
    fn_80034E20((void(*)(void))scene, (int*)lbl_3_data_1146C, (void**)&lbl_3_common_bss_35154);
    scene->unk18 = 0;
    lbl_803CC1B8->aFunctionPointer = (void*)fn_3_BE1D4;
    lbl_3_common_bss_35154.bitfield = 0;
}

// .text:0x000BF070 size:0xE8 mapped:0x806FE104
void fn_3_BF070(void) {
    return;
}

// .text:0x000BF158 size:0x54 mapped:0x806FE1EC
void fn_3_BF158(void) {
    if (g_d_GameSettings.StadiumID == STADIUM_ID_BOWSERS_CASTLE) {
        fn_3_C39C8();
    } else if (g_d_GameSettings.StadiumID == STADIUM_ID_WARIO_PALACE) {
        fn_3_CE8E4();
    } else if (g_d_GameSettings.StadiumID == STADIUM_ID_PEACH_GARDEN) {
        fn_3_F8ABC();
    }
}

// .text:0x000BF1AC size:0x60 mapped:0x806FE240
void fn_3_BF1AC(void) {
    int i;
    minigamesSetSomePointers();
    fn_3_C0854();
    fn_3_CABB4();
    i = 0xc;
    do {
        g_hugeAnimStruct.fielderStructs[i].funcPtr_0x5c = NULL;
    } while (i--);
    lbl_3_common_bss_35154.unk_479 = 1;
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
s32 fn_3_BF878(void) {
    if (lbl_803C6CF8.allowLoad == 1) {
        lbl_3_common_bss_35154.field_0x8 = ARAMTransfer(&lbl_3_data_11380, 0, 0, 0);
        fn_800B0A5C_insertQueue(fn_3_BF6C0, 0);
        lbl_3_common_bss_35154.unk_3b0 = 1;
        return 1;
    }
    return 0;
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

// .text:0x000C07B0 size:0x60 mapped:0x806FF844
void fn_3_C07B0(void) {
    if (fn_80033928(0x10) != 0 || fn_80033A24(fn_3_C0134, 0x80, 0, 0, 0, 0x10) != 0) {
        lbl_3_bss_995C = 0;
    }
}
