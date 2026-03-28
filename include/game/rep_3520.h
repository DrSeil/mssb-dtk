#ifndef __GAME_rep_3520_H_
#define __GAME_rep_3520_H_

#include "mssbTypes.h"
#include "game/UnknownHomes_Game.h"

typedef struct UnknownObj_134908 {
    u8 _pad0[4];
    s16 value;
} UnknownObj_134908;

typedef struct {
    s16 val;
    s16 _2;
} StarDashTimingEntry;
extern StarDashTimingEntry lbl_3_data_21A3C[2];

void fn_3_133200(void);
void fn_3_133320(void);
void fn_3_13334C(void);
void fn_3_1344BC(void);
void fn_3_1345AC(void);
void fn_3_134658(void);
s32 fn_3_134908(UnknownObj_134908* a, UnknownObj_134908* b);
s32 fn_3_134918(f32* a, f32* b);
void fn_3_13493C(void);
void fn_3_134C80(void);
void fn_3_134D4C(void);
void fn_3_1350BC(void);
s32 fn_3_1354BC(s32 idx, f32 f1, f32 f2);
void fn_3_135520(void);
void fn_3_135600(f32 x, f32 z, f32 *outX, f32 *outZ);
s32 fn_3_13564C(f32 val1, f32 val2);
typedef struct {
    f32 field_0x0;
    u8 _pad[0xD];
    u8 field_0x11;
} Struct_135698_Item;

s32 fn_3_135698(Struct_135698_Item* a, Struct_135698_Item* b);
void fn_3_1356F8(void);
void fn_3_1357A4(void);
void fn_3_13583C(void);
void fn_3_135924(void);
void fn_3_135A64(void);
void fn_3_135C18(void);
void fn_3_135E38(void);
void fn_3_135E98(void);
void fn_3_135F4C(void);
void fn_3_135FF4(void);
void fn_3_136048(void);
void fn_3_1360BC(void);
void fn_3_136220(void);
void fn_3_13688C(void);
void fn_3_136CF4(void);
void fn_3_136EA4(void);
void fn_3_1370A0(void);
void fn_3_1371E8(void);
void fn_3_137224(void);
void fn_3_1373E0(void);
void fn_3_1379A0(void);
void fn_3_137B10(void);
void fn_3_137CF8(void);
void fn_3_137DE4(void);
void fn_3_137F14(void);
void fn_3_13802C(void);
void fn_3_1382E0(void);
void fn_3_138448(void);
void fn_3_1384B4(void);
void fn_3_138AA4(void);
void fn_3_1391C0(void);
void fn_3_139700(void);
void fn_3_13974C(void);
void fn_3_139808(void);
void fn_3_139CA0(void);
void fn_3_139F84(void);
void fn_3_13A048(s32 destIdx, s32 srcIdx);
void fn_3_13A0AC(void);
void fn_3_13A724(void);
void fn_3_13A89C(void);
void fn_3_13AA78(void);
void fn_3_13ACB4(void);
void fn_3_13ADC0(VecXYZ* dst, VecXYZ* a, VecXYZ* b);
void fn_3_13AE1C(void);
void fn_3_13AFE4(void);
void fn_3_13B284(void);
void fn_3_13B9C4(void);
void fn_3_13BB30(void);
void fn_3_13BBF4(void);
void fn_3_13BCB8(void);
void fn_3_13C464(void);
void fn_3_13C468(void);

extern s16 lbl_3_bss_B702;
extern s16 lbl_3_data_21AF0;
// No local headers needed.
#endif // !__GAME_rep_3520_H_
