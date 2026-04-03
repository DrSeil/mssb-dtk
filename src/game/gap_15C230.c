#include "game/gap_15C230.h"
#include "UnknownHeaders.h"
#include "game/UnknownHomes_Game.h"
#include "Dolphin/gx.h"

extern void gOz_GXSetTexture(s32, s32, s32);
extern void SetDisplayStateTexture(void*, s32, s32);

extern u32 lbl_3_data_273D4[];
extern u32 lbl_3_data_273CC[];

// .text:0x0015C230 size:0x1C8 mapped:0x8079B2BC
void fn_3_15C230(void* arg0) {
    register u8* r31_ptr = (u8*)arg0;
    register u32 r30_color;
    register s32 r0_cond;
    register s32 r3_val;
    
    gOz_GXSetTexture(0, 0, 0);
    GXSetBlendMode(1, 4, 1, 0);
    GXSetZMode(1, 3, 0);
    
    {
        register u32* r3_bss = (u32*)&lbl_3_common_bss_35154;
        SetDisplayStateTexture((void*)(r3_bss[1] + 0x2E4), 0, 0);
    }
    
    GXSetProjectionv((f32*)(r31_ptr + 0x38));
    GXLoadPosMtxImm((MtxPtr)(r31_ptr + 0x8), 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(GX_CULL_NONE);
    
    if (g_GameLogic.bOD_framesInLiveBallScene >= 0) {
        r0_cond = 2;
    } else {
        r3_val = (s32)g_Ball.framesSinceHit;
        r0_cond = (u32)(-(s32)r3_val & ~r3_val) >> 31;
    }
    
    if (r0_cond == 2) {
        r30_color = lbl_3_data_273D4[1] | 0xFFFFFF00;
    } else {
        r30_color = lbl_3_data_273CC[1] | 0xFFFFFF00;
    }
    
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    
    // Vertex 1
    GXPosition3f32(*(f32*)(r31_ptr + 0x54), *(f32*)(r31_ptr + 0x58), *(f32*)(r31_ptr + 0x5C));
    GXColor1u32(r30_color);
    GXTexCoord2s16(0, 0);
    
    // Vertex 2
    GXPosition3f32(*(f32*)(r31_ptr + 0x60), *(f32*)(r31_ptr + 0x64), *(f32*)(r31_ptr + 0x68));
    GXColor1u32(r30_color);
    GXTexCoord2s16(1, 0);
    
    // Vertex 3
    GXPosition3f32(*(f32*)(r31_ptr + 0x6C), *(f32*)(r31_ptr + 0x70), *(f32*)(r31_ptr + 0x74));
    GXColor1u32(r30_color);
    GXTexCoord2s16(1, 1);
    
    // Vertex 4
    GXPosition3f32(*(f32*)(r31_ptr + 0x78), *(f32*)(r31_ptr + 0x7C), *(f32*)(r31_ptr + 0x80));
    GXColor1u32(r30_color);
    GXTexCoord2s16(0, 1);
    
    {
        u8* r4_ptr = *(u8**)(r31_ptr + 0x84);
        r4_ptr[0]--;
    }
}
