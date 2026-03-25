#ifndef UNKNOWN_HEADERS_H
#define UNKNOWN_HEADERS_H

#include "types.h"
#include "game/UnknownHomes_Game.h"
#include "game/rep_D0.h"
#include "C3/control.h"
#include "Dolphin/mtx.h"

typedef void (*QueueCallback)(void);
void fn_800528AC(void (*callback)(void));
void fn_80011604(s8 arg1, void (*func)(void));

typedef struct {
    u8 _pad0[0x8];
    void* unk8;         /* 0x08 */
    u8 _pad1a[0x10 - 0xC];
    s32 unk10;          /* 0x10 */
    u8 _pad1b[0x98 - 0x14];
    void* unk98;        /* 0x98 */
    u8 _pad9C[0xBC - 0x9C];
    s32 unkBC;
} Lbl800EF808; // size: 0x39C
extern Lbl800EF808 lbl_800EF808;
BOOL fn_80021518(s32 charID, s32 param_2);
void fn_800214D0(void);

typedef struct {
    u8 _data0[0xE];
    u16 unkE;           /* 0xE */
    u8 _data1[0x1E - 0x10];
} StatTableEntry; // size: 0x1E

typedef struct { u8 _pad[0x26]; } BatterStatEntry; // size: 0x26

extern StatTableEntry lbl_803535C8[2][9]; // PitcherStats entries[teamFielding][rosterSlot]
extern BatterStatEntry lbl_803537E4[2][9]; // BatterStats entries[teamBatting][rosterSlot]

// Forward declaration for fn_800339F0 return type
struct Struct_800339F0_Result;

struct Struct_800339F0_Result* fn_800339F0(s32 arg1, s32 arg2);
typedef struct Struct_3_2281C_Entry {
    u8 _pad0[0x68];
    s16 field_0x68;
} Struct_3_2281C_Entry;

/* Forward declarations for types used as pointers in hugeAnimStruct */
typedef struct StadiumFileHeader {
    u8 _pad[0x0C];
    u32 field_0x0C;   // self-relative offset to sub-object
    u32 field_0x10;   // self-relative offset to sub-object
} StadiumFileHeader;
struct ACTBone;
struct ActLayout;
struct AnimBank;
typedef struct starAnimationRelated {
    u8 _pad0[0xC6];
    u8 state_alt;    /* 0xC6 */
    u8 _pad1[0x2CE - 0xC7];
    u8 unk2CE;       /* 0x2CE */
    u8 _pad2[0x27];
    u8 unk2F6;       /* 0x2F6 */
    u8 _pad3[0x27];
    u8 unk31E;       /* 0x31E */
    u8 _pad4[0x27];
    u8 unk346;       /* 0x346 */
} starAnimationRelated;

/* AnimationStruct - 0x27C bytes, used in arrays within hugeAnimStruct.
 * Full field definitions available in in_game.h. */
typedef struct AnimationStruct {
    u8 _pad[0x5C];
    void* funcPtr_0x5c; /* 0x5C - function pointer */
    u8 _rest[0x21C];
} AnimationStruct; /* size: 0x27C */

/* EOSPad - 2-u8 struct used for alignment padding */
typedef struct EOSPad {
    u8 pad[2];
} EOSPad;

/* unkAnimSubstruct - 0xD2 u8s.
 * Full field definitions available in in_game.h.
 * Using opaque u8 array to prevent compiler alignment padding. */
typedef struct unkAnimSubstruct {
    /* 0x00 */ void* field0_0x0[4];
    /* 0x10 */ u8    field1_0x10[28];
    /* 0x2C */ void* field2_0x2c[12];
    /* 0x5C */ u8    field3_0x5c[4];
    /* 0x60 */ u8    field4_0x60[4];
    /* 0x64 */ void* field5_0x64[4];
    /* 0x74 */ u8    field6_0x74[28];
    /* 0x90 */ void* field7_0x90;     // First pointer at 0x90
    /* 0x94 */ void* field7_0x94;     // Second pointer at 0x94
    /* 0x98 */ u8    animFlags;       // Offset 0x98 - Target for lbz
    /* 0x99 */ u8    field7_0x99[39]; // Remainder of the 0x90-0xC0 block
    /* 0xC0 */ u8    field8_0xc0[4];
    /* 0xC4 */ u8    field9_0xc4[4];
    /* 0xC8 */ void* field10_0xc8;
    /* 0xCC */ u8    field11_0xcc;
    /* 0xCD */ u8    field12_0xcd;
    /* 0xCE */ u8    field13_0xce;
    /* 0xCF */ u8    field14_0xcf;
    /* 0xD0 */ u8    field15_0xd0;
    /* 0xD1 */ u8    field16_0xd1;
} unkAnimSubstruct; /* size: 0xD2 */

struct AnimObj_field0;

typedef struct AnimObject {
    struct AnimObj_field0* field_0; // 0x0
    u32 flags; // 0x4
    u8 _pad[0x6C]; // 0x8-0x73
    u32 field_0x74; // 0x74
} AnimObject;

typedef struct AnimLevel4 {
    u8 _pad[0x4];
    AnimObject* ptr04; // 0x4
} AnimLevel4;

typedef struct AnimLevel3 {
    u8 _pad[0x10];
    AnimLevel4* ptr10; // offset 0x10
} AnimLevel3;

typedef struct AnimL2_0C {
    u8 _pad[0x20];
    s16 field_0x20; // offset 0x20
} AnimL2_0C;

