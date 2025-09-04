#ifndef __GAME_rep_1838_H_
#define __GAME_rep_1838_H_

#include "mssbTypes.h"
#include "game/UnknownHomes.h"

#define SANG_MAX_ANGLE (0x1000)

#define SANG_ANG_90 (SANG_MAX_ANGLE / 4)
#define SANG_ANG_180 (SANG_MAX_ANGLE / 2)
#define SANG_ANG_270 (SANG_ANG_90 * 3)
#define SANG_ANG_360 (SANG_MAX_ANGLE)

f32 fn_3_9EAE4(f32 a, f32 b);
f32 fn_3_9EBCC(f32 a, f32 b);
int fn_3_9ED1C(int min, int max);
int fn_3_9EE24(int max);
int RandomInt_Game(int max);
// void static_clamp(void);
f32 fn_3_9EFD0(VecXYZ* a, VecXYZ* b, VecXYZ* c, VecXYZ* d);
bool calculateLineIntersection(VecXZ* out, VecXZ* a, VecXZ* b);
void normalizeVector(VecXYZ* out, VecXYZ* v);
void fn_3_9F79C(f32 a, f32 b, f32 c, f32* x, f32* y);
void getComponentsFromRad(f32 v, f32* x, f32* y);
void fn_3_9F9C8(s16 ang, f32* x, f32* y);
f32 fn_3_9FAA4(f32 x, f32 y);
f32 game_atan2(f32 x, f32 y);
s16 fn_3_9FB8C(f32 x, f32 y);
s16 fn_3_9FC1C(f32 a, f32 b);
s16 fn_3_9FCA4(s16 a, s16 b);
s16 fn_3_9FCF8(s16 a, s16 b);
s16 fn_3_9FD28(s16 ang);
s16 fn_3_9FD6C(s16 ang);
f32 fn_3_9FDD8(f32);
s16 fn_3_9FE6C_normalizeAngle(s16);
f32 fn_3_9FEA8(f32 v);
s16 fn_3_9FF04(f32 v);
s16 radToShortAngle(f32 v);
f32 shortAngleToRad(s16 ang);
f32 shortAngleToRad_Capped(s16);

#endif // !__GAME_rep_1838_H_
