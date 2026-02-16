#include "game/rep_36D8.h"
#include "header_rep_data.h"

// .text:0x0013C7BC size:0xDBC mapped:0x8077B850
void fn_3_13C7BC(void) {
    return;
}

// .text:0x0013D578 size:0x70 mapped:0x8077C60C
void fn_3_13D578(void) {
    return;
}

extern f32 lbl_3_rodata_3728;

// .text:0x0013D5E8 size:0x30 mapped:0x8077C67C
//Two instructions swapped order
s32 fn_3_13D5E8(unk_13d5e8_struct* a, unk_13d5e8_struct* b) {
f32 scale;
    f32 valB;
    f32 valA;
    
    // Force scale to be the first float dereferenced in the function logic
    scale = lbl_3_rodata_3728; 
    
    // By separating these into distinct lines AFTER scale is assigned,
    // we nudge the compiler to follow this sequence for the lfs instructions.
    valB = b->value; // Target: lfs f0, 0x0(r4)
    valA = a->value; // Target: lfs f1, 0x0(r3)

    // The arithmetic (scale * valA) - (scale * valB) 
    // triggers fmuls f0, f2, f0 then fmsubs f0, f2, f1, f0
    return (s32)((scale * a->value) - (scale * b->value));
}

// .text:0x0013D618 size:0x408 mapped:0x8077C6AC
void fn_3_13D618(void) {
    return;
}

// .text:0x0013DA20 size:0x30 mapped:0x8077CAB4
extern f32 lbl_3_rodata_3728;

// .text:0x0013DA20 size:0x30 mapped:0x8077CAB4
// Same issue as fn_3_13D5E8.
s32 fn_3_13DA20(unk_13d5e8_struct* a, unk_13d5e8_struct* b) {
    f32 scale;
    f32 valB;
    f32 valA;
    
    // Step 1: Force scale load (lfs f2)
    scale = lbl_3_rodata_3728; 
    
    // Step 2: Force argument loads in specific order (f0 then f1)
    valB = b->value; 
    valA = a->value; 

    // Step 3: Fused multiply-subtract and conversion
    return (s32)((scale * valA) - (scale * valB));
}

// .text:0x0013DA50 size:0x1F8 mapped:0x8077CAE4
void fn_3_13DA50(void) {
    return;
}

// .text:0x0013DC48 size:0x198 mapped:0x8077CCDC
void fn_3_13DC48(void) {
    return;
}

// .text:0x0013DDE0 size:0xC4 mapped:0x8077CE74
void fn_3_13DDE0(void) {
    return;
}

// .text:0x0013DEA4 size:0x118 mapped:0x8077CF38
void fn_3_13DEA4(void) {
    return;
}

// .text:0x0013DFBC size:0x1B8 mapped:0x8077D050
void fn_3_13DFBC(void) {
    return;
}

// .text:0x0013E174 size:0xA8 mapped:0x8077D208
void fn_3_13E174(void) {
    return;
}

// .text:0x0013E21C size:0x188 mapped:0x8077D2B0
void fn_3_13E21C(void) {
    return;
}

// .text:0x0013E3A4 size:0x2CC mapped:0x8077D438
void fn_3_13E3A4(void) {
    return;
}

// .text:0x0013E670 size:0x64 mapped:0x8077D704
void fn_3_13E670(void) {
    return;
}

// .text:0x0013E6D4 size:0x100 mapped:0x8077D768
void fn_3_13E6D4(void) {
    return;
}

// .text:0x0013E7D4 size:0x25C mapped:0x8077D868
void fn_3_13E7D4(void) {
    return;
}

// .text:0x0013EA30 size:0x214 mapped:0x8077DAC4
void fn_3_13EA30(void) {
    return;
}

// .text:0x0013EC44 size:0x840 mapped:0x8077DCD8
void fn_3_13EC44(void) {
    return;
}

// .text:0x0013F484 size:0x244 mapped:0x8077E518
void fn_3_13F484(void) {
    return;
}

// .text:0x0013F6C8 size:0x11C mapped:0x8077E75C
void fn_3_13F6C8(void) {
    return;
}

// .text:0x0013F7E4 size:0xE0 mapped:0x8077E878
void fn_3_13F7E4(void) {
    return;
}

// .text:0x0013F8C4 size:0x360 mapped:0x8077E958
void fn_3_13F8C4(void) {
    return;
}

// .text:0x0013FC24 size:0x660 mapped:0x8077ECB8
void fn_3_13FC24(void) {
    return;
}

// .text:0x00140284 size:0x200 mapped:0x8077F318
void fn_3_140284(void) {
    return;
}

// .text:0x00140484 size:0x154 mapped:0x8077F518
void fn_3_140484(void) {
    return;
}

// .text:0x001405D8 size:0x11C mapped:0x8077F66C
void fn_3_1405D8(void) {
    return;
}

// .text:0x001406F4 size:0x2B8 mapped:0x8077F788
void fn_3_1406F4(void) {
    return;
}

// .text:0x001409AC size:0x220 mapped:0x8077FA40
void fn_3_1409AC(void) {
    return;
}

// .text:0x00140BCC size:0x114 mapped:0x8077FC60
void fn_3_140BCC(void) {
    return;
}

// .text:0x00140CE0 size:0x410 mapped:0x8077FD74
void fn_3_140CE0(void) {
    return;
}

// .text:0x001410F0 size:0x1CC mapped:0x80780184
void fn_3_1410F0(void) {
    return;
}

// .text:0x001412BC size:0x128 mapped:0x80780350
void fn_3_1412BC(void) {
    return;
}

// .text:0x001413E4 size:0xC8 mapped:0x80780478
void fn_3_1413E4(void) {
    return;
}

// .text:0x001414AC size:0x580 mapped:0x80780540
void fn_3_1414AC(void) {
    return;
}