typedef struct AnimL2_08 {
    u8 _pad[0xC];
    AnimL2_0C* ptr0C; // offset 0xC
} AnimL2_08;

typedef struct AnimLevel2 {
    u8 _pad0[0x8];
    AnimL2_08* ptr08; // offset 0x8
    u8 _pad1[0x4];
    AnimLevel4* ptr10; // offset 0x10
} AnimLevel2;

typedef struct AnimObj_field0 {
    u8 _pad[0x14];
    AnimLevel2* field_14; // offset 0x14
} AnimObj_field0;

typedef struct AnimLevel1 {
    u8 _pad0[0x14]; 
    AnimLevel2* ptr14; // offset 0x14
    AnimObject* unk18;    // offset 0x18
    u8 _pad1[0x90 - 0x1C];
    /* 0x90 */ void* field7_0x90;
    /* 0x94 */ void* field7_0x94;
    /* 0x98 */ u8    animFlags;
    /* 0x99 */ u8    _pad2[0xD2 - 0x99];
} AnimLevel1; 

typedef struct HugeAnimEntry2D94 {
    // Not confident of these
    /* 0x00 */ void* stadiumFileHeaderPointer;
    /* 0x04 */ void* stadiumObjectLoadingPtr;
    /* 0x08 */ s32 unk08[7]; // 0x08 through 0x23
    /* 0x24 */ u8 _pad24[2]; // 0x24-0x25
    /* 0x26 */ u8 unk_26;
    /* 0x27 */ u8 _pad27;
} HugeAnimEntry2D94; // size: 0x28
typedef struct HugeAnimEntry68 {
    AnimLevel1* field_0x0;  // offset 0x0
    AnimLevel1* field_0x4;  // offset 0x4
    u8 _pad1[0x2C];         // 0x08-0x33
    AnimLevel1* unk34;      // offset 0x34, matches lwz r3, 0x34(r3)
    u8 _pad2[0x58]; // (0x90 - 0x38) u8s to make the struct size exactly 0x90
} HugeAnimEntry68;

typedef struct ActRelatedSub {
    /* +0x00 */ f32 prevFloat;    // (sub+1)->prevFloat = r6+0x5c (next sub's first field)
    /* +0x04 */ u8 _pad04[0x50];  // 0x04-0x53
    /* +0x54 */ f32 float54;      // r6+0x54
    /* +0x58 */ u8 _pad58;
    /* +0x59 */ u8 flag59;        // r6+0x59
    /* +0x5a */ u8 flag5a;        // r6+0x5a
    /* +0x5b */ u8 flag5b;        // r6+0x5b
    // sizeof = 0x5c
} ActRelatedSub;

typedef struct ActRelatedEntry {
    /* 0x00 */ u8 _pad0[0x34];    // 0x00-0x33
    /* 0x34 */ ActRelatedSub sub; // r6 = &entries[idx].sub, sizeof = 0x5c
    // sizeof = 0x34 + 0x5c = 0x90
} ActRelatedEntry;

typedef struct HugeAnimSubObj34 {
    u8 data[0x10];
} HugeAnimSubObj34;

typedef struct HugeAnimCtrlObj {
    u8 _pad[0x34];
    HugeAnimSubObj34 sub34;  /* 0x34 */
    Control ctrl;            /* 0x44 */
} HugeAnimCtrlObj;

