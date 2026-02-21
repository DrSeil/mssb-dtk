#include "game/rep_3090.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x000FC448 size:0x4F0 mapped:0x8073B4DC
void fn_3_FC448(void) {
    return;
}

// .text:0x000FC938 size:0x500 mapped:0x8073B9CC
void fn_3_FC938(void) {
    return;
}

// .text:0x000FCE38 size:0x74 mapped:0x8073BECC
void fn_3_FCE38(void) {
    return;
}

// .text:0x000FCEAC size:0x4 mapped:0x8073BF40
void fn_3_FCEAC(void) {
    return;
}

// .text:0x000FCEB0 size:0x70 mapped:0x8073BF44
void fn_3_FCEB0(void) {
    return;
}

// .text:0x000FCF20 size:0x4 mapped:0x8073BFB4
void fn_3_FCF20(void) {
    return;
}

// .text:0x000FCF24 size:0x4E4 mapped:0x8073BFB8
void fn_3_FCF24(void) {
    return;
}

// .text:0x000FD408 size:0xD4 mapped:0x8073C49C
void fn_3_FD408(void) {
    return;
}

// .text:0x000FD4DC size:0x40 mapped:0x8073C570

void fn_3_FD4DC(void) {
    // 1. Load the global pointer into r3 once. 
    // This anchors the base address and prevents reloads.
    Struct_DE94_Inner* inner = lbl_3_common_bss_DE94;

    // 2. srwi. r0, r0, 16 triggers the bne .L_000FD508.
    // In MWCC, (u16)(val >> 16) or (val >> 16) in an if condition 
    // usually triggers the dot-record shift.
    if (!(inner->field_0x118 >> 16)) {
        // 3. stw r0, 0x20(r3) <- lwz r0, 0x124(r3)
        inner->field_0x20 = inner->field_0x124;
    }

    // 4. bl fn_3_FCF24
    fn_3_FCF24();
}

// .text:0x000FD51C size:0x8C mapped:0x8073C5B0
void fn_3_FD51C(void) {
    return;
}

// .text:0x000FD5A8 size:0xC8 mapped:0x8073C63C
void fn_3_FD5A8(void) {
    return;
}

// .text:0x000FD670 size:0x38C mapped:0x8073C704
void fn_3_FD670(void) {
    return;
}

// .text:0x000FD9FC size:0x20 mapped:0x8073CA90
u32 fn_3_FD9FC(void) {
    return lbl_3_common_bss_DE94->field_0x18 == 1;
}

// .text:0x000FDA1C size:0x114 mapped:0x8073CAB0
void fn_3_FDA1C(void) {
    return;
}

// .text:0x000FDB30 size:0x24E8 mapped:0x8073CBC4
void fn_3_FDB30(void) {
    return;
}

// .text:0x00100018 size:0x20 mapped:0x8073F0AC
BOOL fn_3_100018(void) {
    return (s32)lbl_3_common_bss_DE94->field_0x9B6 == 1;
}

// .text:0x00100038 size:0x44 mapped:0x8073F0CC
void fn_3_100038(void) {
    return;
}

// .text:0x0010007C size:0x5C mapped:0x8073F110
void fn_3_10007C(void) {
    return;
}

// .text:0x001000D8 size:0x1BEC mapped:0x8073F16C
void fn_3_1000D8(void) {
    return;
}

// .text:0x00101CC4 size:0x1F6C mapped:0x80740D58
void fn_3_101CC4(void) {
    return;
}

// .text:0x00103C30 size:0x24C mapped:0x80742CC4
void fn_3_103C30(void) {
    return;
}

// .text:0x00103E7C size:0x25C mapped:0x80742F10
void fn_3_103E7C(void) {
    return;
}

// .text:0x001040D8 size:0x260 mapped:0x8074316C
void fn_3_1040D8(void) {
    return;
}

// .text:0x00104338 size:0x270 mapped:0x807433CC
void fn_3_104338(void) {
    return;
}

// .text:0x001045A8 size:0x198 mapped:0x8074363C
void fn_3_1045A8(void) {
    return;
}

// .text:0x00104740 size:0x1A0 mapped:0x807437D4
void fn_3_104740(void) {
    return;
}

// .text:0x001048E0 size:0x15C mapped:0x80743974
void fn_3_1048E0(void) {
    return;
}

// .text:0x00104A3C size:0x4C mapped:0x80743AD0
void fn_3_104A3C(void) {
    return;
}

// .text:0x00104A88 size:0x4C mapped:0x80743B1C
void fn_3_104A88(void) {
    return;
}

// .text:0x00104AD4 size:0x4C mapped:0x80743B68
void fn_3_104AD4(void) {
    return;
}

// .text:0x00104B20 size:0x1C mapped:0x80743BB4
void fn_3_104B20( VecXYZ* dst,  SourceObj_104B20* src) {
    f32 z,y,x;

    x = src->pos.x;
    y = src->pos.y;
    z = src->pos.z;

    dst->x = x;
    dst->y = y;
    dst->z = z;
}

// .text:0x00104B3C size:0x990 mapped:0x80743BD0
void fn_3_104B3C(void) {
    return;
}

// .text:0x001054CC size:0x4 mapped:0x80744560
void fn_3_1054CC(void) {
    return;
}

// .text:0x001054D0 size:0x540 mapped:0x80744564
void fn_3_1054D0(void) {
    return;
}

// .text:0x00105A10 size:0xBC mapped:0x80744AA4
void fn_3_105A10(void) {
    return;
}

// .text:0x00105ACC size:0x10C mapped:0x80744B60
void fn_3_105ACC(void) {
    return;
}

// .text:0x00105BD8 size:0x50 mapped:0x80744C6C
void fn_3_105BD8(void) {
    return;
}

// .text:0x00105C28 size:0x5C mapped:0x80744CBC
void fn_3_105C28(void) {
    return;
}

// .text:0x00105C84 size:0x58 mapped:0x80744D18
void fn_3_105C84(void) {
    return;
}

// .text:0x00105CDC size:0x124 mapped:0x80744D70
void fn_3_105CDC(void) {
    return;
}

// .text:0x00105E00 size:0x214 mapped:0x80744E94
void fn_3_105E00(void) {
    return;
}

// .text:0x00106014 size:0xC4 mapped:0x807450A8
void fn_3_106014(void) {
    return;
}

// .text:0x001060D8 size:0xA4 mapped:0x8074516C
void fn_3_1060D8(void) {
    return;
}

// .text:0x0010617C size:0xF4 mapped:0x80745210
void fn_3_10617C(void) {
    return;
}

// .text:0x00106270 size:0x71C mapped:0x80745304
void fn_3_106270(void) {
    return;
}

// .text:0x0010698C size:0x24 mapped:0x80745A20
u32 fn_3_10698C(u32* list) {
    u32 count = 0;
    while (*list != 0) {
        list++;
        count++;
    }
    return count;
}

// .text:0x001069B0 size:0x10 mapped:0x80745A44
void* fn_3_1069B0(void* base, s32 index) {
    s32* offsetTable = (s32*)base;
    return (u8*)base + offsetTable[index];
}

// .text:0x001069C0 size:0x1E0 mapped:0x80745A54
void fn_3_1069C0(void) {
    return;
}

// .text:0x00106BA0 size:0x25C mapped:0x80745C34
void fn_3_106BA0(void) {
    return;
}