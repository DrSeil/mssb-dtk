#include "game/rep_720.h"
#include "UnknownHeaders.h"
#include "game/rep_3090.h"
#include "header_rep_data.h"

// .text:0x00014BB4 size:0x268 mapped:0x80653C48
void fn_3_14BB4(void) {
    return;
}

// .text:0x00014E1C size:0x34 mapped:0x80653EB0
void fn_3_14E1C(void) {
    // 1. lis r3, lbl_3_bss_44@ha
    // 2. addi r4, r3, lbl_3_bss_44@l
    // Anchoring the pointer to bssPtr ensures r4 is prepared.
    s32 *bssPtr = &lbl_3_bss_44;
    
    // 3. li r0, 0
    // 4. stw r0, 0(r4)
    *bssPtr = 0;

    // 5. li r3, 0 (passing NULL)
    // Overwrites r3 with 0, matching the assembly.
    fn_800528AC(NULL);

}

// .text:0x00014E50 size:0x34 mapped:0x80653EE4
// One register is STWU instead of STW
void fn_3_14E50(void) {
    void (*callback)(void) = fn_3_14BB4; // Anchors to r3
    s32 *bssPtr = &lbl_3_bss_44;        // Anchors to r4

    // stw r0, lbl_3_bss_44@l(r4)
    *bssPtr = 5;

    // Call using r3. r4 remains loaded with bssPtr from the store.
    fn_800528AC(callback); 
}

// .text:0x00014E84 size:0x3C mapped:0x80653F18

void fn_3_14E84(void) {
    /* lbz r0, 0x133(r3) -> cmplwi r0, 2 -> bnelr */
    if (g_FieldingLogic.unk133 != 2) {
        return;
    }

    /* lwz r4, g_pCamera(r3) -> lha r0, 0xaa4(r4) -> cmpwi r0, 0 -> bnelr */
    if (g_pCamera->unkAA4 != 0) {
        return;
    }

    /* lwz r0, 0x24(r3) -> sth r0, 0xaa4(r4) */
    /* Note: sth uses the lower 16 bits of the word loaded from g_Stats */
    g_pCamera->unkAA4 = g_Stats.unk24;
}

// .text:0x00014EC0 size:0x28 mapped:0x80653F54
void fn_3_14EC0(void) {
    if (g_pCamera->unk_A50 == -1) {
        g_pCamera->unk_A50 = g_Strikes.unk_C;
    }
}

// .text:0x00014EE8 size:0x3C mapped:0x80653F7C

/**
 * @address 00014EE8
 */
void fn_3_14EE8(void) {
    /* lbz r0, 0x91(r3) -> cmplwi r0, 0x1 -> bnelr */
    if (g_Batter.contactMadeInd != 1) {
        return;
    }

    /* lwz r4, g_pCamera@l(r3) -> lha r0, 0xaa6(r4) -> cmpwi r0, -1 -> bnelr */
    if (g_pCamera->unkAA6 != -1) {
        return;
    }

    /* lis r3, g_Ball@ha -> addi r3, r3, g_Ball@l -> lha r0, 0x1b9c(r3) */
    /* sth r0, 0xaa6(r4) */
    g_pCamera->unkAA6 = g_Ball.Hit_HorizontalAngle;
}

// .text:0x00014F24 size:0x44 mapped:0x80653FB8
void fn_3_14F24(void) {
    return;
}

// .text:0x00014F68 size:0x60 mapped:0x80653FFC
void fn_3_14F68(void) {
    return;
}

// .text:0x00014FC8 size:0x3C mapped:0x8065405C

/**
 * @address 00014FC8
 */
void fn_3_14FC8(void) {
    /* lis r3, lbl@ha -> addi r3, r3, lbl@l -> lha r0, 0x4(r3) */
    /* cmpwi r0, 0xe -> bnelr */
    if (lbl_3_common_bss_32A94.unk4 != 0xE) {
        return;
    }

    /* lis r3, g_pCamera@ha -> lwz r4, g_pCamera@l(r3) -> lha r0, 0xa96(r4) */
    /* cmpwi r0, 0 -> bnelr */
    if (g_pCamera->unkA96 != 0) {
        return;
    }

    /* lis r3, g_Stats@ha -> addi r3, r3, g_Stats@l -> lwz r0, 0x24(r3) */
    /* sth r0, 0xa96(r4) */
    g_pCamera->unkA96 = (s16)g_Stats.unk24;
}

// .text:0x00015004 size:0xBC mapped:0x80654098
void fn_3_15004(void) {
    return;
}

// .text:0x000150C0 size:0x84 mapped:0x80654154
void fn_3_150C0(void) {
    return;
}

// .text:0x00015144 size:0xDC mapped:0x806541D8
void fn_3_15144(void) {
    return;
}

