#ifndef __GAME_m_sound_H_
#define __GAME_m_sound_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"

void fn_3_8B258(void);
void fn_3_8B2E4(void);
void fn_3_8B318(void);
void fn_3_8B718(void);
void fn_3_8B7DC(void);
void fn_3_8B804(void);
void fn_3_8B890(void);
typedef struct CommonBss32B20 {
    u8 _pad0[0x8];
    u32 field_0x8;
    u8 _pad1[0x2138 - 0xC];
} CommonBss32B20;

typedef struct Data8D70 {
    u8 data[0xC];
} Data8D70;

extern CommonBss32B20 lbl_3_common_bss_32B20;
extern Data8D70 lbl_3_data_8D70;

void sndUpdateListener(CommonBss32B20* listener, VecXYZ* pos, VecXYZ* vel, VecXYZ* orient, Data8D70* data, u32 unk7F, u32 unk0);
void fn_3_8B964(VecXYZ* pos, VecXYZ* vel, VecXYZ* orient);
void fn_3_8B9BC(void);
void fn_3_8BA60(void);
void fn_3_8BBC4(void);
void fn_3_8BDF4(void);
void fn_3_8BE8C(void);
void fn_3_8C07C(void);
void fn_3_8C104(void);
void fn_3_8C2DC(void);
void fn_3_8C4F0(void);
void fn_3_8C5C8(void);
void fn_3_8CD74(void);
void fn_3_8D9C0(void);
void fn_3_8DA80(void);
void fn_3_8F1C8(void);
void fn_3_8F21C(void);
void fn_3_8FC0C(void);
void fn_3_8FC80(void);
void fn_3_8FF18(void);
void fn_3_8FF5C(void);
void fn_3_90064(void);
void fn_3_90150(void);
void sndRemoveListener(void* listener);



void sndVolume(u8 volume, u16 time, u8 volGroup);
void fn_3_90220(void);
void playSoundEffect(int arg0);
void fn_3_902FC(void);
void fn_3_90328(void);
void fn_3_903B8(void);
void fn_3_90434(void);
void fn_3_9056C(void);
void fn_3_90674(void);
void fn_3_906FC(void);

extern s32 lbl_3_bss_1780;
#endif // !__GAME_m_sound_H_