typedef struct hugeAnimStruct {
    /* 0x0000 */ u8 field0_0x0[4];
    /* 0x0004 */ struct StadiumFileHeader* stadiumFileHeaderPointer;
    /* 0x0008 */ void* stadiumObjectLoadingPtr;
    /* 0x000C */ s32 field3_0xc;
    /* 0x0010 */ s32 field4_0x10;
    /* 0x0014 */ s32 field5_0x14;
    /* 0x0018 */ HugeAnimEntry68* entries_18;
    /* 0x001C */ s32 field7_0x1c;
    /* 0x0020 */ s32 field8_0x20;
    /* 0x0024 */ s32 field9_0x24;
    /* 0x0028 */ s32 field10_0x28;
    /* 0x002C */ s32 field11_0x2c;
    /* 0x0030 */ s32 field12_0x30;
    /* 0x0034 */ s32 field13_0x34;
    /* 0x0038 */ s32 field14_0x38;
    /* 0x003C */ s32 field15_0x3c;
    /* 0x0040 */ s32 field16_0x40;
    /* 0x0044 */ s32 field17_0x44;
    /* 0x0048 */ s32 field18_0x48;
    /* 0x004C */ s32 field19_0x4c;
    /* 0x0050 */ s32 field20_0x50;
    /* 0x0054 */ s32 field21_0x54;
    /* 0x0058 */ s32 field22_0x58;
    /* 0x005C */ s32 field23_0x5c;
    /* 0x0060 */ void* ctrlOrBonePtr;
    /* 0x0064 */ ActRelatedEntry* actRelatedPtr;
    /* 0x0068 */ HugeAnimEntry68* entries_68;
    /* 0x006C */ void* unkPtr;
    /* 0x0070 */ struct HugeAnimCtrlObj* field28_0x70;
    /* 0x0074 */ u8 pad_0x74[56];
    /* 0x00AC */ void* field30_0xac;
    /* 0x00B0 */ void* field31_0xb0;
    /* 0x00B4 */ void* field32_0xb4;
    /* 0x00B8 */ void* field33_0xb8;
    /* 0x00BC */ u8 pad2_0xbc[128];
    /* 0x013C */ void* field35_0x13c;
    /* 0x0140 */ struct ActLayout** field36_0x140;
    /* 0x0144 */ void* field37_0x144;
    /* 0x0148 */ void* field38_0x148;
    /* 0x014C */ void* field39_0x14c;
    /* 0x0150 */ f32 field40_0x150;
    /* 0x0154 */ f32 field41_0x154;
    /* 0x0158 */ u8 field42_0x158[9];
    /* 0x0161 */ u8 field43_0x161[15];
    /* 0x0170 */ void* field44_0x170[11];
    /* 0x019C */ u8 field45_0x19c;
    /* 0x019D */ u8 field46_0x19d[3];
    /* 0x01A0 */ u8 field47_0x1a0[4];
    /* 0x01A4 */ struct ActLayout** field48_0x1a4;
    /* 0x01A8 */ void* field49_0x1a8;
    /* 0x01AC */ void* field50_0x1ac;
    /* 0x01B0 */ void* field51_0x1b0;
    /* 0x01B4 */ f32 field52_0x1b4;
    /* 0x01B8 */ f32 field53_0x1b8;
    /* 0x01BC */ u8 field54_0x1bc[9];
    /* 0x01C5 */ u8 field55_0x1c5[15];
    /* 0x01D4 */ void* field56_0x1d4[11];
    /* 0x0200 */ u8 field57_0x200;
    /* 0x0201 */ u8 field58_0x201[3];
    /* 0x0204 */ u8 field59_0x204[4];
    /* 0x0208 */ void* field60_0x208;
    /* 0x020C */ u8 field61_0x20c;
    /* 0x020D */ u8 field62_0x20d;
    /* 0x020E */ u8 field63_0x20e;
    /* 0x020F */ u8 field64_0x20f;
    /* 0x0210 */ u8 field65_0x210;
    /* 0x0211 */ u8 field66_0x211;
    /* 0x0214 */ unkAnimSubstruct substructs[9];
    /* 0x0988 */ void* field86_0x988;
    /* 0x098C */ u8 field87_0x98c[96];
    /* 0x09EC */ void* field88_0x9ec;
    /* 0x09F0 */ u8 field89_0x9f0[108];
    /* 0x0A5C */ void* field90_0xa5c;
    /* 0x0A60 */ u8 field91_0xa60[96];
    /* 0x0AC0 */ void* field92_0xac0;
    /* 0x0AC4 */ u8 field93_0xac4[108];
    /* 0x0B30 */ void* field94_0xb30;
    /* 0x0B34 */ u8 field95_0xb34[96];
    /* 0x0B94 */ void* field96_0xb94;
    /* 0x0B98 */ u8 field97_0xb98[108];
    /* 0x0C04 */ AnimationStruct fielderStructs[13]; /* [0-9]: fielders/pitcher, [10-11]: runners, [12]: extra */
    /* 0x2C50 */ AnimationStruct* AnimationStructPtrs[13];
    /* 0x2C84 */ struct ActLayout** actLayoutPtr;
    /* 0x2C88 */ struct AnimBank* animBankPtr;
    /* 0x2C8C */ void* modelDataPtr;
    /* 0x2C90 */ s32 field112_0x2c90;
    /* 0x2C94 */ s32 field113_0x2c94;
    /* 0x2C98 */ s32 field114_0x2c98;
    /* 0x2C9C */ s32 field115_0x2c9c;
    /* 0x2CA0 */ s32 field116_0x2ca0;
    /* 0x2CA4 */ s32 field117_0x2ca4;
    /* 0x2CA8 */ s32 field118_0x2ca8;
    /* 0x2CAC */ struct AnimBank* animBankPtr2;
    /* 0x2CB0 */ struct AnimBank* animBankPtr3;
    /* 0x2CB4 */ void* field121_0x2cb4;
    /* 0x2CB8 */ void* field122_0x2cb8;
    /* 0x2CBC */ void* field123_0x2cbc;
    /* 0x2CC0 */ struct AnimBank* animBankPtr4;
    /* 0x2CC4 */ struct AnimBank* animBankPtr5;
    /* 0x2CC8 */ struct AnimBank* animBankPtr6;
    /* 0x2CCC */ struct AnimBank* animBankPtr7;
    /* 0x2CD0 */ struct AnimBank* animBankPtr8;
    /* 0x2CD4 */ struct AnimBank* animBankPtr9;
    /* 0x2CD8 */ struct AnimBank* animBankPtr10;
    /* 0x2CDC */ struct AnimBank* animBankPtr11;
    /* 0x2CE0 */ struct AnimBank* animBankPtr12;
    /* 0x2CE4 */ struct AnimBank* animBankPtr13;
    /* 0x2CE8 */ struct AnimBank* animBankPtr14;
    /* 0x2CEC */ s32 field135_0x2cec;
    /* 0x2CF0 */ s32 field136_0x2cf0;
    /* 0x2CF4 */ s32 field137_0x2cf4;
    /* 0x2CF8 */ s32 field138_0x2cf8;
    /* 0x2CFC */ s32 field139_0x2cfc;
    /* 0x2D00 */ s32 field140_0x2d00;
    /* 0x2D04 */ s32 field141_0x2d04;
    /* 0x2D08 */ s32 field142_0x2d08;
    /* 0x2D0C */ s32 field143_0x2d0c;
    /* 0x2D10 */ s32 field144_0x2d10;
    /* 0x2D14 */ s32 field145_0x2d14;
    /* 0x2D18 */ s32 field146_0x2d18;
    /* 0x2D1C */ s32 field147_0x2d1c;
    /* 0x2D20 */ s32 field148_0x2d20;
    /* 0x2D24 */ s32 field149_0x2d24;
    /* 0x2D28 */ s32 field150_0x2d28;
    /* 0x2D2C */ s32 field151_0x2d2c;
    /* 0x2D30 */ s32 field152_0x2d30;
    /* 0x2D34 */ s32 field153_0x2d34;
    /* 0x2D38 */ s32 field154_0x2d38;
    /* 0x2D3C */ void* field155_0x2d3c;
    /* 0x2D40 */ s32 field156_0x2d40;
    /* 0x2D44 */ u16 field157_0x2d44;
    /* 0x2D46 */ u8 maybe_loadingBatterModel_State;
    /* 0x2D47 */ u8 field159_0x2d47;
    /* 0x2D48 */ void* field160_0x2d48;
    /* 0x2D4C */ u32 field161_0x2d4c;
    /* 0x2D50 */ s16 field162_0x2d50;
    /* 0x2D52 */ u8 maybe_relatedToLoadingBatter;
    /* 0x2D53 */ u8 field164_0x2d53;
    /* 0x2D54 */ void* field165_0x2d54;
    /* 0x2D58 */ u32 field166_0x2d58;
    /* 0x2D5C */ s16 field167_0x2d5c;
    /* 0x2D5E */ u8 maybe_relatedToLoadingBatter2;
    /* 0x2D5F */ u8 field169_0x2d5f;
    /* 0x2D60 */ s32 field170_0x2d60;
    /* 0x2D64 */ s32 field171_0x2d64;
    /* 0x2D68 */ s16 characterRelated;
    /* 0x2D6A */ u8 field173_0x2d6a;
    /* 0x2D6B */ u8 field174_0x2d6b;
    /* 0x2D6C */ u8 field175_0x2d6c;
    /* 0x2D6D */ u8 field176_0x2d6d;
    /* 0x2D6E */ u8 field177_0x2d6e;
    /* 0x2D6F */ u8 field178_0x2d6f;
    /* 0x2D70 */ u8 field179_0x2d70;
    /* 0x2D71 */ u8 field180_0x2d71;
    /* 0x2D72 */ u8 field181_0x2d72;
    /* 0x2D73 */ u8 field182_0x2d73;
    /* 0x2D74 */ u8 field183_0x2d74;
    /* 0x2D75 */ u8 field184_0x2d75;
    /* 0x2D76 */ u8 field185_0x2d76;
    /* 0x2D77 */ u8 field186_0x2d77;
    /* 0x2D78 */ u8 field187_0x2d78;
    /* 0x2D79 */ u8 field188_0x2d79;
    /* 0x2D7A */ u8 field189_0x2d7a;
    /* 0x2D7B */ u8 field190_0x2d7b;
    /* 0x2D7C */ u8 field191_0x2d7c;
    /* 0x2D7D */ u8 field192_0x2d7d;
    /* 0x2D7E */ u8 field193_0x2d7e;
    /* 0x2D7F */ u8 field194_0x2d7f;
    /* 0x2D80 */ u8 field195_0x2d80;
    /* 0x2D81 */ u8 field196_0x2d81;
    /* 0x2D82 */ u8 field197_0x2d82;
    /* 0x2D83 */ u8 field198_0x2d83;
    /* 0x2D84 */ u8 field199_0x2d84;
    /* 0x2D85 */ u8 field200_0x2d85;
    /* 0x2D86 */ u8 field201_0x2d86;
    /* 0x2D87 */ u8 field202_0x2d87;
    /* 0x2D88 */ u8 field203_0x2d88;
    /* 0x2D89 */ u8 field204_0x2d89;
    /* 0x2D8A */ u8 field205_0x2d8a;
    /* 0x2D8B */ u8 field206_0x2d8b;
    /* 0x2D8C */ u8 field207_0x2d8c;
    /* 0x2D8D */ u8 field208_0x2d8d;
    /* 0x2D8E */ u8 field209_0x2d8e;
    /* 0x2D8F */ u8 field210_0x2d8f;
    /* 0x2D90 */ starAnimationRelated* starAnimRelated;
    /* 0x2D94 */ HugeAnimEntry2D94* entries_2D94;
    /* 0x2D98 */ s32 field213_0x2d98;
    /* 0x2D9C */ struct ActLayout* ActorPointer;
    /* 0x2DA0 */ s32 field215_0x2da0;
    /* 0x2DA4 */ s32 field216_0x2da4;
    /* 0x2DA8 */ s32 field217_0x2da8;
    /* 0x2DAC */ s32 field218_0x2dac;
    /* 0x2DB0 */ s32 field219_0x2db0;
    /* 0x2DB4 */ s32 field220_0x2db4;
    /* 0x2DB8 */ s32 field221_0x2db8;
    /* 0x2DBC */ s32 field222_0x2dbc;
    /* 0x2DC0 */ s32 field223_0x2dc0;
    /* 0x2DC4 */ s32 field224_0x2dc4;
    /* 0x2DC8 */ s32 field225_0x2dc8;
    /* 0x2DCC */ s32 field226_0x2dcc;
    /* 0x2DD0 */ s32 field227_0x2dd0;
    /* 0x2DD4 */ s32 field228_0x2dd4;
    /* 0x2DD8 */ s32 field229_0x2dd8;
    /* 0x2DDC */ s32 field230_0x2ddc;
    /* 0x2DE0 */ s32 field231_0x2de0;
    /* 0x2DE4 */ s32 field232_0x2de4;
    /* 0x2DE8 */ f32 field233_0x2de8;
    /* 0x2DEC */ s32 field234_0x2dec;
    /* 0x2DF0 */ s32 field235_0x2df0;
    /* 0x2DF4 */ s32 field236_0x2df4;
    /* 0x2DF8 */ s32 field237_0x2df8;
    /* 0x2DFC */ s32 field238_0x2dfc;
    /* 0x2E00 */ s32 field239_0x2e00;
    /* 0x2E04 */ s32 field240_0x2e04;
    /* 0x2E08 */ s32 field241_0x2e08;
    /* 0x2E0C */ s32 field242_0x2e0c;
    /* 0x2E10 */ s32 field243_0x2e10;
    /* 0x2E14 */ s32 field244_0x2e14;
    /* 0x2E18 */ s32 field245_0x2e18;
    /* 0x2E1C */ s32 field246_0x2e1c;
    /* 0x2E20 */ s32 field247_0x2e20;
    /* 0x2E24 */ s32 field248_0x2e24[3];
    /* 0x2E30 */ s32 field249_0x2e30[3];
    /* 0x2E3C */ s32 field250_0x2e3c[3];
    /* 0x2E48 */ s32 field251_0x2e48[3];
    /* 0x2E54 */ s32 field252_0x2e54[3];
    /* 0x2E60 */ s32 field253_0x2e60[3];
    /* 0x2E6C */ s32 field254_0x2e6c[3];
    /* 0x2E78 */ s32 field255_0x2e78[3];
    /* 0x2E84 */ s32 field256_0x2e84[3];
    /* 0x2E90 */ s32 field257_0x2e90[3];
    /* 0x2E9C */ s32 field258_0x2e9c[3];
    /* 0x2EA8 */ s32 field259_0x2ea8;
    /* 0x2EAC */ u8 field260_0x2eac[8];
    /* 0x2EB4 */ s32 field261_0x2eb4[3];
    /* 0x2EC0 */ s32 field262_0x2ec0;
    /* 0x2EC4 */ u8 field263_0x2ec4[8];
    /* 0x2ECC */ s32 field264_0x2ecc[3];
    /* 0x2ED8 */ s32 field265_0x2ed8[102];
    /* 0x3070 */ void (*funcPtr)(s32);
    /* 0x3074 */ void (*funcPtr2)(s32);
    /* 0x3078 */ s16 field268_0x3078;
    /* 0x307A */ u8 field269_0x307a;
    /* 0x307B */ u8 field270_0x307b;
    /* 0x307C */ u8 field271_0x307c;
    /* 0x307D */ u8 field272_0x307d;
    /* 0x307E */ u8 renderStadiumFlag;
    /* 0x307F */ u8 field274_0x307f[2];
    /* 0x3081 */ u8 field275_0x3081;
    /* 0x3082 */ u8 field276_0x3082;
    /* 0x3083 */ u8 field277_0x3083[2];
    /* 0x3085 */ u8 field278_0x3085;
    /* 0x3086 */ u8 field279_0x3086;
    /* 0x3087 */ u8 field280_0x3087;
    /* 0x3088 */ u8 displayObjects;
    /* 0x3089 */ u8 field282_0x3089[3];
    /* 0x308C */ void (*funcPtr3)(void*);
    /* 0x3090 */ u8 field284_0x3090[12];
    /* 0x309C */ void* field285_0x309c[3];
    /* 0x30A8 */ u8 field286_0x30a8[32];
    /* 0x30C8 */ void* field287_0x30c8[2];
    /* 0x30D0 */ u8 field288_0x30d0[40];
    /* 0x30F8 */ void* field289_0x30f8[3];
    /* 0x3104 */ u8 field290_0x3104[32];
    /* 0x3124 */ void* field291_0x3124[2];
    /* 0x312C */ u8 field292_0x312c[40];
} hugeAnimStruct; /* size: 0x3154 */
extern hugeAnimStruct g_hugeAnimStruct;
void fn_800BD670(HugeAnimCtrlObj* obj, s32 arg1);
void fn_800BD548(HugeAnimSubObj34* sub, s32 count, void* a, void* b, void* c, void* d, ...);
void fn_800BD8C4(HugeAnimCtrlObj* obj, s32 arg);
extern f32 lbl_3_rodata_1414;