// .text:0x00015220 size:0x5C mapped:0x806542B4
void fn_3_15220(void) {
    return;
}

// .text:0x0001527C size:0x6C mapped:0x80654310
void fn_3_1527C(void) {
    return;
}

// .text:0x000152E8 size:0x94 mapped:0x8065437C
void fn_3_152E8(void) {
    return;
}

// .text:0x0001537C size:0x74 mapped:0x80654410
void fn_3_1537C(void) {
    return;
}

// .text:0x000153F0 size:0x68 mapped:0x80654484
void fn_3_153F0(void) {
    return;
}

// .text:0x00015458 size:0x6C mapped:0x806544EC
void fn_3_15458(void) {
    return;
}

// .text:0x000154C4 size:0x60 mapped:0x80654558
void fn_3_154C4(void) {
    return;
}

// .text:0x00015524 size:0x574 mapped:0x806545B8
void fn_3_15524(void) {
    return;
}

// .text:0x00015A98 size:0x290 mapped:0x80654B2C
void fn_3_15A98(void) {
    return;
}

// .text:0x00015D28 size:0x3C mapped:0x80654DBC

void fn_3_15D28(void) {
    /* * Target Sequence:
     * lis r3, g_pCamera@ha -> addi r3, r3, g_pCamera@l
     * lwz r3, 0(r3)        -> Load pointer
     * lbz r0, 0xad8(r3)    -> Load cameraFlag2
     * cmplwi r0, 1         -> Check if flag is 1
     * bne .L_00015D50      -> Skip next call if not 1
     */
    if (g_pCamera->cameraFlag2 == 1) {
        /* bl fn_3_FBE24 */
        fn_3_FBE24();
    }

    /* .L_00015D50:
     * bl fn_3_FD670
     */
    fn_3_FD670();
}

// .text:0x00015D64 size:0x18 mapped:0x80654DF8
void fn_3_15D64(void) {
    g_pCamera->cameraFlag2 = TRUE;
}

// .text:0x00015D7C size:0x14 mapped:0x80654E10
void fn_3_15D7C(u8 value) {
    g_pCamera->cameraFlag = value;
}

// .text:0x00015D90 size:0x188 mapped:0x80654E24
void fn_3_15D90(void) {
    return;
}

// .text:0x00015F18 size:0x20 mapped:0x80654FAC
void fn_3_15F18(void) {
    fn_3_FD670();
}

// .text:0x00015F38 size:0x3C mapped:0x80654FCC



void fn_3_15F38(void) {
    if ((u8) g_pCamera->cameraFlag2 == 1) {
        fn_3_FBE24();
    }
    fn_3_FD670();
}

// .text:0x00015F74 size:0x20 mapped:0x80655008
void fn_3_15F74(void) {
    fn_3_FD670();
}

// .text:0x00015F94 size:0x3C mapped:0x80655028

/**
 * @address 00015F94
 */
void fn_3_15F94(void) {
    u32 result;

    /* li r3, 0 -> bl fn_3_9EE24 */
    result = fn_3_9EE24(0);

    /* To match the assembly:
     * 1. result stays in r3.
     * 2. g_d_GameSettings address is loaded into r4.
     * 3. StadiumID is loaded into r0.
     * 4. result (r3) = result (r3) + StadiumID (r0).
     * 5. result (r3) = result (r3) + 0x1D.
     */
    
    result += g_d_GameSettings.StadiumID;
    fn_3_FBDAC(result + 0x1D);
}

// .text:0x00015FD0 size:0x20 mapped:0x80655064
void fn_3_15FD0(void) {
    fn_3_FD670();
}

// .text:0x00015FF0 size:0x24 mapped:0x80655084
void fn_3_15FF0(void) {
    fn_3_FBDAC(0x69);
}

// .text:0x00016014 size:0x20 mapped:0x806550A8
void fn_3_16014(void) {
    fn_3_FD670();
}

// .text:0x00016034 size:0x24 mapped:0x806550C8
void fn_3_16034(void) {
    fn_3_FBDAC(0x68);
}

// .text:0x00016058 size:0x168 mapped:0x806550EC
void fn_3_16058(void) {
    return;
}

// .text:0x000161C0 size:0x168 mapped:0x80655254
void fn_3_161C0(void) {
    return;
}

// .text:0x00016328 size:0x180 mapped:0x806553BC
void fn_3_16328(void) {
    return;
}

// .text:0x000164A8 size:0x64 mapped:0x8065553C
void fn_3_164A8(void) {
    return;
}

// .text:0x0001650C size:0x1E8 mapped:0x806555A0
void fn_3_1650C(void) {
    return;
}

// .text:0x000166F4 size:0x20C mapped:0x80655788
void fn_3_166F4(void) {
    return;
}

