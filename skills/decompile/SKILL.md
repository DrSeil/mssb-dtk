---
name: decompile
description: Full decompilation protocol for mssb-dtk. Covers function selection, initialization, iterative refinement, and commit. Invoke this at the start of any decompilation session.
---

# Decompile Protocol

You are now in decompilation mode. Follow these steps EXACTLY. Do not deviate, do not add extra research steps, do not narrate assembly analysis — just execute the protocol.

## STEP 0 — Load current state

READ `working_progress.md` now. This tells you:
- Which file is the current focus
- Which functions are **Almost Matched** (do not retry — skip these)
- Which function to attempt next ("Next function to attempt")
- The full TODO list in priority order

After reading, state: "Current focus: `<file>`, next function: `<fn>`" — then proceed to STEP 1.

## STEP 1 — Confirm function selection

Use the "Next function to attempt" from `working_progress.md`.
If that function was just completed, pick the next TODO entry by size (smallest first).
Do not attempt any function listed under "Almost Matched".

To verify it's still unmatched:
```bash
python3 tools/score_functions.py --module game --file <basename> --limit 5
```

## STEP 2 — Initialize

```bash
python3 skills/decompile-init/scripts/initialize_function.py --slim <fn>
```

- Use `--slim` for functions ≤ 60 instructions (saves ~40% tokens)
- Use without `--slim` for larger or complex functions
- Write m2c output directly into the source file; update header signature
- **DO NOT** read other source files before the first build
- **DO NOT** grep for patterns "just in case"
- Look up unknown symbols with MCP: max 3 calls, run in parallel

## STEP 3 — Build + Diff loop

```bash
ninja 2>&1 | tail -5
python3 tools/feedback_diff.py <fn>
```

Repeat up to 10 iterations. After each diff, apply the **first matching rule**:

| Condition | Action |
|---|---|
| `MATCH!` | → STEP 5 |
| Build errors | Fix errors, repeat STEP 3 |
| Iteration ≥ 2 | Pipe diff through `\| head -15` |
| Register-name swaps only, 3+ attempts | → STEP 4 (NONMATCHING) |
| `lfsu` with sin/cos or int-to-float, 2+ attempts | → STEP 4 (NONMATCHING) |
| `bge+b+bl` branch direction mismatch, 2+ attempts | → STEP 4 (NONMATCHING) |
| 10 iterations reached, no match | → STEP 4 (NONMATCHING) |
| Logic mismatch (wrong opcode, constant, offset) | Fix and repeat |

Never run the permuter for register-name-only mismatches.

## STEP 4 — NONMATCHING commit + update progress

```bash
git add src/game/<file>.c include/game/<file>.h
git commit -m "Almost matched <fn>

<one sentence: what the mismatch is and why it can't be fixed from C>"
```

Then update `working_progress.md`:
- Move `<fn>` from **TODO** to **Almost Matched** with the reason
- Update "Next function to attempt" to the next TODO entry

```bash
git add working_progress.md
git commit -m "update working_progress: <fn> → almost matched"
```

Return to STEP 1.

## STEP 5 — MATCH commit + update progress

```bash
ninja && python3 tools/feedback_diff.py <fn>   # must show MATCH!
git add src/game/<file>.c include/game/<file>.h
git commit -m "matched <fn>"
```

Then update `working_progress.md`:
- Move `<fn>` from **TODO** to **Matched ✓**
- Update "Next function to attempt" to the next TODO entry

```bash
git add working_progress.md
git commit -m "update working_progress: matched <fn>"
```

Return to STEP 1.

---

## Quick-reference: known NONMATCHING patterns

| Pattern | Signal | Limit |
|---|---|---|
| Register swap only | All mismatches are fN→fM or rN→rM | 3 attempts |
| sin/cos + int-to-float | `lfsu` instead of `lfs` for a rodata constant | 2 attempts |
| Multi-return + conditional call | `bge+b+bl` branch direction wrong | 2 attempts |
| `-requireprotos` redeclaration | Compiler error on local redecl | NONMATCHING immediately |
