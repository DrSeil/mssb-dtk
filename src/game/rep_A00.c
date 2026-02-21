#include "game/rep_A00.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x00021C90 size:0x154 mapped:0x80660D24
void fn_3_21C90(void) {
    return;
}

// .text:0x00021DE4 size:0x130 mapped:0x80660E78
void fn_3_21DE4(void) {
    return;
}

// .text:0x00021F14 size:0x828 mapped:0x80660FA8
void fn_3_21F14(void) {
    return;
}

// .text:0x0002273C size:0xE0 mapped:0x806617D0
void fn_3_2273C(void) {
    return;
}

// .text:0x0002281C size:0x34 mapped:0x806618B0
/**
 * @address 0002281C
 */
BOOL fn_3_2281C(s32 index) {
    // lis r4, g_hugeAnimStruct@ha
    // addi r3, r4, g_hugeAnimStruct@l
    // slwi r0, r3, 2 -> add r3, r3, r0
    Struct_3_2281C_Entry* ptr = (Struct_3_2281C_Entry*)g_hugeAnimStruct.AnimationStructPtrs[index];

    // lwz r3, 0x2C50(r3)
    // cmplwi r3, 0
    if (ptr == NULL) {
        return TRUE;
    }

    // lha r0, 0x68(r3)
    // cntlzw r0, r0
    // srwi r3, r0, 5
    return ptr->field_0x68 == 0;
}

// .text:0x00022850 size:0xF4 mapped:0x806618E4
void fn_3_22850(void) {
    return;
}

// .text:0x00022944 size:0x4 mapped:0x806619D8
void fn_3_22944(void) {
    return;
}

// .text:0x00022948 size:0xD8 mapped:0x806619DC
void fn_3_22948(void) {
    return;
}

// .text:0x00022A20 size:0x9C mapped:0x80661AB4
void fn_3_22A20(void) {
    return;
}

// .text:0x00022ABC size:0x154 mapped:0x80661B50
void fn_3_22ABC(void) {
    return;
}

// .text:0x00022C10 size:0x10 mapped:0x80661CA4
void spinWait(void) {
    s32 i;
    for (i = 13; i > 0; i--);
}

// .text:0x00022C20 size:0x4B4 mapped:0x80661CB4
void fn_3_22C20(void) {
    return;
}

// .text:0x000230D4 size:0x3E8 mapped:0x80662168
void fn_3_230D4(void) {
    return;
}

// .text:0x000234BC size:0x3D4 mapped:0x80662550
void fn_3_234BC(void) {
    return;
}

// .text:0x00023890 size:0x45C mapped:0x80662924
void fn_3_23890(void) {
    return;
}

// .text:0x00023CEC size:0x40C mapped:0x80662D80
void fn_3_23CEC(void) {
    return;
}

// .text:0x000240F8 size:0x4A0 mapped:0x8066318C
void fn_3_240F8(void) {
    return;
}

// .text:0x00024598 size:0x98 mapped:0x8066362C
void fn_3_24598(void) {
    return;
}

// .text:0x00024630 size:0xD8 mapped:0x806636C4
void fn_3_24630(void) {
    return;
}

// .text:0x00024708 size:0x2E0 mapped:0x8066379C
void fn_3_24708(void) {
    return;
}
