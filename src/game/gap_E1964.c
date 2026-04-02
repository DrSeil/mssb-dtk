#include "game/gap_E1964.h"

#include "UnknownHeaders.h"
#include "game/rep_1838.h"
#include "game/rep_1D58.h"
#include "Dolphin/GX/GXTev.h"

extern void fn_8001D0D0();
extern f32 lbl_3_data_18D98[];
extern void fn_800BF058(void (*fn)(void));
extern void fn_8003A548(void (*fn)(void));
extern void fn_800BDF70(void** ptr);
extern void fn_3_B8184(void);

typedef struct {
    void* _0;
    u8 _4[0x94];
    u8 unk98;   /* 0x98 */
} E1D00Inner;

typedef struct {
    u8 _pad[0x10];
    u8 flag;    /* 0x10 */
    u8 _pad2[0x1c - 0x11];
} E1D00Entry;

extern E1D00Entry lbl_3_data_18ED0[];

typedef struct GapE1964Entry {
    u8 _pad0[0x14];
    f32 field_0x14;
} GapE1964Entry;

// .text:0x000E1964 size:0x84
void fn_3_E1964(void) {
    s32 i;
    GapE1964Entry* entry;
    s32 offset;

    i = 0;
    offset = i;
    do {
        entry = (GapE1964Entry*)((u8*)g_hugeAnimStruct.entries_2D94 + offset);
        fn_8001D0D0(i, lbl_3_data_18D98[0]);
        {
            u32 r = (u32)rand();
            u32 shifted = r << 20;
            r >>= 31;
            shifted -= r;
            shifted = (shifted << 12) | (shifted >> 20);
            shifted += r;
            entry->field_0x14 = shortAngleToRad_Capped((s16)shifted);
        }
        i++;
        offset += 0x28;
    } while (i < 0x1E);
}

// .text:0x000E19E8 size:0x278
void fn_3_E19E8(void) {
    return;
}

// .text:0x000E1C60 size:0xA0
void fn_3_E1C60(void) {
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorIn(0, 0xF, 0xF, 7, 0xA);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
}

// .text:0x000E1D00 size:0xB8
void fn_3_E1D00(void) {
    StadiumObjectElem* obj;
    s32 i;

    fn_800BF058(fn_3_B8184);
    fn_8003A548(fn_3_E1C60);
    i = 0;
    while (i < (s32)lbl_3_common_bss_350E4[0].memcpyCount) {
        if (lbl_3_data_18ED0[i].flag == 2) break;
        obj = &lbl_3_common_bss_350E4[0].src[i];
        fn_3_B828C((StadiumObject*)obj);
        if (obj->unk74 != NULL) {
            E1D00Inner* inner = (E1D00Inner*)*obj->unk74;
            inner->unk98 = obj->unk93 | 6;
            fn_800BDF70(obj->unk74);
        }
        i++;
    }
    fn_8003A548(NULL);
}
