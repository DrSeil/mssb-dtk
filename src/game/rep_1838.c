#include "game/rep_1838.h"
#include "header_rep_data.h"
#include "game/UnknownHomes.h"
#include "Dolphin/rand.h"

#define MAX_ANGLE (0x1000)

#define ANG_90 (MAX_ANGLE / 4)
#define ANG_180 (MAX_ANGLE / 2)
#define ANG_270 (ANG_90 * 3)
#define ANG_360 (MAX_ANGLE)

// .text:0x000A0018 size:0x84 mapped:0x806DF0AC
f32 shortAngleToRad_Capped(s16 ang) {
    f32 v = shortAngleToRad(ang);
    if (v > PI) {
        v = -(TAU - v);
    }
    return v;
}

// .text:0x0009FFAC size:0x6C mapped:0x806DF040
f32 shortAngleToRad(s16 ang) {
    f32 v;
    if (ang < 0) {
        ang += ANG_360;
    }
    if (ang >= ANG_360) {
        ang -= ANG_360;
    }
    return PI * (f32)(ang * 2) / (f32)ANG_360;
}

// .text:0x0009FF60 size:0x4C mapped:0x806DEFF4
s16 radToShortAngle(f32 v) {
    if (v < 0) {
        v = v + TAU;
    }
    return v * (f32)ANG_180 / PI; // implicit conversion to s16
}

// .text:0x0009FF04 size:0x5C mapped:0x806DEF98
s16 fn_3_9FF04(f32 v) {
    s16 ret = radToShortAngle(v);
    if (ret >= ANG_180) {
        ret -= ANG_360;
    }
    return ret; // implicit conversion to s16
}

// .text:0x0009FEA8 size:0x5C mapped:0x806DEF3C
f32 fn_3_9FEA8(f32 v) {
    if (v >= PI) {
        while (v >= PI) {
            v = v - TAU;
        }
    }
    if (v < -PI) {
        while (v < -PI) {
            v = v + TAU;
        }
    }
    return v;
}

// .text:0x0009FE6C size:0x3C mapped:0x806DEF00
s16 fn_3_9FE6C(s16 ang) {
    if (ang < 0) {
        while (ang < 0) {
            ang += ANG_360;
        }
    }
    if (ang >= ANG_360) {
        while (ang >= ANG_360) {
            ang -= ANG_360;
        }
    }
    return ang;
}

// .text:0x0009FDD8 size:0x94 mapped:0x806DEE6C
f32 fn_3_9FDD8(f32 ang) {
    f32 v;
    if (ang < 0) {
        while (ang < 0) {
            ang += (f32)ANG_360;
        }
    }
    if (ang >= (f32)ANG_360) {
        while (ang >= (f32)ANG_360) {
            ang -= (f32)ANG_360;
        }
    }
    v = PI * (f32)(ang * 2) / (f32)ANG_360;
    if (v > PI) {
        v = -(TAU - v);
    }
    return v;
}


// .text:0x0009FD6C size:0x6C mapped:0x806DEE00
s16 fn_3_9FD6C(s16 ang) {
    if (ang < 0) {
        while (ang < 0) {
            ang += ANG_360;
        }
    }
 
    if (ang >= ANG_360) {
        while (ang >= ANG_360) {
            ang -= ANG_360;
        }
    }
 
    if (ang > ANG_180) {
        ang -= ANG_360;
        if (ang <= -ANG_90) {
            ang = -ANG_90 + 1;
        }
    } else if (ang >= ANG_90) {
        ang = ANG_90 - 1;
    }

    return ang;
}

// .text:0x0009FD28 size:0x44 mapped:0x806DEDBC
s16 fn_3_9FD28(s16 ang) {
    if (ang < -ANG_180) {
        while (ang < -ANG_180) {
            ang += ANG_360;
        }
    }
    if (ang > ANG_180 - 1) {
        while (ang > ANG_180 - 1) {
            ang -= ANG_360;
        }
    }
    return ang;
}

// .text:0x0009FCF8 size:0x30 mapped:0x806DED8C
s16 fn_3_9FCF8(s16 a, s16 b) {
    int diff = a - b;
    diff = (s16)ABS(diff);
    if (diff > ANG_180) {
        return ANG_360 - diff;
    }
    return diff;
}

// .text:0x0009FCA4 size:0x54 mapped:0x806DED38
s16 fn_3_9FCA4(s16 a, s16 b) {
    s16 diff;
    if (a > ANG_180) {
        a = a - ANG_360;
    }
    if (b > ANG_180) {
        b = b - ANG_360;
    }
    diff = a - b;
    if (diff > ANG_180) {
        diff -= ANG_360;
    }
    if (diff < -ANG_180) {
        diff += ANG_360;
    }
    return diff;
}

// .text:0x0009FC1C size:0x88 mapped:0x806DECB0
s16 fn_3_9FC1C(f32 x, f32 y) {
    f32 diff = x - y;
    int ang;
    if (diff < 0) {
        diff = -diff;
    }

    if (diff < 0) {
        diff = diff + TAU;
    }

    ang = (diff * (f32)ANG_180 / PI);

    if ((s16)ang >= ANG_180) {
        ang = ang - ANG_360;
    }
    ang = (s16)ang;

    return (s16)ABS(ang);
}

