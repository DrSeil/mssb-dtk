#ifndef __GAME_sta_c6_H_
#define __GAME_sta_c6_H_

#include "mssbTypes.h"
#include "game/sta_c2.h"

// Structs for fn_3_E6638
typedef struct OuterPtr_E6798 {
    struct NodeArray_E6798* unk0;  // +0x00
} OuterPtr_E6798;

typedef struct ElemNode_E6798 {
    u8 _pad[0x14];
    u32 unk14;  // +0x14
    u32 unk18;  // +0x18
} ElemNode_E6798;

typedef struct NodeArray_E6798 {
    u8 _pad[0x18];
    ElemNode_E6798** unk18;  // +0x18
} NodeArray_E6798;

typedef struct Arg0_E6798 {
    u8 _pad[0x74];
    OuterPtr_E6798* unk74;  // +0x74
} Arg0_E6798;

typedef struct InnerStruct_E6638 {
    u8 _pad[0x14];
    u32 field_0x14;  // 0x14
    u32 field_0x18;  // 0x18
} InnerStruct_E6638;

typedef struct MiddleStruct_E6638 {
    u8 _pad[0x18];
    InnerStruct_E6638** array;  // 0x18 - array of pointers
} MiddleStruct_E6638;

typedef struct OuterStruct_E6638 {
    MiddleStruct_E6638* field_0x00;  // 0x00
} OuterStruct_E6638;

typedef struct BaseStruct_E6638 {
    u8 _pad[0x74];
    OuterStruct_E6638* field_0x74;  // 0x74
} BaseStruct_E6638;

typedef struct {
    u8 _pad0[0xA4];
    u32 unkA4;      // 0xA4
    u8 _pad1[0xC];  // 0xA8..0xB3
    u8* unkB4;      // 0xB4
    u8 _pad2[0x4];  // 0xB8..0xBB
    u8* unkBC;      // 0xBC
    u8 _pad3[0x2];  // 0xC0..0xC1
    u8 unkC2;       // 0xC2
} Struct_E6528;

typedef struct {
    u8 _pad0[0x6];
    u8 unk6;   // +0x06
} Inner_E5E14;

typedef struct {
    u8 _pad0[0x4];
    Inner_E5E14* unk4;  // +0x04
} Arg0_E5E14;

extern u8 lbl_3_data_196B4;
extern u8 lbl_3_data_19024;
extern u8 lbl_3_data_1963F;
extern u8 lbl_3_data_19640;
extern f32 lbl_3_rodata_2B90;
extern f32 lbl_3_rodata_2B94;
extern f32 lbl_3_rodata_2B98;
extern f32 lbl_3_rodata_2B9C;
extern f32 lbl_3_rodata_2BA0;
f32 fn_800B4C40(void *boneData);
void fn_800B4CA0(void *boneData, f32 val);
void fn_3_E59B4(ActorObject *arg0);
void fn_3_E5A1C(ActorObject *arg0);
void fn_3_E5A84(void);
void fn_3_E5CBC(void);
int fn_3_E5E14(Arg0_E5E14* arg0);
void fn_3_E5E70(void);
void fn_3_E5FEC(void);
void fn_3_E6410(void);
void fn_3_E64A8(void);
void fn_3_E6528(Struct_E6528* arg0);
void fn_3_E6578(void);
void fn_3_E6638(BaseStruct_E6638* arg0);
void fn_3_E6684(void);
void fn_3_E671C(void);
void fn_3_E6798(Arg0_E6798* arg0);
void fn_3_E67F4(void);
void fn_3_E68A8(void);
void fn_3_E698C(void);
void fn_3_E6A48(void);
void fn_3_E6D90(void);
void fn_3_E7350(void);
void fn_3_E7364(s32 index);
void fn_3_E7388(void);
void fn_3_E7424(void);
void fn_3_E751C(void);
void fn_3_E763C(void);
void fn_3_E7A2C(void);
u8 fn_3_E7B20(u32 arg0, u32 arg1);
int fn_3_E8AC8(void);
void fn_3_E8B24(void);
void fn_3_E911C(void);

#endif // !__GAME_sta_c6_H_
