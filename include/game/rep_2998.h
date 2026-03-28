#ifndef __GAME_rep_2998_H_
#define __GAME_rep_2998_H_

#include "mssbTypes.h"
#include "UnknownHeaders.h"

void fn_3_E1FA8(void);
void fn_3_E2034(void);
void fn_3_E2118(void);
void fn_3_E22A4(void);
void fn_3_E2324(void);
void fn_3_E25D0(void);
void fn_3_E266C(void);
void fn_3_E28DC(void);
void fn_3_E29B4(void);
void fn_3_E2B70(void);
void fn_3_E2E78(void);
void fn_3_E2F4C(void);
void fn_3_E3044(void);
void fn_3_E3284(void);
void fn_3_E3668(void);
void fn_3_E3764(void);
void fn_3_E3914(void);
void fn_3_E3B88(void);
void fn_3_E4554(castleThwompObj *arg);
extern s32 lbl_3_bss_AE18;
void fn_3_E45A8(castleThwompObj *param);
typedef struct {
    f32 rotValues[7]; /* 0x00: 7 floats per entry, stride 0x1C */
} RotTableEntry;

typedef struct {
    u8 _pad[0xC];
    RotTableEntry entries[1]; /* 0x0C: rotation table entries */
} CastleRotData;

typedef struct {
    Control control;     /* 0x00 */
    u8 _pad0[0x9C - 0x3C];
    u8 rotIdx;           /* 0x9C */
    u8 _pad1[0xB0 - 0x9D];
    f32 curRotation;     /* 0xB0 */
} CastleRotObj;

extern CastleRotData lbl_3_data_18ED0;
extern f32 lbl_3_rodata_2A5C;

void fn_3_E45F0(CastleRotObj *arg0);
void fn_3_E4658(void);
void fn_3_E4760(void);
void fn_3_E48D0(void);
void fn_3_E4A38(void);
void fn_3_E4BE8(void);
void fn_3_E4CB0(void);
void fn_3_E4EF4(void);
void fn_3_E4FC4(void);

#endif // !__GAME_rep_2998_H_