typedef struct StadiumObjectElem {
    u8 _pad[0x80];
    BALL_COLLISION_TYPE (*fnPtr)(u32, BALL_COLLISION_TYPE, CollisionStruct*);  /* 0x80 */
    u8 _pad2[0xE8 - 0x84];
} StadiumObjectElem;

typedef struct UnknownStruct_350E4 {
    /* 0x000 */ StadiumObjectElem* src;
    /* 0x004 */ void* dest;
    /* 0x008 */ u8 _pad_0x8[0x28];
    /* 0x030 */ u32 memcpyCount;
    /* 0x034 */ u32 unk34;
    /* 0x038 */ u32 unk38;
    /* 0x03C */ u32* unk3C;
    /* 0x040 */ u16* unk40;
    /* 0x044 */ u32* unk44;
    /* 0x048 */ VecXYZ* unk48;
    /* 0x04C */ u8 _pad_0x4c[0x1A];
    /* 0x066 */ s16 unk66;
    /* 0x068 */ s16 unk68;
    /* 0x06A */ u8 _pad_0x6a[0x2];
    /* 0x06C */ u8 unk6C;
} UnknownStruct_350E4;

extern UnknownStruct_350E4 lbl_3_common_bss_350E4[];

typedef struct Bss_1910 {
f32 unk0;
f32 unk4;
f32 unk8;
f32 unkC;
f32 unk10;
f32 unk14;
} Bss_1910;
extern Bss_1910 lbl_3_bss_1910;
extern f32 lbl_3_rodata_1DD0;
extern f32 lbl_3_rodata_1DD4;
typedef struct QueueEntry {
    u8 _pad0[0x10];
    s16 unk10;     /* 0x10 */
    u8 _pad1[0x02];
    VecXYZ vec;    /* 0x14 */
    s32 unk20;     /* 0x20 */
    u8* unk24;     /* 0x24 */
} QueueEntry;
/* extern function with proper types */
QueueEntry* fn_800B0A5C_insertQueue(QueueCallback callback, s32 priority);
typedef struct UnknownStruct_EF3D4 {
u8 _pad0[0x74];
s32 unk74;
} UnknownStruct_EF3D4;
/* The global is accessed as an array of 32-bit values (pointers or ints) */
extern s32 lbl_3_bss_B154[];
typedef struct {
    u8 pad[0x14];
    f32 unk14;
} UnknownStructB7;
extern UnknownStructB7 lbl_3_data_4444;

