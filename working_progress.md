# Working Progress

## Current Focus
**Strategy:** Work through functions listed in `queue.log` (plain entries at the bottom, after all COMPLETED/NONMATCHING lines)
**Queue file:** `queue.log`
**Next function to attempt:** fn_3_107CD0 (line 123 of queue.log)

To find the next function: look for the first plain entry (not prefixed with COMPLETED/NONMATCHING/SKIPPED) in queue.log that hasn't been committed yet.

---

## Recently Completed (from queue.log)
| Function | Notes |
|---|---|
| fn_3_59BCC | matched |
| fn_3_9E7D4 | matched |
| fn_3_B3B70 | matched |

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
