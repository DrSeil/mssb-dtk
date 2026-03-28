#include "game/sta_c6.h"
#include "UnknownHeaders.h"
#include "static/UnknownHomes_Static.h"
#include "stl/mem.h"
#include "header_rep_data.h"

// .text:0x000E59B4 size:0x68 mapped:0x80724A48
void fn_3_E59B4(ActorObject *arg0) {
    void *boneData = arg0->wrapper->boneData;
    if (lbl_3_rodata_2B90 + fn_800B4C40(boneData) > lbl_3_rodata_2B94) {
        fn_800B4CA0(boneData, lbl_3_rodata_2B98);
    }
    AnimateActorBones(boneData);
}

// .text:0x000E5A1C size:0x68 mapped:0x80724AB0
void fn_3_E5A1C(ActorObject *arg0) {
    void *boneData = arg0->wrapper->boneData;
    if (lbl_3_rodata_2B90 + fn_800B4C40(boneData) > lbl_3_rodata_2B9C) {
        fn_800B4CA0(boneData, lbl_3_rodata_2BA0);
    }
    AnimateActorBones(boneData);
}

// .text:0x000E5A84 size:0x238 mapped:0x80724B18
void fn_3_E5A84(void) {
    return;
}

// .text:0x000E5CBC size:0x158 mapped:0x80724D50
void fn_3_E5CBC(void) {
    return;
}

// .text:0x000E5E14 size:0x5C mapped:0x80724EA8
int fn_3_E5E14(Arg0_E5E14* arg0) {
    u8 nibble = (u8)(arg0->unk4->unk6 >> 4);
    switch (nibble) {
    case 0:
    case 3:
        return 2;
    case 5:
        return 4;
    case 1:
    case 2:
    case 4:
        return 3;
    default:
        return 0;
    }
}

// .text:0x000E5E70 size:0x17C mapped:0x80724F04
void fn_3_E5E70(void) {
    return;
}

// .text:0x000E5FEC size:0x424 mapped:0x80725080
void fn_3_E5FEC(void) {
    return;
}

// .text:0x000E6410 size:0x98 mapped:0x807254A4
void fn_3_E6410(void) {
    return;
}

// .text:0x000E64A8 size:0x80 mapped:0x8072553C
void fn_3_E64A8(void) {
    return;
}

// .text:0x000E6528 size:0x50 mapped:0x807255BC
void fn_3_E6528(Struct_E6528* arg0) {
    memcpy(&arg0->unkA4, arg0->unkB4, 4);
    arg0->unkBC = &lbl_3_data_19024 + 0x64;
    arg0->unkC2 = 0;
}

// .text:0x000E6578 size:0xC0 mapped:0x8072560C
void fn_3_E6578(void) {
    return;
}

// .text:0x000E6638 size:0x4C mapped:0x807256CC
void fn_3_E6638(BaseStruct_E6638* arg0) {
    arg0->field_0x74->field_0x00->array[0]->field_0x14 =
        arg0->field_0x74->field_0x00->array[0]->field_0x18;

    arg0->field_0x74->field_0x00->array[1]->field_0x14 =
        arg0->field_0x74->field_0x00->array[1]->field_0x18;

    arg0->field_0x74->field_0x00->array[2]->field_0x14 =
        arg0->field_0x74->field_0x00->array[2]->field_0x18;
}

// .text:0x000E6684 size:0x98 mapped:0x80725718
void fn_3_E6684(void) {
    return;
}

// .text:0x000E671C size:0x7C mapped:0x807257B0
void fn_3_E671C(void) {
    return;
}

// .text:0x000E6798 size:0x5C mapped:0x8072582C
void fn_3_E6798(Arg0_E6798* arg0) {
    u8* flags = &lbl_3_data_196B4;
    int i = 0;
    NodeArray_E6798* nodeArr = arg0->unk74->unk0;
    for (; i < 7; i++) {
        if (*flags != 0) {
            nodeArr->unk18[i]->unk14 = nodeArr->unk18[i]->unk18;
        } else {
            nodeArr->unk18[i]->unk14 = 0;
        }
        flags++;
    }
}

// .text:0x000E67F4 size:0xB4 mapped:0x80725888
void fn_3_E67F4(void) {
    return;
}

// .text:0x000E68A8 size:0xE4 mapped:0x8072593C
void fn_3_E68A8(void) {
    return;
}

// .text:0x000E698C size:0xBC mapped:0x80725A20
void fn_3_E698C(void) {
    return;
}

// .text:0x000E6A48 size:0x348 mapped:0x80725ADC
void fn_3_E6A48(void) {
    return;
}

// .text:0x000E6D90 size:0x5C0 mapped:0x80725E24
void fn_3_E6D90(void) {
    return;
}

// .text:0x000E7350 size:0x14 mapped:0x807263E4
void fn_3_E7350(void) {
    lbl_3_data_1963F = lbl_3_data_19640;
}

// .text:0x000E7364 size:0x24 mapped:0x807263F8
void fn_3_E7364(s32 index) {
    lbl_3_data_19640 = ((u8*)lbl_3_common_bss_350E4)[index * 0xE8 + 0xC0];
}

// .text:0x000E7388 size:0x9C mapped:0x8072641C
void fn_3_E7388(void) {
    return;
}

// .text:0x000E7424 size:0xF8 mapped:0x807264B8
void fn_3_E7424(void) {
    return;
}

// .text:0x000E751C size:0x120 mapped:0x807265B0
void fn_3_E751C(void) {
    return;
}

// .text:0x000E763C size:0x3F0 mapped:0x807266D0
void fn_3_E763C(void) {
    return;
}

// .text:0x000E7A2C size:0xF4 mapped:0x80726AC0
void fn_3_E7A2C(void) {
    return;
}

// .text:0x000E7B20 size:0xFA8 mapped:0x80726BB4
u8 fn_3_E7B20(u32 arg0, u32 arg1) {
    return 0;
}

// .text:0x000E8AC8 size:0x5C mapped:0x80727B5C
int fn_3_E8AC8(void) {
    if (g_d_GameSettings.StadiumID != 6) {
        return 0;
    }
    return (u8)fn_3_E7B20(lbl_3_common_bss_350E4[0].unk38, lbl_3_common_bss_350E4[0].unk34) != 0;
}

// .text:0x000E8B24 size:0x5F8 mapped:0x80727BB8
void fn_3_E8B24(void) {
    return;
}