typedef struct DataArrayElement {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ u8 _pad[2];
    /* 0xE */ u8 flag;
    /* 0xF */ u8 _pad2;
} DataArrayElement; // size: 0x10

typedef struct BssTransRotData {
    u8 _pad[4];      /* 0x00 */
    f32 transX;      /* 0x04 */
    f32 transY;      /* 0x08 */
    f32 transZ;      /* 0x0C */
    f32 rotX;        /* 0x10 */
    f32 rotY;        /* 0x14 */
    f32 rotZ;        /* 0x18 */
} BssTransRotData; /* size: 0x1C */

typedef struct CommonBss_35154 {
    /* 0x000 */ u32 firstMember;
    /* 0x004 */ u8 _pad_1[4];
    /* 0x008 */ void* field_0x8;
    /* 0x00C */ u8 _pad0[0x3AC - 0xC];
    /* 0x3AC */ u32 bitfield;
    /* 0x3B0 */ u8 unk_3b0;
    /* 0x3B1 */ u8 _pad3b1[7];
    /* 0x3B8 */ DataArrayElement* unk_3b8;
    /* 0x3BC */ f32 unk_3bc;
    /* 0x3C0 */ f32 unk_3c0;
    /* 0x3C4 */ f32 unk_3c4;
    /* 0x3C8 */ u32 unk_3c8;
    /* 0x3CC */ u32 unk_3cc;
    /* 0x3D0 */ f32 unk_3d0;
    /* 0x3D4 */ f32 unk_3d4;
    /* 0x3D8 */ f32 unk_3d8;
    /* 0x3DC */ f32 unk_3dc;
    /* 0x3E0 */ u8 unk_3e0;
    /* 0x3E1 */ u8 _pad3e1[3];
    /* 0x3E4 */ BssTransRotData transRot;
    /* 0x400 */ u8 _pad400[4];
    /* 0x404 */ s16 someHalfword;
    /* 0x406 */ u8 _pad406[4];
    /* 0x40A */ u8 unk_40a;
    /* 0x40B */ u8 _pad40b;
    /* 0x40C */ u32 array_40c[1]; // Array starting at 0x40c
    /* 0x410 */ u8 _pad_mid[0x8];
    /* 0x418 */ u8 someFlag_418;  // stb r0, 0x418(r5)
    /* 0x419 */ u8 unk_419;
    /* 0x41A */ u8 _pad_e[0x2];
    /* 0x41C */ f32 unk_41C;
    /* 0x420 */ f32 unk_420;
    /* 0x424 */ f32 unk_424;
    /* 0x428 */ u8 _pad_end[0xC];
    /* 0x434 */ f32 unk_434;
    /* 0x438 */ f32 unk_438;
    /* 0x43C */ f32 unk_43C;
    /* 0x440 */ f32 unk_440;
    /* 0x444 */ f32 unk_444;
    /* 0x448 */ f32 unk_448;
    /* 0x44C */ u8 _pad3[0x18];
    /* 0x464 */ s16 unk_464;
    /* 0x466 */ u8 someFlag;
    /* 0x467 */ u8 unk_467;
    /* 0x468 */ u8 _pad4[0x11];
    /* 0x479 */ u8 unk_479;
} CommonBss_35154;

