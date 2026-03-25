#include "game/gap_CB6B4.h"

// .text:0x000CB6B4 size:0x38 mapped:0x8070A748
void fn_3_CB6B4(u32* arg0) {
    *arg0 += (u32)arg0;
    lbl_3_bss_9FDC = *arg0;
    convertTextureHeader(lbl_3_bss_9FDC);
}

// .text:0x000CB6EC size:0x4C mapped:0x8070A780
void fn_3_CB6EC(f32 x, f32 y, f32 z) {
    Vec vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    lbl_3_data_18268 = lbl_3_common_bss_35154.unk4;
    fn_8002955C(&vec, 0, &lbl_3_data_18268);
}

