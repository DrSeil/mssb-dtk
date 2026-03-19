#ifndef __GAME_sta_c6_H_
#define __GAME_sta_c6_H_

#include "mssbTypes.h"

// Structs for fn_3_E6638
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

extern u8 lbl_3_data_1963F;
extern u8 lbl_3_data_19640;
void fn_3_E59B4(void);
void fn_3_E5A1C(void);
void fn_3_E5A84(void);
void fn_3_E5CBC(void);
void fn_3_E5E14(void);
void fn_3_E5E70(void);
void fn_3_E5FEC(void);
void fn_3_E6410(void);
void fn_3_E64A8(void);
void fn_3_E6528(void);
void fn_3_E6578(void);
void fn_3_E6638(BaseStruct_E6638* arg0);
void fn_3_E6684(void);
void fn_3_E671C(void);
void fn_3_E6798(void);
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
void fn_3_E7B20(void);
void fn_3_E8AC8(void);
void fn_3_E8B24(void);

#endif // !__GAME_sta_c6_H_