extern CommonBss_35154 lbl_3_common_bss_35154;
extern void fn_80034CEC(u32);
extern void minigamesSetSomePointers(void);
typedef struct UnknownBss_803C6CF8 {
    u8 _pad[0x715];
    s8 allowLoad;
} UnknownBss_803C6CF8;

extern UnknownBss_803C6CF8 lbl_803C6CF8;

typedef struct DrawingSceneItem {
    u8 _pad[0x90];
    u8 _b0 : 2;
    u8 b2 : 1;  // bit 5 from LSB (0x20)
    u8 _b3 : 5;
} DrawingSceneItem;

typedef struct DrawingSceneStruct {
    void* aFunctionPointer;
    struct {
        struct DrawingSceneStruct *Prev;
        struct DrawingSceneStruct *Next;
    } Link;
    struct DrawingSceneStruct *currentDrawingItem;
    short loadingState;
    u16 priority;
    union {
        struct {
            u16 unkIndex;
            u16 unkIndex2;
        };
        DrawingSceneItem *unk14;
    };
    s16 unk18;
    s16 unk1a;
    u16 eventID;
    u16 fielderIndex;
    u8 field12_0x20[2];
    short drawingState;
    u8 field14_0x24[4];
    u8 field15_0x28;
    u8 field16_0x29;
    u8 field17_0x2a;
    u8 field18_0x2b;
    short field19_0x2c;
} DrawingSceneStruct;

