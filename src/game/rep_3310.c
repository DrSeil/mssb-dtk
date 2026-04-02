#include "Dolphin/GX/GXFifo.h"
#include "Dolphin/GX/GXGeometry.h"
#include "Dolphin/GX/GXLight.h"
#include "Dolphin/GX/GXPixel.h"
#include "Dolphin/GX/GXTev.h"
#include "Dolphin/GX/GXTransform.h"
#include "game/rep_3310.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"
#include "game/rep_1E08.h"

typedef struct Rep3310Item {
    u8 _pad0[0x14];
    AnimLevel2* field_0x14;
} Rep3310Item;

typedef struct Rep3310List {
    u8 _pad0[0x06];
    u16 count;
    u8 _pad1[0x10];
    Rep3310Item** items;
} Rep3310List;

typedef struct Rep3310Entry2D94Ext {
    u8 _padPreFCC[0xFCC];
    f32 field_0xFCC;
    f32 field_0xFD0;
    f32 field_0xFD4;
    u8 _pad0[0xFEE - 0xFD8];
    u8 field_0xFEE;
    u8 _pad1[0x2508 - 0xFEF];
    void (*field_0x2508)(s32);
    f32 field_0x250C;
    f32 field_0x2510;
    f32 field_0x2514;
    u8 _pad2[0x252E - 0x2518];
    u8 field_0x252E;
} Rep3310Entry2D94Ext;

typedef struct Rep3310Entry2D94Pos {
    u8 _pad0[0x04];
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
} Rep3310Entry2D94Pos;

typedef struct Rep3310MiniPos {
    f32 field_0x00;
    u8 _pad0[0x04];
    f32 field_0x08;
    u8 _pad1[0x31];
    u8 field_0x3D;
} Rep3310MiniPos;

