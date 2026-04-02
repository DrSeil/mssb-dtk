# Working Progress

## Current Focus
**Source of truth:** Always follow `queue.log`. If this file disagrees with `queue.log`, `queue.log` wins.
**Strategy:** Work through functions listed in `queue.log` (plain entries at the bottom, after all COMPLETED/NONMATCHING lines)
**Queue file:** `queue.log`
**Next function to attempt:** fn_3_E1D00 (line 194 of queue.log)

To find the next function: look for the first plain entry (not prefixed with COMPLETED/NONMATCHING/SKIPPED) in `queue.log` that hasn't been committed yet.

---

## Recently Completed (from queue.log)
| Function | Notes |
|---|---|
| fn_3_8FC0C | matched |
| fn_3_6620 | already matched; queue entry updated |
| fn_3_5985C | matched |
| fn_3_1AE44 | already matched; queue entry updated |
| fn_3_59BCC | matched |
| fn_3_9E7D4 | matched |
| fn_3_B03F0 | matched |
| fn_3_D24E8 | matched |
| fn_3_DCED0 | matched |
| fn_3_F8B34 | matched |
| fn_3_112558 | matched |
| fn_3_167CC4 | matched (new gap_167CC4.c) |
| fn_3_76C78 | matched (new gap_759BC.c) |
| fn_3_E1C60 | matched |
| fn_3_E587C | matched |
| fn_3_DF820 | matched |

---

## Almost Matched (do not retry)
| Function | Issue |
|---|---|
| fn_3_BD6AC | bge+b+bl branch direction — CW codegen, unfixable |
| fn_3_BD80C | bss/transRot base register split (r5+r30 vs r30 only) |
| fn_3_BB07C | sin/cos scheduler: lfsu cascade, +1 instruction |
| fn_3_BF070 | stmw r27 vs 4 individual stw's |
| fn_3_BDE14 | register allocation: r25-r28+stmw vs r29-r31 individual stw |
| fn_3_BDCA4 | register allocation: CW assigns r6/r5 instead of r5/r4 → stfsx vs stfs |
| fn_3_107B9C | nonmatching |
| fn_3_107BD0 | nonmatching |
| fn_3_5A6A0 | nonmatching |
| fn_3_107C04 | nonmatching |
| fn_3_107D34 | nonmatching |
| fn_3_107988 | nonmatching |
| fn_3_EBFD4 | nonmatching |
| fn_3_107C40 | nonmatching |
| fn_3_107C88 | nonmatching |
| fn_3_8A5A4 | persistent register swap: `g_Runners` base in `r5` and `-1` in `r6`, target wants `r6`/`r5` |
| fn_3_7AEEC | CW running-pointer on bss struct (addi r6,r6,2 then +1) not reproducible from C struct field access |
| fn_3_7AF68 | same pattern as fn_3_7AEEC |
| fn_3_90220 | table-address shaping in `m_sound`: current code folds `lbl_3_data_8530 + arg1` too early and keeps `lhzx` result in `r0` instead of target `r3` |
| fn_3_99C88 | single remaining `rlwimi` form mismatch: current code emits equivalent `rlwimi r4, r0, 0, 0, 23` / `stw r4` instead of target `rlwimi r0, r4, 0, 24, 31` / `stw r0` |
| fn_3_FCE38 | branch-layout artifact in `rep_3090`: MWCC shortcuts the non-`bgt` path directly into the backward check instead of branching to the bottom `slwi` reset block first |
| fn_3_10C7A4 | register-allocation artifact in `gap_106DFC`: target wants `r29 = &g_Minigame` and `r28 = loop index`, current code keeps swapping base/index through an extra temp |
| fn_3_11741 | boolean materialization and register-allocation mismatch in `rep_3310`: current code stays on `or/not/rlwinm/extsh` with swapped base regs instead of target `subi/subfic/nor/srawi/andc` flow |
| fn_3_117494 | `rep_3310` setup/order mismatch: MWCC keeps `g_Minigame` live in `r3` too long, delays `r30 = &g_Minigame`, and lowers the `_CCC` guard through the wrong even/odd test shape |
| fn_3_E0758 | CW assigns rand-mod temp to r4 instead of r0, causing slwi/srwi register swap and preventing in-place rotlwi — unfixable from C |
| fn_3_E1964 | Same modulo register swap as E0758 + prologue counter/global init order mismatch (mr vs li) — CW scheduler artifacts |
| fn_3_E5924 | CW assigns loop counter to r5 instead of r4 in unrolled lbzu scan — volatile register swap |
| fn_3_6B5D8 | CW uses lbz r0 instead of destructive lbz r3 for lbl_80353A90 field load — volatile register swap |
