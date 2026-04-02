#include "game/gap_1665E4.h"

#include "UnknownHeaders.h"
#include "game/rep_1C0.h"
#include "game/rep_3E00.h"

extern CompressedDiskRead lbl_3_data_284E8[];

// .text:0x001665E4 size:0xCC
s32 fn_3_1665E4(void) {
    switch (lbl_3_common_bss_32724.unkD8) {
    case 0:
        g_hugeAnimStruct.ActorPointer = ARAMTransfer(&lbl_3_data_284E8[1], NULL, 0, NULL);
        lbl_3_common_bss_32724.unkD8 += 1;
        break;
    case 1:
        if (lbl_803C6CF8.allowLoad == 1) {
            lbl_3_common_bss_32724.unkD8 += 1;
        }
        break;
    case 2:
        fn_3_166448();
        lbl_3_common_bss_32724.unkD8 += 1;
        break;
    default:
        return 1;
    }
    return 0;
}