static const f32 lbl_3_rodata_338C[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

extern void fn_8001D0D0();
extern s32 fn_8005268C(void);
extern void fn_80033B58(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern f32 lbl_3_rodata_33C0;
extern f32 lbl_3_rodata_33C4;
extern f32 lbl_3_rodata_33C8;
extern f32 lbl_3_rodata_33E4;
extern f32 lbl_3_rodata_33E8;
extern f32 lbl_3_rodata_33EC;
extern f32 lbl_3_data_21A64;

// .text:0x00116840 size:0x190 mapped:0x807558D4
void fn_3_116840(void) {
    return;
}

// .text:0x001169D0 size:0x168 mapped:0x80755A64
void fn_3_1169D0(void) {
    return;
}

// .text:0x00116B38 size:0x3C mapped:0x80755BCC


void fn_3_116B38(void) {
    s32 multiplier = 2;

    if (g_Minigame.GameMode_MiniGame == 3) {
        multiplier = 31;
    }

    ((u8*)g_hugeAnimStruct.entries_2D94)[multiplier * 40 + 0x26] = 0;
}

// .text:0x00116B74 size:0x8A8 mapped:0x80755C08
void fn_3_116B74(void) {
    return;
}

// .text:0x0011741C size:0x78 mapped:0x807564B0
void fn_3_11741C(s32 index) {
    Rep3310List* list;
    s32 cmpA;
    s32 cmpB;
    s32 i;
    s16 state;
    u8 temp;

    temp = g_Minigame._CCE[0];
    cmpA = temp - 1;
    cmpB = 1 - temp;
    state = 2 & ~((~(cmpA | cmpB)) >> 0x1F);

    list = (Rep3310List*)g_hugeAnimStruct.entries_68[index].unk34;
    for (i = 0; i < list->count; i++) {
        list->items[i]->field_0x14->ptr08->ptr0C->field_0x20 = state;
    }
}

// .text:0x00117494 size:0xF4 mapped:0x80756528
void fn_3_117494(void) {
    Rep3310Entry2D94Ext* entry;
    s16 temp0;
    f32 temp1;
    f32 temp2;
    f32 temp3;
    MiniGameStruct* minigame;

    minigame = &g_Minigame;
    entry = (Rep3310Entry2D94Ext*)g_hugeAnimStruct.entries_2D94;
    entry->field_0x252E = 0;
    entry->field_0xFEE = 0;
    entry->field_0x2508 = NULL;

    if ((u8)minigame->_CCE[0] != 0) {
        temp0 = minigame->_CCC;
        if ((temp0 > 0x3C) || ((temp0 & 1) != 0)) {
            entry->field_0x252E = 1;
            entry->field_0x2508 = fn_3_11741C;
            temp1 = minigame->_CB0;
            entry->field_0x250C = temp1;
            entry->field_0x2510 = -minigame->_CB4;
            temp2 = minigame->_CB8;
            entry->field_0x2514 = temp2;
            fn_8001D0D0(0xED, lbl_3_rodata_33C0);
            entry->field_0xFEE = 1;
            temp3 = lbl_3_rodata_33C4;
            entry->field_0xFCC = temp1;
            entry->field_0xFD0 = temp3;
            entry->field_0xFD4 = temp2;
            fn_8001D0D0(0x65, lbl_3_rodata_33C8, temp1);
        }
    }
}

// .text:0x00117588 size:0x464 mapped:0x8075661C
void fn_3_117588(s32 index) {
    Rep3310Entry2D94Pos* entry;
    Rep3310List* list;
    Rep3310MiniPos* minigamePos;
    Fn52768Result* camera;
    f32 scale;
    f32 texCoords[8];
    f32 vertices[4][3];
    u32 color;
    s32 state;
    s32 i;

    entry = (Rep3310Entry2D94Pos*)&g_hugeAnimStruct.entries_2D94[index];
    minigamePos = (Rep3310MiniPos*)&g_Minigame.unkBB0[index - 0xE9];

    scale = lbl_3_rodata_33C0 *
            (lbl_3_rodata_33E0 - ((-entry->field_0x08 * lbl_3_rodata_33E4) / lbl_3_data_21A64));

    texCoords[0] = lbl_3_rodata_338C[0];
    texCoords[1] = lbl_3_rodata_338C[1];
    texCoords[2] = lbl_3_rodata_338C[2];
    texCoords[3] = lbl_3_rodata_338C[3];
    texCoords[4] = lbl_3_rodata_338C[4];
    texCoords[5] = lbl_3_rodata_338C[5];
    texCoords[6] = lbl_3_rodata_338C[6];
    texCoords[7] = lbl_3_rodata_338C[7];

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetChanCtrl(GX_COLOR0A0, 0, 1, 1, 0, 0, 2);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    color = 0x9B;

    vertices[0][0] = minigamePos->field_0x00 - ((lbl_3_rodata_33E8 * scale) * lbl_3_rodata_33E4);
    vertices[0][1] = lbl_3_rodata_33EC;
    vertices[0][2] = entry->field_0x0C + ((lbl_3_rodata_33C0 * scale) * lbl_3_rodata_33E4);
    vertices[1][0] = minigamePos->field_0x00 + ((lbl_3_rodata_33E8 * scale) * lbl_3_rodata_33E4);
    vertices[1][1] = lbl_3_rodata_33EC;
    vertices[1][2] = entry->field_0x0C + ((lbl_3_rodata_33C0 * scale) * lbl_3_rodata_33E4);
    vertices[2][0] = minigamePos->field_0x00 + ((lbl_3_rodata_33E8 * scale) * lbl_3_rodata_33E4);
    vertices[2][1] = lbl_3_rodata_33EC;
    vertices[2][2] = entry->field_0x0C - ((lbl_3_rodata_33C0 * scale) * lbl_3_rodata_33E4);
    vertices[3][0] = minigamePos->field_0x00 - ((lbl_3_rodata_33E8 * scale) * lbl_3_rodata_33E4);
    vertices[3][1] = lbl_3_rodata_33EC;
    vertices[3][2] = entry->field_0x0C - ((lbl_3_rodata_33C0 * scale) * lbl_3_rodata_33E4);

    camera = fn_80052768(fn_8005268C());
    GXLoadPosMtxImm(camera->mtx, 0);
    GXSetCurrentMtx(0);
    camera = fn_80052768(fn_8005268C());
    GXSetProjection(*(Mtx44*)camera, GX_PERSPECTIVE);
    fn_80033B58(lbl_3_common_bss_32724.field6C, 0x13, 0, 0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GX_WRITE_F32(vertices[0][0]);
    GX_WRITE_F32(vertices[0][1]);
    GX_WRITE_F32(vertices[0][2]);
    GX_WRITE_U32(color);
    GX_WRITE_F32(texCoords[0]);
    GX_WRITE_F32(texCoords[1]);
    GX_WRITE_F32(vertices[1][0]);
    GX_WRITE_F32(vertices[1][1]);
    GX_WRITE_F32(vertices[1][2]);
    GX_WRITE_U32(color);
    GX_WRITE_F32(texCoords[2]);
    GX_WRITE_F32(texCoords[3]);
    GX_WRITE_F32(vertices[2][0]);
    GX_WRITE_F32(vertices[2][1]);
    GX_WRITE_F32(vertices[2][2]);
    GX_WRITE_U32(color);
    GX_WRITE_F32(texCoords[4]);
    GX_WRITE_F32(texCoords[5]);
    GX_WRITE_F32(vertices[3][0]);
    GX_WRITE_F32(vertices[3][1]);
    GX_WRITE_F32(vertices[3][2]);
    GX_WRITE_U32(color);
    GX_WRITE_F32(texCoords[6]);
    GX_WRITE_F32(texCoords[7]);

    list = (Rep3310List*)g_hugeAnimStruct.entries_68[index].unk34;
    state = 1;
    if ((minigamePos->field_0x3D == 1) || (minigamePos->field_0x3D == 5)) {
        state = 0;
    }

    for (i = 0; i < list->count; i++) {
        list->items[i]->field_0x14->ptr08->ptr0C->field_0x20 = state;
    }
}

// .text:0x001179EC size:0xF8 mapped:0x80756A80
void fn_3_1179EC(void) {
    return;
}

// .text:0x00117AE4 size:0x94 mapped:0x80756B78
void fn_3_117AE4(void) {
    return;
}

// .text:0x00117B78 size:0x450 mapped:0x80756C0C
void fn_3_117B78(void) {
    return;
}

// .text:0x00117FC8 size:0xDC mapped:0x8075705C
void fn_3_117FC8(void) {
    return;
}

// .text:0x001180A4 size:0xC0 mapped:0x80757138
void fn_3_1180A4(void) {
    return;
}

// .text:0x00118164 size:0x1F4 mapped:0x807571F8
void fn_3_118164(void) {
    return;
}

// .text:0x00118358 size:0xA4 mapped:0x807573EC
void fn_3_118358(void) {
    return;
}

// .text:0x001183FC size:0x10C mapped:0x80757490
void fn_3_1183FC(void) {
    return;
}

// .text:0x00118508 size:0x10C mapped:0x8075759C
void fn_3_118508(void) {
    return;
}

// .text:0x00118614 size:0x138 mapped:0x807576A8
void fn_3_118614(void) {
    return;
}

// .text:0x0011874C size:0xD0 mapped:0x807577E0
void fn_3_11874C(void) {
    return;
}

// .text:0x0011881C size:0x60 mapped:0x807578B0
void fn_3_11881C(s32 arg0) {
    int val;
    HugeAnimEntry68* entries;
    if (arg0 - 0xe9 < 4) {
        val = lbl_3_data_226BC[arg0 - 0xe9];
    } else {
        val = lbl_3_data_226BC[4];
    }
    entries = g_hugeAnimStruct.entries_68;
    entries[arg0].unk34->unk18->field_0->field_14->ptr08->ptr0C->field_0x20 = val;
}

// .text:0x0011887C size:0x100 mapped:0x80757910
void fn_3_11887C(void) {
    return;
}

// .text:0x0011897C size:0x4C mapped:0x80757A10

void fn_3_11897C(s32 index) {
    g_hugeAnimStruct.entries_68[index].unk34->ptr14->ptr08->ptr0C->field_0x20 =
        (s16)lbl_3_data_22670[((u8*)&g_Minigame)[index * 0x38 - 0x1C44]];
}

// .text:0x001189C8 size:0x150 mapped:0x80757A5C
void fn_3_1189C8(void) {
    return;
}

// .text:0x00118B18 size:0x7A0 mapped:0x80757BAC
void fn_3_118B18(void) {
    return;
}

// .text:0x001192B8 size:0x1B0 mapped:0x8075834C
void fn_3_1192B8(void) {
    return;
}

// .text:0x00119468 size:0x44 mapped:0x807584FC

void fn_3_119468(s32 index) {
    if (g_hugeAnimStruct.entries_68[index].unk34->animFlags & 0x9) {
        fn_3_14225C();
    }
}

// .text:0x001194AC size:0x50 mapped:0x80758540
void fn_3_1194AC(s32 index) {
    HugeAnimEntry68* entries = g_hugeAnimStruct.entries_68;
    entries[index].unk34->unk18->field_0->field_14->ptr08->ptr0C->field_0x20 =
        (s16)lbl_3_data_2265C[((u8*)&g_Minigame)[index + 0x1AFF]];
}

// .text:0x001194FC size:0x358 mapped:0x80758590
void fn_3_1194FC(void) {
    return;
}

// .text:0x00119854 size:0x24 mapped:0x807588E8
// Register Mismatch

f32 fn_3_119854(u32 index) {
    u32 var_r3;

    var_r3 = index;
    if ((u8) var_r3 > 2U) {
        var_r3 = 2;
    }
    return lbl_3_data_22650[(u8)var_r3];
}
// .text:0x00119878 size:0xBC mapped:0x8075890C
void fn_3_119878(void) {
    return;
}

// .text:0x00119934 size:0x300 mapped:0x807589C8
void fn_3_119934(void) {
    return;
}

// .text:0x00119C34 size:0x74 mapped:0x80758CC8
void fn_3_119C34(void) {
    HugeAnimEntry68* entries = g_hugeAnimStruct.entries_68;
    s32 unk74 = lbl_3_common_bss_32724.unk74;
    f32* rodata33AC = &lbl_3_rodata_33AC;
    f32 new_var;
    u32 temp_r4;

    entries[31].unk38 = unk74;
    entries[31].unk42 = 1;
    new_var = *rodata33AC;
    temp_r4 = (u32)(-unk74 | unk74) >> 31;
    entries[32].unk0 = new_var;
    entries[31].unk8C = 1;
    entries[31].unk8D = temp_r4;
    entries[31].unk8E = temp_r4;
    entries[32].unk4 = new_var;
    entries[31].unk88 = lbl_3_rodata_33E0;
    entries[31].unk8E = 1;
    entries[32].unk0 = new_var;
    entries[31].unk8D = 1;
    entries[31].unk8F = 2;
}

// .text:0x00119CA8 size:0x80 mapped:0x80758D3C
void fn_3_119CA8(void) {
    return;
}

// .text:0x00119D28 size:0xC mapped:0x80758DBC
f32 fn_3_119D28(void) {
    return lbl_3_data_2262C;
}

// .text:0x00119D34 size:0xFC mapped:0x80758DC8
void fn_3_119D34(void) {
    return;
}

// .text:0x00119E30 size:0xB0 mapped:0x80758EC4
void fn_3_119E30(void) {
    return;
}

// .text:0x00119EE0 size:0x8C mapped:0x80758F74
void fn_3_119EE0(void) {
    return;
}

// .text:0x00119F6C size:0x2A0 mapped:0x80759000
void fn_3_119F6C(void) {
    return;
}

// .text:0x0011A20C size:0x4 mapped:0x807592A0
void fn_3_11A20C(void) {
    return;
}

// .text:0x0011A210 size:0x140 mapped:0x807592A4
void fn_3_11A210(void) {
    return;
}

// .text:0x0011A350 size:0x3C mapped:0x807593E4




u32 fn_3_11A350(s32 index) {
    // Cast the existing pointer to our 0x90-stride struct
    HugeAnimEntry68* entries = (HugeAnimEntry68*)g_hugeAnimStruct.entries_68;
    
    // This perfectly aligns the registers:
    // r4 is used for &g_hugeAnimStruct (arg 2) and as the base pointer load.
    // r3 calculates the array offset, loads unk34 (arg 1), and is passed directly.
    return (u32)fn_800B4A94(entries[index].unk34, &g_hugeAnimStruct);
}

// .text:0x0011A38C size:0x7C mapped:0x80759420
void fn_3_11A38C(void) {
    return;
}

// .text:0x0011A408 size:0x524 mapped:0x8075949C
void fn_3_11A408(void) {
    return;
}

// .text:0x0011A92C size:0x200 mapped:0x807599C0
void fn_3_11A92C(void) {
    return;
}

// .text:0x0011AB2C size:0x140 mapped:0x80759BC0
void fn_3_11AB2C(void) {
    return;
}

// .text:0x0011AC6C size:0xAF0 mapped:0x80759D00
void fn_3_11AC6C(void) {
    return;
}

// .text:0x0011B75C size:0x448 mapped:0x8075A7F0
void fn_3_11B75C(void) {
    return;
}

// .text:0x0011BBA4 size:0x488 mapped:0x8075AC38
void fn_3_11BBA4(void) {
    return;
}

// .text:0x0011C02C size:0x2A0 mapped:0x8075B0C0
void fn_3_11C02C(void) {
    return;
}

// .text:0x0011C2CC size:0x32C mapped:0x8075B360
void fn_3_11C2CC(void) {
    return;
}

// .text:0x0011C5F8 size:0x708 mapped:0x8075B68C
void fn_3_11C5F8(void) {
    return;
}

// .text:0x0011CD00 size:0x204 mapped:0x8075BD94
void fn_3_11CD00(void) {
    return;
}

// .text:0x0011CF04 size:0x80 mapped:0x8075BF98
void fn_3_11CF04(void) {
    return;
}

// .text:0x0011CF84 size:0x22C mapped:0x8075C018
void fn_3_11CF84(void) {
    return;
}

// .text:0x0011D1B0 size:0x118 mapped:0x8075C244
void fn_3_11D1B0(void) {
    return;
}

// .text:0x0011D2C8 size:0xE4 mapped:0x8075C35C
void fn_3_11D2C8(void) {
    return;
}
