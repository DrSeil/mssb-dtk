#include "game/gap_249E8.h"
#include "game/gap_219CC.h"

#include <string.h>

// .text:0x000249E8 size:0xF4 mapped:0x80663A7C
void fn_3_249E8(s32 arg0) {
    return;
}

// .text:0x00024ADC size:0x2E0 mapped:0x80663B70
void fn_3_24ADC(s32 arg0, s32 arg1) {
    return;
}

// .text:0x00024DBC size:0x80 mapped:0x80663E50
void fn_3_24DBC(void) {
    return;
}

// .text:0x00024EA0 size:0x80 mapped:0x80663F34
void fn_3_24EA0(void) {
    register s32 halfOffset;
    register s32 byteOffset;
    register s32 index;
    s32 flagOffset;
    register s32 zero;
    register s16 minusOne;
    register Bss1323CItem* volatile* basePtr;

    halfOffset = 0;
    byteOffset = halfOffset;
    index = 0;
    zero = halfOffset;
    minusOne = -1;
    basePtr = &lbl_3_common_bss_1323C;
    do {
        *(s16*)((u8*)(*basePtr) + halfOffset + 0x240) = minusOne;
        memset((u8*)(*basePtr) + byteOffset, 0, 0x2C);
        flagOffset = index + 0x261;
        ((u8*)(*basePtr))[flagOffset] = zero;
        index++;
        halfOffset += 2;
        byteOffset += 0x2C;
    } while (index < 0xD);
}

// .text:0x00024F20 size:0x4 mapped:0x80663FB4
void fn_3_24F20(void) {}

// .text:0x00024F24 size:0x1D8 mapped:0x80663FB8
void fn_3_24F24(void) {
    return;
}

// .text:0x000250FC size:0xE8 mapped:0x80664190
void fn_3_250FC(void) {
    return;
}
