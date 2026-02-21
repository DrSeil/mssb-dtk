#ifndef UNKNOWN_HEADERS_H
#define UNKNOWN_HEADERS_H

#include "types.h"
#include "game/UnknownHomes_Game.h"

typedef void (*QueueCallback)(void);
void fn_800528AC(void (*callback)(void), s32 *arg1);
typedef struct Struct_3_2281C_Entry {
    u8 _pad0[0x68];
    s16 field_0x68;
} Struct_3_2281C_Entry;

/* Forward declarations for types used as pointers in hugeAnimStruct */
struct StadiumFileHeader;
struct ACTBone;
struct ActLayout;
struct AnimBank;
struct starAnimationRelated;

/* AnimationStruct - 0x27C bytes, used in arrays within hugeAnimStruct.
 * Full field definitions available in in_game.h.
 * Using opaque byte array to guarantee exact size for struct layout. */
typedef struct AnimationStruct {
    u8 data[0x27C];
} AnimationStruct; /* size: 0x27C */

/* EOSPad - 2-byte struct used for alignment padding */
typedef struct EOSPad {
    u8 pad[2];
} EOSPad;

/* unkAnimSubstruct - 0xD2 bytes.
 * Full field definitions available in in_game.h.
 * Using opaque byte array to prevent compiler alignment padding. */
typedef struct unkAnimSubstruct {
    u8 data[0xD2];
} unkAnimSubstruct; /* size: 0xD2 */

typedef struct HugeAnimEntry2D94 {
    // Not confident of these
    /* 0x00 */ void* stadiumFileHeaderPointer;
    /* 0x04 */ void* stadiumObjectLoadingPtr;
    /* 0x08 */ s32 unk08[7]; // 0x08 through 0x24
    /* 0x24 */ s32 unk24;
    
    // --- THE TARGET BYTE ---
    /* 0x26 */ u8 unk_26; // The byte zeroed in fn_3_116B38
    // -----------------------
    
    /* 0x27 */ u8 _pad[0x28 - 0x27]; // Ensure total size is 0x28 (40 bytes)
} HugeAnimEntry2D94;
typedef struct HugeAnimEntry68 {
    u8 _pad0[0x34];
    s32 unk34;      // offset 0x34, matches lwz r3, 0x34(r3)
    u8 _pad1[0x58]; // (0x90 - 0x38) bytes to make the struct size exactly 0x90
} HugeAnimEntry68;

typedef struct hugeAnimStruct {
    /* 0x0000 */ u8 field0_0x0[4];
    /* 0x0004 */ struct StadiumFileHeader* stadiumFileHeaderPointer;
    /* 0x0008 */ void* stadiumObjectLoadingPtr;
    /* 0x000C */ s32 field3_0xc;
    /* 0x0010 */ s32 field4_0x10;
    /* 0x0014 */ s32 field5_0x14;
    /* 0x0018 */ s32 field6_0x18;
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
    /* 0x0064 */ struct ACTBone* actRelatedPtr;
    /* 0x0068 */ HugeAnimEntry68* entries_68;
    /* 0x006C */ void* unkPtr;
    /* 0x0070 */ s32 field28_0x70;
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
    /* 0x0212 */ EOSPad pad3;
    /* 0x0214 */ unkAnimSubstruct field68_0x214;
    /* 0x02E6 */ EOSPad pad4;
    /* 0x02E8 */ unkAnimSubstruct field70_0x2e8;
    /* 0x03BA */ EOSPad pad5;
    /* 0x03BC */ unkAnimSubstruct field72_0x3bc;
    /* 0x048E */ EOSPad pad6;
    /* 0x0490 */ unkAnimSubstruct field74_0x490;
    /* 0x0562 */ EOSPad pad7;
    /* 0x0564 */ unkAnimSubstruct field76_0x564;
    /* 0x0636 */ EOSPad pad8;
    /* 0x0638 */ unkAnimSubstruct field78_0x638;
    /* 0x070A */ EOSPad pad9;
    /* 0x070C */ unkAnimSubstruct field80_0x70c;
    /* 0x07DE */ EOSPad pad10;
    /* 0x07E0 */ unkAnimSubstruct field82_0x7e0;
    /* 0x08B2 */ EOSPad pad11;
    /* 0x08B4 */ unkAnimSubstruct field84_0x8b4;
    /* 0x0986 */ EOSPad pad12;
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
    /* 0x0C04 */ AnimationStruct fielderStructs[10];
    /* 0x24DC */ u8 field99_0x24dc[8];
    /* 0x24E4 */ AnimationStruct RunnerStructs[2];
    /* 0x29DC */ void* field101_0x29dc[9];
    /* 0x2A00 */ s32 field102_0x2a00;
    /* 0x2A04 */ s32 field103_0x2a04[11];
    /* 0x2A30 */ s32 field104_0x2a30[128];
    /* 0x2C30 */ u8 field105_0x2c30[24];
    /* 0x2C48 */ u8 field106_0x2c48[4];
    /* 0x2C4C */ s32 field107_0x2c4c;
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
    /* 0x2D90 */ struct starAnimationRelated* starAnimRelated;
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
    /* 0x3070 */ void (*funcPtr)(void);
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
void fn_800BD670(s32 fieldVal, s32 arg1, hugeAnimStruct* globalPtr);
extern f32 lbl_3_rodata_1414;

typedef struct UnknownStruct_350E4 {
    u8 _pad0[0x3C];
    u32* unk3C;         /* 0x3C - Pointer to u32 array */
    u16* unk40;         /* 0x40 - Pointer to u16 array */
    u32* unk44;         /* 0x44 - Pointer to u32 array base */
    u8 _pad1[0x6C-0x48];
    u8 unk6C;           /* 0x6C */
    u8 _pad2[0xC0-0x6D];
    u8 targetByte;      /* 0xC0 */
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
s16 unk10;
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

typedef struct CommonBss_35154 {
    /* 0x000 */ u32 firstMember; // Previously inside _pad0
    /* 0x004 */ u8 _pad0[0x3AC - 4];
    /* 0x3AC */ u32 bitfield;
    /* 0x3B0 */ u8 _pad_extra[0x54];
    /* 0x404 */ s16 someHalfword;
    /* 0x406 */ u8 _pad2[0x2E];
    /* 0x434 */ f32 unk_434;
    /* 0x438 */ f32 unk_438;
    /* 0x43C */ f32 unk_43C;
    /* 0x440 */ u8 _pad3[0x26];
    /* 0x466 */ u8 someFlag;
    /* 0x467 */ u8 unk_467;
} CommonBss_35154;

extern CommonBss_35154 lbl_3_common_bss_35154;
extern void fn_80034CEC(u32);
// The assembly passes the index in r4
extern VecXZ lbl_3_data_111C8[];
// Extern function prototype based on bl fn_8003A688
extern void fn_8003A688(f32, f32);
extern u8 lbl_3_bss_B894[0xF];
typedef struct CommonBss32A94 {
u8 _pad[0x4];
s16 unk4;
} CommonBss32A94;
extern CommonBss32A94 lbl_3_common_bss_32A94;
extern void fn_3_FBE24(void);
extern f32 lbl_3_rodata_1998;
extern void fn_3_5A6D4(s32, GameControlsStruct *);
extern f32 fn_800B4A94(s32, hugeAnimStruct*);
#endif