// .text:0x00016900 size:0x2A0 mapped:0x80655994
void fn_3_16900(void) {
    return;
}

// .text:0x00016BA0 size:0x1CC mapped:0x80655C34
void fn_3_16BA0(void) {
    return;
}

// .text:0x00016D6C size:0x198 mapped:0x80655E00
void fn_3_16D6C(void) {
    return;
}

// .text:0x00016F04 size:0x20C mapped:0x80655F98
void fn_3_16F04(void) {
    return;
}

// .text:0x00017110 size:0x20C mapped:0x806561A4
void fn_3_17110(void) {
    return;
}

// .text:0x0001731C size:0x444 mapped:0x806563B0
void fn_3_1731C(void) {
    return;
}

// .text:0x00017760 size:0xA4 mapped:0x806567F4
void fn_3_17760(void) {
    return;
}

// .text:0x00017804 size:0x58 mapped:0x80656898
void fn_3_17804(void) {
    return;
}

// .text:0x0001785C size:0x294 mapped:0x806568F0
void fn_3_1785C(void) {
    return;
}

// .text:0x00017AF0 size:0x3C mapped:0x80656B84



void fn_3_17AF0(void) {
    if ((u8) g_GameLogic.sceneID == 4) {
        fn_3_197E8();
        return;
    }
    fn_3_1C1B0();
}

// .text:0x00017B2C size:0x274 mapped:0x80656BC0
void fn_3_17B2C(void) {
    return;
}

// .text:0x00017DA0 size:0x76C mapped:0x80656E34
void fn_3_17DA0(void) {
    return;
}

// .text:0x0001850C size:0xAEC mapped:0x806575A0
void fn_3_1850C(void) {
    return;
}

// .text:0x00018FF8 size:0x3F0 mapped:0x8065808C
void fn_3_18FF8(void) {
    return;
}

// .text:0x000193E8 size:0x1D4 mapped:0x8065847C
void fn_3_193E8(void) {
    return;
}

// .text:0x000195BC size:0x1B4 mapped:0x80658650
void fn_3_195BC(void) {
    return;
}

// .text:0x00019770 size:0x78 mapped:0x80658804
void fn_3_19770(void) {
    return;
}

// .text:0x000197E8 size:0x354 mapped:0x8065887C
void fn_3_197E8(void) {
    return;
}

// .text:0x00019B3C size:0x174 mapped:0x80658BD0
void fn_3_19B3C(void) {
    return;
}

// .text:0x00019CB0 size:0x2F4 mapped:0x80658D44
void fn_3_19CB0(void) {
    return;
}

// .text:0x00019FA4 size:0xEA0 mapped:0x80659038
void fn_3_19FA4(void) {
    return;
}

// .text:0x0001AE44 size:0x74 mapped:0x80659ED8
void fn_3_1AE44(void) {
    return;
}

// .text:0x0001AEB8 size:0x2C4 mapped:0x80659F4C
void fn_3_1AEB8(void) {
    return;
}

// .text:0x0001B17C size:0x1F4 mapped:0x8065A210
void fn_3_1B17C(void) {
    return;
}

// .text:0x0001B370 size:0x6A8 mapped:0x8065A404
void fn_3_1B370(void) {
    return;
}

// .text:0x0001BA18 size:0x698 mapped:0x8065AAAC
void fn_3_1BA18(void) {
    return;
}

// .text:0x0001C0B0 size:0x80 mapped:0x8065B144
void fn_3_1C0B0(void) {
    return;
}

// .text:0x0001C130 size:0x80 mapped:0x8065B1C4
void fn_3_1C130(void) {
    return;
}

// .text:0x0001C1B0 size:0x4D4 mapped:0x8065B244
void fn_3_1C1B0(void) {
    return;
}

// .text:0x0001C684 size:0x214 mapped:0x8065B718
void fn_3_1C684(void) {
    return;
}

// .text:0x0001C898 size:0x10 mapped:0x8065B92C
u8 getSceneID(void) {
    return g_GameLogic.sceneID;
}

// .text:0x0001C8A8 size:0x4 mapped:0x8065B93C
void fn_3_1C8A8(void) {
    return;
}

// .text:0x0001C8AC size:0x214 mapped:0x8065B940
void fn_3_1C8AC(void) {
    return;
}

// .text:0x0001CAC0 size:0x10C mapped:0x8065BB54
void fn_3_1CAC0(void) {
    return;
}

// .text:0x0001CBCC size:0xFC mapped:0x8065BC60
void fn_3_1CBCC(void) {
    return;
}

// .text:0x0001CCC8 size:0x1C8 mapped:0x8065BD5C
void fn_3_1CCC8(void) {
    return;
}

// .text:0x0001CE90 size:0x9DC mapped:0x8065BF24
void fn_3_1CE90(void) {
    return;
}
