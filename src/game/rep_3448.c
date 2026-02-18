#include "game/rep_3448.h"
#include "UnknownHeaders.h"
#include "header_rep_data.h"

// .text:0x0011EC28 size:0x404 mapped:0x8075DCBC
void fn_3_11EC28(void) {
    return;
}

// .text:0x0011F02C size:0x454 mapped:0x8075E0C0
void fn_3_11F02C(void) {
    return;
}

// .text:0x0011F480 size:0x34 mapped:0x8075E514

#include "game/UnknownHomes_Game.h"

/**
 * @address 0011F480
 */

#include "game/UnknownHomes_Game.h"

/**
 * @address 0011F480
 */
void fn_3_11F480(void) {
    MiniGameStruct* mini = &g_Minigame;

    /* Establishing r3 and checking condition */
    if (mini->GameMode_MiniGame == 4) {
        /* To force 'li r4, 0' to stay after the bnelr:
           We initialize it here. If it still hoists, we use a dummy 
           inline block or a volatile-like approach. */
        u32 i = 0; 
        
        do {
            /* lha r0, 0x1890(r3) */
            s16 points = *(s16*)((u8*)mini + 0x1890);
            
            /* The increment and compare are interleaved to match 
               Target scheduling: addi r4, r4, 1 -> cmplwi r4, 4 */
            i++;
            
            /* sth r0, 0x1df4(r3) */
            *(s16*)((u8*)mini + 0x1df4) = points;

            /* addi r3, r3, 2 */
            mini = (MiniGameStruct*)((u8*)mini + 2);

        } while (i < 4);
    }
}

// .text:0x0011F4B4 size:0x54 mapped:0x8075E548
void fn_3_11F4B4(void) {
    return;
}

// .text:0x0011F508 size:0x270 mapped:0x8075E59C
void fn_3_11F508(void) {
    return;
}

// .text:0x0011F778 size:0x2E0 mapped:0x8075E80C
void fn_3_11F778(void) {
    return;
}

// .text:0x0011FA58 size:0x358 mapped:0x8075EAEC
void fn_3_11FA58(void) {
    return;
}

// .text:0x0011FDB0 size:0x4BC mapped:0x8075EE44
void fn_3_11FDB0(void) {
    return;
}

// .text:0x0012026C size:0x630 mapped:0x8075F300
void fn_3_12026C(void) {
    return;
}

// .text:0x0012089C size:0x6C0 mapped:0x8075F930
void fn_3_12089C(void) {
    return;
}

// .text:0x00120F5C size:0x9C mapped:0x8075FFF0
void fn_3_120F5C(void) {
    return;
}

// .text:0x00120FF8 size:0x30C mapped:0x8076008C
void fn_3_120FF8(void) {
    return;
}

// .text:0x00121304 size:0x604 mapped:0x80760398
void fn_3_121304(void) {
    return;
}

// .text:0x00121908 size:0xA2C mapped:0x8076099C
void fn_3_121908(void) {
    return;
}

// .text:0x00122334 size:0x3A0 mapped:0x807613C8
void fn_3_122334(void) {
    return;
}

// .text:0x001226D4 size:0x650 mapped:0x80761768
void fn_3_1226D4(void) {
    return;
}

// .text:0x00122D24 size:0x4B0 mapped:0x80761DB8
void fn_3_122D24(void) {
    return;
}

// .text:0x001231D4 size:0x3E4 mapped:0x80762268
void fn_3_1231D4(void) {
    return;
}

// .text:0x001235B8 size:0x3D8 mapped:0x8076264C
void fn_3_1235B8(void) {
    return;
}

// .text:0x00123990 size:0x52C mapped:0x80762A24
void fn_3_123990(void) {
    return;
}

// .text:0x00123EBC size:0x4E8 mapped:0x80762F50
void fn_3_123EBC(void) {
    return;
}

// .text:0x001243A4 size:0x394 mapped:0x80763438
void fn_3_1243A4(void) {
    return;
}

// .text:0x00124738 size:0x5A8 mapped:0x807637CC
void fn_3_124738(void) {
    return;
}

// .text:0x00124CE0 size:0x68C mapped:0x80763D74
void fn_3_124CE0(void) {
    return;
}

// .text:0x0012536C size:0xB8 mapped:0x80764400
void fn_3_12536C(void) {
    return;
}

// .text:0x00125424 size:0x5C mapped:0x807644B8
void fn_3_125424(void) {
    return;
}

// .text:0x00125480 size:0x78 mapped:0x80764514
void fn_3_125480(void) {
    return;
}

// .text:0x001254F8 size:0x10C mapped:0x8076458C
void fn_3_1254F8(void) {
    return;
}

// .text:0x00125604 size:0x24C mapped:0x80764698
void fn_3_125604(void) {
    return;
}

// .text:0x00125850 size:0x70 mapped:0x807648E4
void fn_3_125850(void) {
    return;
}

// .text:0x001258C0 size:0xD44 mapped:0x80764954
void fn_3_1258C0(void) {
    return;
}

// .text:0x00126604 size:0xEB0 mapped:0x80765698
void fn_3_126604(void) {
    return;
}

// .text:0x001274B4 size:0x6B4 mapped:0x80766548
void fn_3_1274B4(void) {
    return;
}

// .text:0x00127B68 size:0xED0 mapped:0x80766BFC
void fn_3_127B68(void) {
    return;
}

// .text:0x00128A38 size:0x158 mapped:0x80767ACC
void fn_3_128A38(void) {
    return;
}

// .text:0x00128B90 size:0x88 mapped:0x80767C24
void fn_3_128B90(void) {
    return;
}

// .text:0x00128C18 size:0x758 mapped:0x80767CAC
void fn_3_128C18(void) {
    return;
}

// .text:0x00129370 size:0x60 mapped:0x80768404
void fn_3_129370(void) {
    return;
}