extern DrawingSceneStruct *lbl_803CC1B8;
typedef struct CameraStruct {
    u8 _pad0[0xAB0];
    void* unkAB0;   // 0xAB0 - allocated buffer
    u8 _padAB4[0x9B8];
    void* unk146C;  // 0x146C - allocated buffer
    u8 _pad1470[0x1440];
} CameraStruct;

extern CameraStruct g_Camera;
extern void* _OSAllocFromHeap(s32 heap, u32 size);

extern void (*lbl_3_data_11390[])(u32);
extern void fn_800B0A14_removeQueue(void);
// The assembly passes the index in r4
extern VecXZ lbl_3_data_111C8[];
// Extern function prototype based on bl fn_8003A688
extern void fn_8003A688(s32, f32, f32);
extern u8 lbl_3_bss_B894[0xF];
typedef struct CommonBss32A94 {
    s16 unk0;           /* 0x00 */
    s16 unk2;
    s16 unk4;
    u8 _pad6[0x44 - 0x06];
    s16 unk44[2];  /* 0x44 */
    u8 _pad48[0x7D - 0x48];
    u8 unk7D;
    u8 _pad7E;
    u8 unk7F;
    u8 _pad80;
    u8 unk81;
    u8 unk82;
    u8 _pad83;
    u8 unk84;
    u8 unk85;
    u8 _pad86;
    u8 unk87;
    u8 unk88;
    u8 _pad89;
    u8 unk8A;
    u8 _pad8B;
} CommonBss32A94; // size: 0x8C
extern CommonBss32A94 lbl_3_common_bss_32A94;
extern void fn_3_FBE24(void);
extern f32 lbl_3_rodata_1998;
extern void fn_3_5A6D4(u8);
extern f32 fn_800B4A94(AnimLevel1* arg0, hugeAnimStruct* arg1);
typedef struct Struct_14D6D4_Node {
struct Struct_14D6D4_Node* next; // 0x00
u8 _pad0[0x40];                  // Padding to 0x44
u8 unk44;                        // 0x44
u8 unk45;                        // 0x45
u8 unk46;                        // 0x46
u8 _pad1[3];                     // Padding to 0x4A
s16 unk4A;                       // 0x4A
u8 _pad2[2];                     // Padding to 0x4E
u8 unk4E;                        // 0x4E
} Struct_14D6D4_Node;
typedef struct Struct_14D6D4_Input {
u8 _pad0[0x0C];
Struct_14D6D4_Node* listHead;    // 0x0C
s32 field10;                     // 0x10
} Struct_14D6D4_Input;

typedef struct CommonBss_32724 {
    /* 0x00 */ u8 _pad0[0x6C];
    /* 0x6C */ s32 field6C;      // From fn_3_14D6D4
    /* 0x70 */ u8 _pad1[0x20];   // Pad to 0x90
    /* 0x90 */ s16 unk_90;
    /* 0x92 */ s16 unk_92;
    /* 0x94 */ u8 _pad2[0x2];
    /* 0x96 */ u8 someStatusInd;
    /* 0x97 */ u8 _pad3[0x12];
    /* 0xA9 */ u8 unk_A9;
    /* 0xAA */ u8 _pad4[0x0B];
    /* 0xB5 */ u8 unk_B5;
    /* 0xB6 */ u8 _pad5[0x12];   // Pad to 0xC8
    /* 0xC8 */ u8 field_0xC8;
    /* 0xC9 */ u8 field_0xC9;
    /* 0xCA */ u8 field_0xCA;
    /* 0xCB */ u8 flagCB;        // New flag
    /* 0xCC */ u8 flagCC;  
    /* 0xCD */ u8 transitionToMenuAnimation;      // New flag
    /* 0xCD */ u8 _pad6[0x05];   // Pad to 0xD3 (0xD3 - 0xCD = 0x06)
    /* 0xD3 */ u8 unk_D3;
    /* 0xD4 */ u8 someFlag;
} CommonBss_32724;

extern CommonBss_32724 lbl_3_common_bss_32724;
// Definition for the global structure at 0x800EFBA4
typedef struct SoundConfig {
u8 _pad[0x6];
u8 volume; // Offset 0x6
} SoundConfig;
extern SoundConfig lbl_800EFBA4;
s32 rand();                                         /* extern */
extern s16 lbl_3_bss_170;
void AnimateCharacter(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, u8 arg7, int arg8);
extern f32 lbl_3_rodata_2B28;

typedef struct ActLayout ActLayout;
// --- Enums ---
typedef enum thwompState {
    THWOMP_IDLE = 0,
    THWOMP_FALLING = 1,
    THWOMP_GROUNDED = 2,
    THWOMP_RISING = 3
} thwompState;

typedef enum barrelState {
    BARREL_IDLE = 0,
    BARREL_ACTIVE = 1
} barrelState;

