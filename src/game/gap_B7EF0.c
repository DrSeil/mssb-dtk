#include "game/gap_B7EF0.h"
#include "UnknownHeaders.h"

// .text:0x000B7EF0 size:0x28 mapped:0x806F6F84
f32 vecDotProduct(VecXYZ* a, VecXYZ* b) {
    f32 ay = a->y;
    f32 by = b->y;
    f32 ax = a->x;
    f32 dot = ay * by;
    f32 bx = b->x;
    f32 az = a->z;
    f32 bz = b->z;
    return dot + ax * bx + az * bz;
}

// .text:0x000B7F18 size:0x58 mapped:0x806F6FAC
void fn_3_B7F18(VecXYZ* dest, VecXYZ* vec1, VecXYZ* vec2) {
    dest->x = vec1->y * vec2->z - vec1->z * vec2->y;
    dest->y = vec1->z * vec2->x - vec1->x * vec2->z;
    dest->z = vec1->x * vec2->y - vec1->y * vec2->x;
}

// .text:0x000B7F70 size:0x58 mapped:0x806F7004
s16 fn_3_B7F70(s16 input) {
    s16 lcg;
    if (input < 0) {
        input = input * -1;
    }
    lcg = (lbl_3_common_bss_350E4->unk66 * 5 + 1) % 32768;
    lbl_3_common_bss_350E4->unk66 = lcg;
    return lcg % input;
}