// .text:0x0009FB8C size:0x90 mapped:0x806DEC20
s16 fn_3_9FB8C(f32 x, f32 y) {
    s16 v;
    if (x == 0) {
        if (y >= 0.f) {
            return ANG_90;
        } else {
            return ANG_270;
        }
    }
    v = (((f32)atan2(y, x)) * (f32)ANG_180 / PI);
    if (v < 0) {
        v = v + ANG_360;
    }
    return v;
}

// .text:0x0009FB40 size:0x4C mapped:0x806DEBD4
f32 game_atan2(f32 x, f32 y) {
   if (x == 0.f && y == 0.f) {
        return 0;
    }
    return atan2(y, x);
}

// .text:0x0009FAA4 size:0x9C mapped:0x806DEB38
f32 fn_3_9FAA4(f32 x, f32 y) {
    f32 v;
    if (x == 0) {
        if (y >= 0.f) {
            return (f32)ANG_90;
        } else {
            return (f32)ANG_270;
        }
    }

    v = ((f32)atan2(y, x)) * (f32)ANG_180 / PI;
    if (v < 0.f)
    {
        v = v + (f32)ANG_360;
    }
    return v;
}

// .text:0x0009F9C8 size:0xDC mapped:0x806DEA5C
void fn_3_9F9C8(s16 ang, f32* x, f32* y) {
    f32 v;
    if (ang < 0) {
        ang = ang + ANG_360;
    }
    if (ang >= ANG_360) {
        ang -= ANG_360;
    }
    v = PI * (ang * 2) / (f32)ANG_360;
    if (v > PI) {
        v = -(TAU - v);
    }
    getComponentsFromRad(v, x, y);
}

// .text:0x0009F964 size:0x64 mapped:0x806DE9F8
void getComponentsFromRad(f32 v, f32* x, f32* y) {
    *x = COSF(v);
    *y = SINF(v);
}

// .text:0x0009F79C size:0x1C8 mapped:0x806DE830
void fn_3_9F79C(f32 a, f32 b, f32 c, f32* x, f32* y) {
    f32 v = game_atan2(b, c);
    a += v;
    {
        f32 c_ = COSF(a);
        f32 s = SINF(b);
        *x = dolsqrtf2(a * a + b * b);
    }
}

// .text:0x0009F658 size:0x144 mapped:0x806DE6EC
void fn_3_9F658(void) {
    return;
}

// .text:0x0009F5B8 size:0xA0 mapped:0x806DE64C
void fn_3_9F5B8(void) {
    return;
}

// .text:0x0009EFD0 size:0x5E8 mapped:0x806DE064
void fn_3_9EFD0(void) {
    return;
}

// UNUSED .text:0x0009EFAC size:0x24 mapped:0x806DE040
static u8 static_clamp(int v, int min, int max) {
    if (v > max)
    {
        return max;
    }
    else if (v < min)
    {
        return min;
    }
    return v;
}

// .text:0x0009EEB8 size:0xF4 mapped:0x806DDF4C
int RandomInt_Game(int max) {
    int ret;
    int absmax = ABS(max);
    
    if (absmax <= 1) {
        return 0;
    }

    inMemBall.StaticRandomInt1 = inMemBall.StaticRandomInt1 - ((u8)inMemBall.StaticRandomInt2) +
                                 inMemBall.StaticRandomInt2 / absmax + inMemBall.totalFramesAtPlay;

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE && practiceStruct.instructionNumber >= 0) {
        return 0;
    }

    if (gameInitVariables.GameModeSelected == GAME_TYPE_MINIGAMES) {
        inMemBall.StaticRandomInt1 += rand();
    }

    ret = inMemBall.StaticRandomInt1 % absmax;
    ret = ABS(ret);
    if (max < 0) {
        ret = -ret;
    }
    
    return ret;
}

// .text:0x0009EE24 size:0x94 mapped:0x806DDEB8
int fn_3_9EE24(int max) {
    int ret;
    int absmax = ABS(max);
    
    if (absmax <= 1) {
        return 0;
    }
    unkSimulationRelatedStruct._00 = ((u8)inMemBall.StaticRandomInt2) + unkSimulationRelatedStruct._00 +
                                     gameInitVariables.FrameCountWhileNotAtMainMenu +
                                     (gameInitVariables.FrameCountWhileNotAtMainMenu >> 1) -
                                     inMemBall.StaticRandomInt1 + unkSimulationRelatedStruct._00 / absmax;
    ret = unkSimulationRelatedStruct._00 % absmax;
    ret = ABS(ret);
    if (max < 0) {
        ret = -ret;
    }
    return ret;
}

// .text:0x0009ED1C size:0x108 mapped:0x806DDDB0
int fn_3_9ED1C(int min, int max) {
    int diff = max - min + 1;
    return RandomInt_Game(diff);
}

// .text:0x0009EBCC size:0x150 mapped:0x806DDC60
f32 fn_3_9EBCC(f32 a, f32 b) {
    return RandomInt_Game((int)((b - a) * 1000.f) + 1) * (1.f / 1000.f) + a;
}

// .text:0x0009EAE4 size:0xE8 mapped:0x806DDB78
f32 fn_3_9EAE4(f32 a, f32 b) {
    return RandomInt_Game((int)((b - a) * 1000.f) + 1) * (1.f / 1000.f) + a;
}
