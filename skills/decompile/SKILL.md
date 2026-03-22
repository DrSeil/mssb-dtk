---
name: decompile
description: Full decompilation protocol for mssb-dtk. Covers function selection, initialization, iterative refinement, and commit. Invoke this at the start of any decompilation session.
---

# Decompile Protocol

You are now in decompilation mode. Follow these steps EXACTLY. Do not deviate, do not add extra research steps, do not narrate assembly analysis — just execute the protocol.

## STEP 1 — Find a function

```bash
python3 tools/score_functions.py --module game --file <file> --limit 10
```

- If no specific file: omit `--file`, pick from the full list
- Pick the **smallest** unmatched function
- Skip functions already in `config/skip_list.txt` (auto-filtered)

## STEP 2 — Initialize

```bash
python3 skills/decompile-init/scripts/initialize_function.py --slim <fn>
```

- Use `--slim` if the function has ≤ 60 instructions (saves ~40% tokens)
- Use without `--slim` for larger or complex functions with unknown callers/deps
- After running: write the m2c output directly into the source file
- Update the function signature in the header file
- **DO NOT** read other source files before the first build attempt
- **DO NOT** grep for patterns "just in case"
- Look up unknown symbols with MCP: max 3 calls, run in parallel

## STEP 3 — Build + Diff loop

```bash
ninja 2>&1 | tail -5
python3 tools/feedback_diff.py <fn>
```

Repeat up to 10 iterations. After each diff, apply ONE of these rules:

| Condition | Action |
|---|---|
| `MATCH!` | → STEP 5 |
| Build errors | Fix errors, repeat STEP 3 |
| Iteration ≥ 2 | Use `\| head -15` on diff output |
| Only register-name swaps, 3+ attempts | → STEP 4 (NONMATCHING) |
| `lfsu` with sin/cos or int-to-float, 2+ attempts | → STEP 4 (NONMATCHING) |
| `bge+b+bl` branch direction mismatch, 2+ attempts | → STEP 4 (NONMATCHING) |
| 10 iterations reached, no match | → STEP 4 (NONMATCHING) |
| Logic mismatch (wrong instruction, constant, offset) | Fix and repeat |

**Never** run the permuter for register-name-only mismatches.

## STEP 4 — NONMATCHING commit

```bash
git add src/game/<file>.c include/game/<file>.h   # and any other changed files
git commit -m "Almost matched <fn>

<one sentence: what the mismatch is and why it can't be fixed from C>"
```

Return to STEP 1 for the next function.

## STEP 5 — MATCH commit

```bash
ninja && python3 tools/feedback_diff.py <fn>   # must confirm MATCH!
git add src/game/<file>.c include/game/<file>.h
git commit -m "matched <fn>"
```

Return to STEP 1 for the next function.

---

## Quick-reference: known NONMATCHING patterns

- **Register swap only** (all mismatches are just fN→fM or rN→rM): commit after 3 attempts
- **`lfsu` + sin/cos + int-to-float**: CW scheduler pre-loads rodata_1ED0 before frsp, cascades to double-load of EC4. Unfixable. Commit after 2 attempts.
- **bge+b+bl branch pattern**: multi-early-return + final conditional call. CW branch direction is not controllable. Commit after 2 attempts.
- **`-requireprotos` redeclaration**: local redeclaration with different param count blocked. NONMATCHING immediately.
