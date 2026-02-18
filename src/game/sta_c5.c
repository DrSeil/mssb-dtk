#include "Dolphin/GX/GXPixel.h"
#include "UnknownHeaders.h"
#include "Dolphin/GX/GXPixel.h"
#include "game/sta_c5.h"
#include "game/UnknownHomes_Game.h"
#include "header_rep_data.h"
#include "game/rep_1D58.h"

// .text:0x000EE100 size:0x288 mapped:0x8072D194
void fn_3_EE100(void) {
    return;
}

// .text:0x000EE388 size:0x2F4 mapped:0x8072D41C
void fn_3_EE388(void) {
    return;
}

// .text:0x000EE67C size:0x2F0 mapped:0x8072D710
void fn_3_EE67C(void) {
    return;
}

// .text:0x000EE96C size:0x228 mapped:0x8072DA00
void fn_3_EE96C(void) {
    return;
}

// .text:0x000EEB94 size:0x160 mapped:0x8072DC28
void fn_3_EEB94(void) {
    return;
}

// .text:0x000EECF4 size:0x148 mapped:0x8072DD88
void fn_3_EECF4(void) {
    return;
}

// .text:0x000EEE3C size:0xE8 mapped:0x8072DED0
void fn_3_EEE3C(void) {
    return;
}

// .text:0x000EEF24 size:0x80 mapped:0x8072DFB8
void fn_3_EEF24(void) {
    return;
}

// .text:0x000EEFA4 size:0x2C mapped:0x8072E038
// .text:0x000EEFA4 size:0x2C mapped:0x8072E038
void fn_3_EEFA4(void) {
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
}

// .text:0x000EEFD0 size:0x4 mapped:0x8072E064
void fn_3_EEFD0(void) {
    return;
}

// .text:0x000EEFD4 size:0x244 mapped:0x8072E068
void fn_3_EEFD4(void) {
    return;
}

// .text:0x000EF218 size:0x4 mapped:0x8072E2AC
void fn_3_EF218(void) {
    return;
}

// .text:0x000EF21C size:0x1B8 mapped:0x8072E2B0
void fn_3_EF21C(void) {
    return;
}

// .text:0x000EF3D4 size:0x34 mapped:0x8072E468



void fn_3_EF3D4(UnknownStruct_EF3D4* arg0, u32 index) {
    /* * clrlslwi r0, r4, 24, 2 
     * This instruction clears the top 24 bits of r4 and shifts left by 2.
     * This is the compiler's way of doing (u8)index * 4.
     */
    u8 scaledIndex = (u8)index;
    
    /* * lwz r3, 0x74(r3)
     * lwzx r4, r4, r0 (where r4 is base of lbl_3_bss_B154)
     * bl fn_3_B97DC
     */
    fn_3_B97DC(arg0->unk74, lbl_3_bss_B154[scaledIndex]);
}

// .text:0x000EF408 size:0x154 mapped:0x8072E49C
void fn_3_EF408(void) {
    return;
}

// .text:0x000EF55C size:0x258 mapped:0x8072E5F0
void fn_3_EF55C(void) {
    return;
}

// .text:0x000EF7B4 size:0x4C mapped:0x8072E848
void fn_3_EF7B4(void) {
    return;
}

// .text:0x000EF800 size:0x90 mapped:0x8072E894
void fn_3_EF800(void) {
    return;
}

// .text:0x000EF890 size:0xA0 mapped:0x8072E924
void fn_3_EF890(void) {
    return;
}

// .text:0x000EF930 size:0x224 mapped:0x8072E9C4
void fn_3_EF930(void) {
    return;
}

// .text:0x000EFB54 size:0x630 mapped:0x8072EBE8
void fn_3_EFB54(void) {
    return;
}

// .text:0x000F0184 size:0xA0 mapped:0x8072F218
void fn_3_F0184(void) {
    return;
}

// .text:0x000F0224 size:0x608 mapped:0x8072F2B8
void fn_3_F0224(void) {
    return;
}

// .text:0x000F082C size:0x778 mapped:0x8072F8C0
void fn_3_F082C(void) {
    return;
}

// .text:0x000F0FA4 size:0x454 mapped:0x80730038
void fn_3_F0FA4(void) {
    return;
}

// .text:0x000F13F8 size:0x50 mapped:0x8073048C
void fn_3_F13F8(void) {
    return;
}

// .text:0x000F1448 size:0xD0 mapped:0x807304DC
void fn_3_F1448(void) {
    return;
}

// .text:0x000F1518 size:0x15C mapped:0x807305AC
void fn_3_F1518(void) {
    return;
}

// .text:0x000F1674 size:0xDC mapped:0x80730708
void fn_3_F1674(void) {
    return;
}

// .text:0x000F1750 size:0x154 mapped:0x807307E4
void fn_3_F1750(void) {
    return;
}

// .text:0x000F18A4 size:0x98 mapped:0x80730938
void fn_3_F18A4(void) {
    return;
}

// .text:0x000F193C size:0x4F0 mapped:0x807309D0
void fn_3_F193C(void) {
    return;
}

// .text:0x000F1E2C size:0x4D0 mapped:0x80730EC0
void fn_3_F1E2C(void) {
    return;
}

