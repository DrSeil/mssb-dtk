# Working Progress

## Current Focus
**Strategy:** Smallest available functions across all game files
**Next function to attempt:** see score_functions.py output

---

## rep_1E08 — Function Status

### Almost Matched (committed, do not retry)
| Function | Size | Issue |
|---|---|---|
| fn_3_BD6AC | 172 | bge+b+bl branch direction — CW codegen, unfixable |
| fn_3_BD80C | 204 | bss/transRot base register split (r5+r30 vs r30 only) |
| fn_3_BB07C | 224 | sin/cos scheduler: lfsu cascade, +1 instruction |
| fn_3_BF070 | 232 | stmw r27 vs 4 individual stw's — can't get lbl_3_data_111A8 cached in r30 |
| fn_3_BDE14 | 352 | register allocation: r25-r28+stmw vs r29-r31 individual stw; rodata not cached in f27-f31 |

### Matched ✓
| Function | Notes |
|---|---|
| fn_3_BC224 | |
| fn_3_BC25C | |
| fn_3_BC274 | |
| fn_3_BC850 | dummy arg register steering |
| fn_3_BD434 | |
| fn_3_BD4F0 | |
| fn_3_BD758 | |
| fn_3_BD7D0 | |
| fn_3_BD7DC | |
| fn_3_BD8D8 | |
| fn_3_BEFF8 | |
| fn_3_BE140 | |
| fn_3_BE174 | |
| fn_3_BF158 | |
| fn_3_BF1AC | |
| fn_3_BF20C | |
| fn_3_BF878 | |
| fn_3_C0770 | |
| fn_3_C07A0 | |
| fn_3_C07B0 | |

### TODO (ordered by size, attempt in this order)
| Function | Size | Notes |
|---|---|---|
| fn_3_BDE14 | 352 | stub |
| fn_3_BDCA4 | 368 | stub |
| fn_3_BC6D8 | 376 | stub |
| fn_3_BC888 | 408 | stub |
| fn_3_BD504 | 424 | has partial code, needs work |
| fn_3_BF6C0 | 440 | stub |
| fn_3_BDF74 | 460 | stub |
| fn_3_BF8F8 | 580 | stub |
| fn_3_BD1D8 | 604 | stub |
| fn_3_BFB3C | 616 | stub |
| fn_3_BBF94 | 656 | stub |
| fn_3_BA538 | 700 | stub |
| fn_3_BB15C | 760 | stub |
| fn_3_BFDA4 | 912 | stub |
| fn_3_BB454 | 928 | stub |
| fn_3_BD8FC | 936 | stub |
| fn_3_BB7F4 | 976 | stub |
| fn_3_BBBC4 | 976 | stub |
| fn_3_BC2DC | 1020 | stub |
| fn_3_BF238 | 1160 | stub |
| fn_3_C0134 | 1596 | stub |
| fn_3_BCA20 | 1972 | stub |
| fn_3_BA7F4 | 2184 | stub |
| fn_3_BE1D4 | 3620 | stub |