// --- The Main Object Struct ---
typedef struct ThwompActData {
    u8 _00[0x5C];
    f32 unk5C;
} ThwompActData;

typedef struct castleThwompObj {
    /* 0x00 */ Control control;      // Ghidra says Length 52 (0x34)
    /* 0x34 */ u8 pad[8];           // Length 16 (0x10)
    /* 0x44 */ Mtx mtx;              // Length 48 (0x30)
    /* 0x74 */ ThwompActData *actPtr; // Length 4
    /* 0x78 */ s32 vertexData;       // Length 4
    /* 0x7C */ void (*functionPtr)(void *);
    /* 0x80 */ void (*unkFunctionPtr)(void *);
    /* 0x84 */ s32 field7_0x84;
    /* 0x88 */ s32 field8_0x88;
    /* 0x8C */ s32 field9_0x8c;
    /* 0x90 */ u8 flags[2];
    /* 0x92 */ u8 field11_0x92;
    /* 0x93 */ u8 field12_0x93;
    /* 0x94 */ s16 field13_0x94;
    /* 0x96 */ s16 field14_0x96;
    /* 0x98 */ u8 field15_0x98;
    /* 0x99 */ u8 hasBeenResetAfterLiveBallInd;
    /* 0x9A */ u8 field17_0x9a[2];
    /* 0x9C */ VecXYZ position;       // Length 12 (Ends at 0xA8)

    /* 0xA8 */ u8 id;
    /* 0xA9 */ u8 starCollectedStatus;
    /* 0xAA */ u8 field21_0xaa;
    /* 0xAB */ u8 field22_0xab;
    /* 0xAC */ s32 unkAC;             // Length 4 (Ends at 0xB0)

    /* 0xB0 */ u8 state;
    /* 0xB1 */ u8 framesOnGround;
    /* 0xB2 */ u8 checkForSlamInd;
    /* 0xB3 */ u8 field27_0xb3;      // Exactly 4 u8s total here

    /* 0xB4 */ VecXYZ rotation;       // Starts at 180 (0xB4)

    /* 0xC0 */ f32 alsoRotation_maybe;
    /* 0xC4 */ u8 objectState;
    /* 0xC5 */ u8 maybeObjsToScale;
    /* 0xC6 */ u8 state_alt;
    /* 0xC7 */ u8 counterBetweenActions;
    /* 0xC8 */ u8 field34_0xc8;
    /* 0xC9 */ u8 field35_0xc9;
    /* 0xCA */ u8 maybeIsActive;
    /* 0xCB */ u8 field37_0xcb;
    /* 0xCC */ u8 field38_0xcc[28];
} castleThwompObj;

s32 fn_80033928(s32 arg0);
u32 fn_80033A24(void (*func)(void), s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

typedef struct Scores {
    /* 0x00 */ s32 highscore;    // This might be Inning
    /* 0x04 */ s16 ScoreTeam0;    // Target for lha r7, 0x4(r6)
    u8 _pad0[0x2A - 0x06];        // 0x06 to 0x29
    /* 0x2A */ s16 ScoreTeam1;
    u8 _pad1[0x9C - 0x2C];        // Pad to 0x9C
    /* 0x9C */ s16 field_0x9C;
    u8 _pad2[0xAA - 0x9E];        // Pad to 0xAA
    /* 0xAA */ u8 threshold;     // Matches 'lbz r0, 0xaa(r4)'
    /* 0xAB */ u8 _pad3[24];     // Padding to reach your original 0xC3
    /* 0xC3 */ u8 someFlag;
} Scores;

extern Scores g_Scores;
extern VecXZ lbl_3_data_446C;
extern int lbl_3_data_F350;
// r3 = value, r4 = address, r5 = address
void fn_80034E20(void (*first_arg)(void), int *second_arg, void **third_arg);
extern int lbl_3_data_E120;
extern int lbl_3_data_C1EC;
// Identified as Vec3f_807d2604 in Ghidra
extern Vec lbl_3_data_21004;
typedef struct Struct_800D249C_C4 {
u8 _pad0[0xC];     // Gap: 0x00 - 0x0C = 0xC
Vec field_0x0C;      // Offset 0x0C (accessed via r4 + 0xC)
u8 _pad1[0x1B4];     // Gap: 0x1CC - (0x0C + 0x0C) = 0x1B4
Vec field_0x1CC;     // Offset 0x1CC (accessed via r4 + 0x1CC)
} Struct_800D249C_C4;
typedef struct Struct_800D249C_Arg0 {
u8 _pad0[0xC4];
Struct_800D249C_C4 *ptr_0xC4; // Offset 0xC4
} Struct_800D249C_Arg0;
extern f64 lbl_3_rodata_2800;
extern void* lbl_3_bss_1904;
extern void fn_800ACFB0(void*);
extern void fn_80035CA4(s32 arg);
extern void fn_800BCDBC(void* ptr);
extern VecXYZ lbl_3_data_21A48;
extern f32 lbl_3_rodata_35D0;
extern f32 lbl_3_rodata_3638;
typedef struct GameStateBss {
s32 val0; // 0x00
s32 val1; // 0x04
s32 val2; // 0x08
s32 val3; // 0x0C
s32 val4; // 0x10
s32 val5; // 0x14
} GameStateBss;
extern GameStateBss lbl_3_bss_D6F0;
extern f32 lbl_3_rodata_B20;
void fn_800B0A14_removeQueue(void);
#endif