// .text:0x000F22FC size:0x14C mapped:0x80731390
void fn_3_F22FC(void) {
    return;
}

// .text:0x000F2448 size:0x2DC mapped:0x807314DC
void fn_3_F2448(void) {
    return;
}

// .text:0x000F2724 size:0x214 mapped:0x807317B8
void fn_3_F2724(void) {
    return;
}

// .text:0x000F2938 size:0x6C4 mapped:0x807319CC
void fn_3_F2938(void) {
    return;
}

// .text:0x000F2FFC size:0x1E4 mapped:0x80732090
void fn_3_F2FFC(void) {
    return;
}

// .text:0x000F31E0 size:0x5DC mapped:0x80732274
void fn_3_F31E0(void) {
    return;
}

// .text:0x000F37BC size:0x118 mapped:0x80732850
void fn_3_F37BC(void) {
    return;
}

// .text:0x000F38D4 size:0x130 mapped:0x80732968
void fn_3_F38D4(void) {
    return;
}

// .text:0x000F3A04 size:0x58 mapped:0x80732A98
void fn_3_F3A04(void) {
    return;
}

// .text:0x000F3A5C size:0x84 mapped:0x80732AF0
void fn_3_F3A5C(void) {
    return;
}

// .text:0x000F3AE0 size:0xD0 mapped:0x80732B74
void fn_3_F3AE0(void) {
    return;
}

// .text:0x000F3BB0 size:0x120 mapped:0x80732C44
void fn_3_F3BB0(void) {
    return;
}

// .text:0x000F3CD0 size:0x22C mapped:0x80732D64
void fn_3_F3CD0(void) {
    return;
}

// .text:0x000F3EFC size:0x3A4 mapped:0x80732F90
void fn_3_F3EFC(void) {
    return;
}

// .text:0x000F42A0 size:0x3CC mapped:0x80733334
void fn_3_F42A0(void) {
    return;
}

// .text:0x000F466C size:0x30 mapped:0x80733700
// .text:0x000F466C size:0x30 mapped:0x80733700
void fn_3_27648(void);

void fn_3_F466C(void) {
    fn_3_27648();
    g_FieldingLogic.unk_13B = 1;
}

// .text:0x000F469C size:0x4 mapped:0x80733730
void fn_3_F469C(void) {
    return;
}

// .text:0x000F46A0 size:0x500 mapped:0x80733734
void fn_3_F46A0(void) {
    return;
}

// .text:0x000F4BA0 size:0xAC mapped:0x80733C34
void fn_3_F4BA0(void) {
    return;
}

// .text:0x000F4C4C size:0xB4 mapped:0x80733CE0
void fn_3_F4C4C(void) {
    return;
}

// .text:0x000F4D00 size:0xAC mapped:0x80733D94
void fn_3_F4D00(void) {
    return;
}

// .text:0x000F4DAC size:0x210 mapped:0x80733E40
void fn_3_F4DAC(void) {
    return;
}

// .text:0x000F4FBC size:0x710 mapped:0x80734050
void fn_3_F4FBC(void) {
    return;
}

// .text:0x000F56CC size:0x564 mapped:0x80734760
void fn_3_F56CC(void) {
    return;
}

// .text:0x000F5C30 size:0x248 mapped:0x80734CC4
void fn_3_F5C30(void) {
    return;
}

// .text:0x000F5E78 size:0x84 mapped:0x80734F0C
void fn_3_F5E78(void) {
    return;
}

// .text:0x000F5EFC size:0x2C mapped:0x80734F90
s32 fn_3_F5EFC(unknown_tri_compare* a, unknown_tri_compare* b) {
    u32 valA = a->value;
    u32 valB = b->value;

    if (valA < valB) {
        return -1;
    }
    return valA > valB;
}

// .text:0x000F5F28 size:0x24 mapped:0x80734FBC
s32 fn_3_F5F28(f32* a, f32* b) {
    f32 fa = *a;
    f32 fb = *b;
    if (fa < fb) {
        return -1;
    }
    return fa > fb;
}

// .text:0x000F5F4C size:0x138 mapped:0x80734FE0
void fn_3_F5F4C(void) {
    return;
}

// .text:0x000F6084 size:0x480 mapped:0x80735118
void fn_3_F6084(void) {
    return;
}

// .text:0x000F6504 size:0xC4 mapped:0x80735598
void fn_3_F6504(void) {
    return;
}

// .text:0x000F65C8 size:0x100 mapped:0x8073565C
void fn_3_F65C8(void) {
    return;
}

// .text:0x000F66C8 size:0x270 mapped:0x8073575C
void fn_3_F66C8(void) {
    return;
}

// .text:0x000F6938 size:0x15C mapped:0x807359CC
void fn_3_F6938(void) {
    return;
}

// .text:0x000F6A94 size:0x1CC mapped:0x80735B28
void fn_3_F6A94(void) {
    return;
}

// .text:0x000F6C60 size:0x36C mapped:0x80735CF4
void fn_3_F6C60(void) {
    return;
}

// .text:0x000F6FCC size:0x10 mapped:0x80736060
void fn_3_F6FCC(void) {
    lbl_3_bss_AEE8 = TRUE;
}

// .text:0x000F6FDC size:0x1468 mapped:0x80736070
void fn_3_F6FDC(void) {
    return;
}
