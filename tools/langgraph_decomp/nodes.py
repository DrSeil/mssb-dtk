"""Graph node functions for the decompilation loop.

Each node takes DecompState and returns a partial state update dict.
"""

import os
import sys
import subprocess
import json
import re
import tree_sitter_c as tsc
from tree_sitter import Language, Parser, Query, QueryCursor

# Add tools directory to path for imports
_script_dir = os.path.dirname(os.path.abspath(__file__))
_tools_dir = os.path.dirname(_script_dir)
_root_dir = os.path.dirname(_tools_dir)
sys.path.insert(0, _tools_dir)

import decomp_helper
import gen_prompt
import feedback_diff
from . import struct_utils
from . import masking
from .state import merge_unique_symbols


# ---------------------------------------------------------------------------
# Node A: Source Finder
# ---------------------------------------------------------------------------

def source_finder_node(state):
    """Resolve function name → module, source file, ASM, unit, header, Ghidra."""
    func_name = state["function_name"]
    print(f"[source_finder] Resolving {func_name}...")

    # Phase 0.4: Isolated Branching
    original_branch = "main"
    try:
        res = subprocess.run(["git", "branch", "--show-current"], capture_output=True, text=True, cwd=_root_dir)
        original_branch = res.stdout.strip()
        
        # Create and switch to temporary branch
        base_branch = f"decomp/{func_name}"
        tmp_branch = base_branch
        
        # Check if the base branch already exists
        res = subprocess.run(["git", "branch", "--list", tmp_branch], capture_output=True, text=True, cwd=_root_dir)
        if res.stdout.strip():
            # If it exists, find the next available attempt number
            i = 1
            while True:
                tmp_branch = f"{base_branch}_{i}"
                res = subprocess.run(["git", "branch", "--list", tmp_branch], capture_output=True, text=True, cwd=_root_dir)
                if not res.stdout.strip():
                    break
                i += 1
            print(f"[source_finder] Branch {base_branch} already exists, creating attempt {i}: {tmp_branch}")
        else:
            print(f"[source_finder] Creating branch {tmp_branch} (from {original_branch})...")
            
        # Create and switch to temporary branch
        subprocess.run(["git", "checkout", "-b", tmp_branch], capture_output=True, cwd=_root_dir)
        # If it failed to create (race condition?), try checking it out
        subprocess.run(["git", "checkout", tmp_branch], capture_output=True, cwd=_root_dir)
    except Exception as e:
        print(f"[source_finder] Git branching failed: {e}")

    # Locate function in symbols
    info = gen_prompt.locate_function(func_name)
    if not info:
        return {
            "status": "error",
            "feedback": f"Could not locate {func_name} in any symbols file.",
        }

    module = info["module"]
    source_file = info["file"]
    addr = info["addr"]

    # Extract assembly
    asm_text = gen_prompt.get_assembly(func_name, info)
    if not asm_text:
        return {
            "status": "error",
            "feedback": f"Could not extract assembly for {func_name}.",
        }

    # Find the objdiff unit name
    unit_name = feedback_diff.find_unit_for_symbol(func_name)
    if not unit_name:
        return {
            "status": "error",
            "feedback": f"Could not find objdiff unit for {func_name}.",
        }

    # Read header file
    header_path, header_content = gen_prompt.read_header_file(info)

    # Get Ghidra decompilation
    ghidra_output = gen_prompt.run_ghidra_decomp(
        func_name, addr=addr, module=module
    )

    # Read existing source file content
    src_path, src_content = gen_prompt.read_source_file(info)

    # Detect stub
    is_stub = False
    if src_content:
        # Match INCLUDE_ASM or an empty/stub function body
        stub_patterns = [
            rf"INCLUDE_ASM\(.*{func_name}.*\)",
            rf"/[/*]\s*STUB\s*[*/]/",
            rf"/{re.escape(func_name)}\b\s*\([^)]*\)\s*\{{\s*[/*\s]*STUB[*/\s]*\}}",
            rf"{re.escape(func_name)}\b\s*\([^)]*\)\s*\{{\s*\}}" # Empty body
        ]
        for pattern in stub_patterns:
            if re.search(pattern, src_content, re.DOTALL):
                is_stub = True
                break
    else:
        # No source content means we need to bootstrap anyway
        is_stub = True

    # Find the ASM path for reference
    rel_path = os.path.splitext(source_file)[0] + ".s"
    if module:
        asm_path = os.path.join(_root_dir, f"build/GYQE01/{module}/asm", rel_path)
    else:
        asm_path = os.path.join(_root_dir, "build/GYQE01/asm", rel_path)

    print(f"[source_finder] Found: module={module}, unit={unit_name}, "
          f"source={source_file}, is_stub={is_stub}, asm_lines={len(asm_text.splitlines())}")

    # Phase 1.1: SDA Mapping
    sda_info = ""
    symbols_txt = os.path.join(_root_dir, "config/GYQE01", "symbols.txt")
    if module:
        module_symbols = os.path.join(_root_dir, "config/GYQE01", module, "symbols.txt")
        if os.path.exists(module_symbols):
            symbols_txt = module_symbols
            
    if os.path.exists(symbols_txt):
        print(f"[source_finder] Parsing SDA symbols from {symbols_txt}...")
        all_syms = struct_utils.parse_symbols_txt(symbols_txt)
        mapping = struct_utils.get_sda_mapping(all_syms)
        
        sda_lines = ["SDA Mapping (Relative to base registers):"]
        if mapping['r13']:
            sda_lines.append("r13 (SDA):")
            for s in mapping['r13'][:50]: # Cap to 50 symbols for token efficiency
                sda_lines.append(f"  {s['name']} (type:{s['type']}) = r13 + {hex(s['sda_offset'])}")
        if mapping['r2']:
            sda_lines.append("r2 (SDA2):")
            for s in mapping['r2'][:50]:
                sda_lines.append(f"  {s['name']} (type:{s['type']}) = r2 + {hex(s['sda_offset'])}")
        
        if len(sda_lines) > 1:
            sda_info = "\n".join(sda_lines)

    # Phase 1.2: Symbol context (size/type/array hints) and Ghidra annotation
    _, raw_syms = gen_prompt.extract_deps_and_symbols(asm_text)
    symbol_context = gen_prompt.annotate_referenced_symbols(raw_syms, asm_text, module=None)
    annotated_ghidra = gen_prompt.annotate_ghidra_addresses(ghidra_output or "", module=None)

    return {
        "module": module,
        "source_file": source_file,
        "source_path": src_path or "",
        "asm_path": asm_path,
        "asm_text": asm_text,
        "unit_name": unit_name,
        "header_path": header_path or "",
        "header_content": header_content or "",
        "ghidra_output": annotated_ghidra,
        "symbol_context": symbol_context,
        "is_stub": is_stub,
        "sda_map": sda_info,
        "original_branch": original_branch,
        "status": "running",
    }


# ---------------------------------------------------------------------------
# Node B: Decompiler (M2C)
# ---------------------------------------------------------------------------

def decompiler_node(state):
    """Run M2C to generate the initial C skeleton if starting from a stub."""
    func_name = state["function_name"]
    is_stub = state.get("is_stub", False)
    
    # If not a stub and we have code already, don't overwrite it with M2C
    if not is_stub and state.get("current_c_code"):
        print(f"[decompiler] Skipping M2C bootstrap: function is not a stub.")
        return {
            "iterations": 0,
            "attempts": [],
            "messages": [("ai", f"Skipped M2C bootstrap for {func_name} (already has implementation).")],
        }

    print(f"[decompiler] Running M2C for {func_name}...")

    # Use gen_prompt.run_m2c which wraps m2c_helper.py
    m2c_output = gen_prompt.run_m2c(func_name)

    if not m2c_output:
        print(f"[decompiler] M2C failed, using empty skeleton")
        m2c_output = f"// M2C could not decompile {func_name}\n"

    # Remove any ? placeholders from M2C output early to avoid build errors.
    # M2C headers with '?' are almost always garbage that conflicts with real types.
    m2c_clean = []
    for line in m2c_output.splitlines():
        if '?' in line:
            # If it's a function prototype guess, try to fix it to void
            if '(' in line and ')' in line:
                line = re.sub(r'\?\s+([a-zA-Z_0-9]+)\(', r'void \1(', line)
            
            # If it still has a '?', and it's not in a comment, skip it
            if '?' in re.sub(r'/\*.*?\*/', '', line):
                print(f"[decompiler] Scrubbing garbage line: {line.strip()}")
                continue
        m2c_clean.append(line)
    m2c_refined = "\n".join(m2c_clean)
    m2c_refined = _postprocess_m2c_output(m2c_refined)

    split = _split_m2c_draft(m2c_refined, func_name)

    current_c_code = split.get("body", m2c_refined)
    externs = split.get("externs", "")
    local_headers = split.get("headers", "")

    print(f"[decompiler] M2C output: {len(m2c_refined)} chars, "
          f"body: {len(current_c_code)} chars")

    return {
        "m2c_output": m2c_refined,
        "current_c_code": current_c_code,
        "externs": externs,
        "local_headers": local_headers,
        "iterations": 0,
        "attempts": [],
        "messages": [("ai", f"Generated initial M2C decompilation for {func_name}.")],
    }


def _postprocess_m2c_output(c_code: str) -> str:
    """Convert m2c pointer-arithmetic patterns to proper C array indexing.

    m2c emits *(&sym + idx) for indexed global array accesses.  This notation
    is forbidden by project style (no pointer arithmetic) AND often triggers
    Metrowerks "illegal type" errors.  Convert it to sym[idx] so the LLM
    never sees the bad pattern in the first place.
    """
    # *(&sym + idx)  →  sym[idx]
    c_code = re.sub(r'\*\(&(\w+)\s*\+\s*([^)]+?)\)', r'\1[\2]', c_code)
    return c_code


def _split_m2c_draft(c_code, func_name):
    """Split M2C output into externs, headers, and body."""
    externs = []
    unit_headers = []
    function_body = []

    lines = c_code.splitlines()
    in_function = False

    for line in lines:
        stripped = line.strip()
        if stripped.startswith('#'):
            continue

        if not stripped:
            if in_function:
                function_body.append(line)
            continue

        # Detect function start
        if func_name in line and '{' in line and '(' in line:
            in_function = True

        if in_function:
            function_body.append(line)
        else:
            if stripped.startswith('?') or '/* extern */' in line:
                externs.append(line)
            elif (stripped.startswith('extern') or stripped.startswith('static')
                  or stripped.startswith('typedef')):
                unit_headers.append(line)
            else:
                if stripped.endswith(';') and '(' in stripped:
                    unit_headers.append(line)
                else:
                    externs.append(line)

    return {
        "externs": "\n".join(externs),
        "headers": "\n".join(unit_headers),
        "body": "\n".join(function_body),
    }


# ---------------------------------------------------------------------------
# Node C: Refactorer (LLM)
# ---------------------------------------------------------------------------

def refactorer_node(state, escalate_after=5):
    """Send context + history to the LLM, get improved C code back."""
    from .llm import invoke_refactor
    
    # Clear struct updates from previous iterations to prevent state bleed
    state['struct_updates'] = []

    func_name = state["function_name"]
    iteration = state.get("iterations", 0)
    escalate = state.get("escalate", False)
    prefer_local = state.get("prefer_local", False)
    print(f"[refactorer] Iteration {iteration + 1} for {func_name} (escalate={escalate}, prefer_local={prefer_local})...")

    prompt = ""
    try:
        llm_result, tier, prompt, raw_response = invoke_refactor(
            state, escalate_after=escalate_after, escalate=escalate, prefer_local=prefer_local
        )
    except Exception as e:
        print(f"[refactorer] LLM error: {e}")
        # Try to at least get the prompt if possible
        try:
            from .llm import build_refactor_prompt
            prompt = build_refactor_prompt(state)
        except:
            pass
        return {
            "status": "error",
            "feedback": f"LLM invocation failed: {e}",
            "messages": [("ai", f"LLM error on iteration {iteration + 1}: {e}")],
            "last_prompt": prompt,
        }

    # llm_result is a dict with 'function', 'updates', 'header', 'externs' keys
    func_code = llm_result.get("function", "")
    struct_updates = llm_result.get("updates", [])
    header_adds = llm_result.get("header", "")
    extern_adds = llm_result.get("externs", "")

    # Post-process: strip #include directives from function code
    func_lines = []
    for line in func_code.splitlines():
        if line.strip().startswith("#include"):
            continue
        func_lines.append(line)
    func_code = "\n".join(func_lines).strip()

    header_lines = []
    for line in header_adds.splitlines():
        stripped = line.strip()
        if stripped.startswith("#ifndef") or stripped.startswith("#define __") or stripped.startswith("#endif"):
            continue
        if stripped.startswith("#include"):
            continue
        if '?' in line:
            print(f"[refactorer] Filtering out invalid header line: {line}")
            continue
        header_lines.append(line)
    header_adds = "\n".join(header_lines).strip()

    extern_lines = []
    for line in extern_adds.splitlines():
        if '?' in line:
            print(f"[refactorer] Filtering out invalid extern line: {line}")
            continue
        extern_lines.append(line)
    extern_adds = "\n".join(extern_lines).strip()

    # If there are struct updates, the state["struct_updates"] is now a list of mod actions.
    # We should merge them or just pass them along to the builder.
    
    print(f"[refactorer] Got {len(func_code)} chars function, "
          f"{len(struct_updates)} struct modifications, "
          f"{len(header_adds)} chars header, {len(extern_adds)} chars externs from {tier} LLM")
    print(f"[refactorer] === LLM FUNCTION ===")
    print(func_code)
    print(f"[refactorer] === END LLM FUNCTION ===")
    
    if len(func_code) < 10:
        print("[refactorer] WARNING: LLM returned empty or trivial code.")
        return {
            "status": "error",
            "feedback": "LLM returned empty or trivial code.",
            "messages": [("ai", f"Iteration {iteration + 1}: LLM returned empty code.")],
            "last_prompt": prompt,
            "last_raw_response": raw_response,
        }
    if struct_updates:
        print(f"[refactorer] === LLM STRUCT UPDATES ({len(struct_updates)}) ===")
        for upd in struct_updates:
            print(f"  Type: {upd.get('type_name')}")
            for action in upd.get('actions', []):
                print(f"    - {json.dumps(action)}")
        print(f"[refactorer] === END STRUCT UPDATES ===")
    if header_adds:
        print(f"[refactorer] === LLM HEADER ADDITIONS ===")
        print(header_adds)
        print(f"[refactorer] === END HEADER ===")
    if extern_adds:
        print(f"[refactorer] === LLM EXTERN ADDITIONS ===")
        print(extern_adds)
        print(f"[refactorer] === END EXTERNS ===")

    return {
        "current_c_code": func_code,
        "llm_tier": tier,
        "local_headers": header_adds,
        "externs": extern_adds,
        "struct_updates": struct_updates,
        "messages": [("ai",
            f"Iteration {iteration + 1}: Refactored using {tier} LLM "
            f"(func={len(func_code)}, updates={len(struct_updates)}, "
            f"header={len(header_adds)}, externs={len(extern_adds)} chars)."
        )],
        "explanation": llm_result.get("explanation", ""),
        "last_prompt": prompt,
        "last_raw_response": raw_response,
    }


# ---------------------------------------------------------------------------
# Node D: Builder (Verification)
# ---------------------------------------------------------------------------

def builder_node(state):
    """Write code, build with ninja, score with objdiff, collect feedback."""
    func_name = state["function_name"]
    unit_name = state["unit_name"]
    iteration = state.get("iterations", 0)
    current_code = state.get("current_c_code", "")
    print(f"[builder] Build + score iteration {iteration + 1} for {func_name}...")
    
    if not current_code or len(current_code.strip()) < 10:
        _log = print
        _log("[builder] Skipping build: current_c_code is empty or too short.")
        return {
            "status": "error",
            "feedback": "LLM returned empty or incomplete code.",
            "build_log": "Empty code - build skipped.",
            "match_percent": 0.0,
            "iterations": iteration + 1,
            "attempts": state.get("attempts", []) + [{
                "c_code": current_code,
                "match_percent": 0.0,
                "feedback": "Empty code",
                "build_error": "Empty code",
            }],
            "status": "running",
            "messages": [("ai", f"Iteration {iteration + 1}: LLM returned empty code.")],
        }

    # Use the DecompOrchestrator from aidecomp for build-and-score
    # We import it here to avoid circular deps at module load
    sys.path.insert(0, os.path.join(_tools_dir, "aidecomp"))

    try:
        result = _run_build_and_score(
            func_name, unit_name, current_code,
            state.get("externs", ""),
            state.get("local_headers", ""),
            state,
        )
        current_asm = result.get("current_asm", "")
    except Exception as e:
        print(f"[builder] Build exception: {e}")
        attempt = {
            "c_code": current_code,
            "match_percent": 0.0,
            "feedback": "",
            "current_asm": "",
            "build_error": str(e),
        }

        _write_attempt_log(state, attempt, iteration + 1)

        return {
            "feedback": str(e),
            "build_log": str(e),
            "match_percent": 0.0,
            "iterations": iteration + 1,
            "attempts": state.get("attempts", []) + [attempt],
            "status": "running",
            "messages": [("ai", f"Build failed on iteration {iteration + 1}: {e}")],
        }

    status_code = result.get("status", "error")
    match_percent = result.get("score", 0.0) * 100.0  # score is 0-1, we want 0-100
    mismatch_count = result.get("mismatch_count", 0)
    mismatch_summary = result.get("mismatch_summary", "")
    feedback_text = result.get("asm_diff", "")
    build_log = result.get("log", "")

    # Progress tracking & Escalation detection
    prev_match = state.get("match_percent", 0.0)
    prev_mismatch = state.get("mismatch_count", 0)
    escalate = state.get("escalate", False)

    if status_code == "success":
        # Check for stagnation or regression
        # If mismatch count didn't decrease, or match percent didn't increase
        if iteration > 0: # Only after the first real attempt
            if mismatch_count >= prev_mismatch and match_percent <= prev_match:
                print(f"[builder] Progress stagnant ({prev_mismatch} -> {mismatch_count} mismatches).")
                # If stagnant for 2 consecutive turns (or just 1 for early escalation)
                # For now, let's escalate immediately on first stagnation to be proactive
                escalate = True
            elif mismatch_count > prev_mismatch:
                print(f"[builder] REGRESSION detected ({prev_mismatch} -> {mismatch_count} mismatches)!")
                escalate = True

    # Determine build error vs diff feedback
    build_error = ""
    if status_code in ("build_error", "build_timeout"):
        build_error = build_log
        feedback_text = ""
        match_percent = 0.0
        mismatch_count = prev_mismatch # Carry over last known count
    elif status_code == "error":
        build_error = build_log or result.get("message", "Unknown error")

    # Build the attempt record
    attempt = {
        "c_code": current_code,
        "match_percent": match_percent,
        "mismatch_count": mismatch_count,
        "mismatch_summary": mismatch_summary,
        "explanation": state.get("explanation", ""),
        "feedback": feedback_text, # Keep full diff for logs
        "current_asm": current_asm,
        "build_error": build_error,
        "struct_updates": state.get("struct_updates", []),
        "header_additions": state.get("local_headers", ""),
        "extern_declarations": state.get("externs", ""),
        "process_log": result.get("process_log", []),
    }

    new_status = "running"
    if match_percent >= 100.0:
        new_status = "matched"
    elif status_code == "error":
        new_status = "error"

    print(f"[builder] Result: {match_percent:.1f}% match ({mismatch_count} mismatches), status={new_status}, escalate={escalate}")

    res = {
        "feedback": mismatch_summary or feedback_text, # Use efficient summary for LLM
        "build_log": build_error,
        "match_percent": match_percent,
        "mismatch_count": mismatch_count,
        "prev_match_percent": prev_match,
        "prev_mismatch_count": prev_mismatch,
        "escalate": escalate,
        "current_asm": current_asm,
        "iterations": iteration + 1,
        "attempts": state.get("attempts", []) + [attempt],
        "status": new_status,
        "messages": [("ai",
            f"Iteration {iteration + 1}: {match_percent:.1f}% match ({mismatch_count} mismatches). "
            f"Status: {new_status}. Escalate: {escalate}"
        )],
    }

    # Merge reactive fixes back into state so they persist
    fixes = result.get("fixes", [])
    if fixes:
        new_local_headers = state.get("local_headers", "")
        for fix in fixes:
            if fix["type"] == "header_rectification":
                # If it's the main header for this unit, add it to local_headers
                if fix["file"] == state.get("header_path") or os.path.basename(fix["file"]) == os.path.basename(state.get("header_path", "")):
                    new_local_headers = merge_unique_symbols(new_local_headers, fix["content"])
            elif fix["type"] == "missing_include":
                # Missing includes in source go to local_headers too (M2C logic)
                new_local_headers = merge_unique_symbols(new_local_headers, fix["content"])
        res["local_headers"] = new_local_headers

    _write_attempt_log(state, attempt, iteration + 1)

    return res


def _write_attempt_log(state, attempt, iteration):
    """Write all data for this attempt to a log file."""
    func_name = state["function_name"]
    log_dir = os.path.join(_root_dir, "logs", "decomp", func_name)
    os.makedirs(log_dir, exist_ok=True)

    log_file = os.path.join(log_dir, f"attempt_{iteration}.log")

    content = []
    content.append(f"METADATA")
    content.append(f"========\n")
    content.append(f"Function:    {func_name}")
    content.append(f"Iteration:   {iteration}")
    content.append(f"LLM Tier:    {state.get('llm_tier', 'unknown')}")
    content.append(f"Match Pct:   {attempt.get('match_percent', 0.0):.1f}%")
    content.append(f"Status:      {state.get('status', 'running')}\n")

    content.append(f"\nPROMPT")
    content.append(f"======\n")
    content.append(state.get("last_prompt", "No prompt found"))

    content.append(f"\nEXPLANATION")
    content.append(f"===========\n")
    content.append(state.get("explanation", "No explanation provided"))

    content.append(f"\nRAW RESPONSE")
    content.append(f"============\n")
    content.append(state.get("last_raw_response", "No raw response found"))

    content.append(f"\nPARSED RESULT")
    content.append(f"=============\n")
    content.append(f"--- C CODE ---\n")
    content.append(attempt.get("c_code", ""))
    content.append(f"\n--- HEADER ADDS ---\n")
    content.append(state.get("local_headers", ""))
    content.append(f"\n--- EXTERN ADDS ---\n")
    content.append(state.get("externs", ""))
    content.append(f"\n--- STRUCT UPDATES ---\n")
    updates = state.get("struct_updates", [])
    for upd in updates:
        content.append(f"Type: {upd.get('type_name')}")
        for action in upd.get('actions', []):
            content.append(f"  - {json.dumps(action)}")

    content.append(f"\n\nBUILD LOG")
    content.append(f"=========\n")
    content.append(attempt.get("build_error", "No build errors"))

    content.append(f"\nOBJDIFF FEEDBACK")
    content.append(f"================\n")
    content.append(attempt.get("feedback", "No diff feedback (matched or build error)"))

    content.append(f"\nBUILDER PROCESS LOG")
    content.append(f"===================\n")
    for log_line in attempt.get("process_log", []):
        content.append(log_line)

    with open(log_file, "w", encoding="utf-8") as f:
        f.write("\n".join(content))
    print(f"[builder] Logged attempt to {log_file}")


def _filter_build_output(output: str) -> str:
    """Strip ninja progress noise from build output, keeping only error-relevant lines."""
    filtered = []
    for line in output.splitlines():
        # Skip [N/M] MWCC progress lines
        if re.match(r'^\[\d+/\d+\]\s+MWCC\b', line):
            continue
        # Skip the long wibo/mwcceppc compiler command line
        if re.match(r'^build/tools/wibo\b', line):
            continue
        # For FAILED: lines, keep just the target filename, not the command
        if line.startswith('FAILED:'):
            parts = line.split()
            filtered.append(f"FAILED: {parts[1]}" if len(parts) > 1 else line)
            continue
        filtered.append(line)
    return '\n'.join(filtered)


def _attempt_auto_fix(compiler_output: str, source_path: str, backups: dict, _log=print, fixes_acc=None) -> bool:
    """Attempt to parse compiler errors and automatically fix the source file.
    
    If fixes_acc is provided (a list), it will be populated with dicts describing
    the fixes made (e.g. {'type': 'header_rectification', 'file': '...', 'content': '...'}).
    """
    if fixes_acc is None:
        fixes_acc = []
        
    try:
        import re
        from tools.langgraph_decomp import struct_utils
        from tools.langgraph_decomp.llm import _extract_struct_block
        
        _log("[builder] Checking if auto-fix is possible for compiler errors...")
        
        fixed_something = False
        
        with open(source_path, "r") as f:
            src_content = f.read()

        lines = compiler_output.splitlines()
        for i, line in enumerate(lines):
            if line.startswith("#   Error:"):
                msg_line = lines[i+1] if i+1 < len(lines) else ""
                code_line = lines[i-1] if i > 0 else ""
                
                # Case 1: 'unk[Hex]' is not a struct/union/class member
                msg_match = re.match(r"#\s+'([^']+)' is not a struct/union/class member", msg_line)
                if msg_match:
                    field = msg_match.group(1)
                    if field.startswith("unk"):
                        offset_hex = field[3:]
                        try:
                            offset = int(offset_hex, 16)
                        except ValueError:
                            continue
                        
                        code_text = code_line.split(":", 1)[1].strip() if ":" in code_line else code_line
                        var_match = re.search(r"(\w+)\s*(?:\.|->)\s*" + re.escape(field), code_text)
                        if var_match:
                            var_name = var_match.group(1)
                            _log(f"[builder] Auto-fix matching variable: {var_name}")
                            
                            # 1. find type of variable
                            type_pattern = rf'\b\w+\s*\*?\s+{re.escape(var_name)}\b.*?;'
                            include_dir = os.path.join(_root_dir, "include")
                            src_dir = os.path.join(_root_dir, "src")
                            type_str = None
                            try:
                                result = subprocess.run(
                                    ["grep", "-rE", type_pattern, include_dir, src_dir],
                                    capture_output=True, text=True, timeout=5
                                )
                                for hit in result.stdout.splitlines():
                                    if "extern " in hit or "struct " in hit or "typedef " in hit:
                                        type_match = re.search(rf'(?:extern\s+)?([\w\s\*]+?)\s+{re.escape(var_name)}\b', hit.split(':', 1)[-1])
                                        if type_match:
                                            t = type_match.group(1).strip()
                                            if t not in ('struct', 'typedef'):
                                                type_str = t.replace('const', '').replace('volatile', '').replace('*', '').strip()
                                                break
                            except Exception:
                                pass
                            
                            if type_str:
                                # 2. find struct field mapping
                                struct_pattern = rf'(?:typedef\s+)?(?:struct|enum|union)\s+_?{re.escape(type_str)}\b'
                                try:
                                    result = subprocess.run(
                                        ["grep", "-rln", "-P", "--include=*.h", struct_pattern, include_dir],
                                        capture_output=True, text=True, timeout=5
                                    )
                                    if result.stdout:
                                        s_filepath = result.stdout.splitlines()[0].strip()
                                        block = _extract_struct_block(s_filepath, type_str)
                                        if block:
                                            fields = struct_utils.parse_struct_fields(block)
                                            found_field_name = None
                                            for fld in fields:
                                                if fld.offset <= offset < fld.offset + fld.size:
                                                    if not fld.is_padding:
                                                        found_field_name = fld.name
                                                        break
                                            
                                            if found_field_name:
                                                _log(f"[builder] Auto-fixing {field} -> {found_field_name} (struct {type_str})")
                                                if source_path not in backups:
                                                    backups[source_path] = src_content
                                                # regex replace whole word only for var_name.field
                                                src_content = re.sub(rf'\b{re.escape(var_name)}\s*\.\s*{re.escape(field)}\b', f'{var_name}.{found_field_name}', src_content)
                                                src_content = re.sub(rf'\b{re.escape(var_name)}\s*->\s*{re.escape(field)}\b', f'{var_name}->{found_field_name}', src_content)
                                                fixed_something = True
                                except Exception:
                                    pass

                # Case 2: undefined identifier 'X'
                undef_match = re.match(r"#\s+undefined identifier '([^']+)'", msg_line)
                if undef_match:
                    ident = undef_match.group(1)
                    _log(f"[builder] Auto-fix searching for undefined identifier: {ident}")
                    include_dir = os.path.join(_root_dir, "include")
                    try:
                        # Match extern <type> ident, or #define ident, or struct ident
                        decl_pattern = rf'(?:extern|#define|struct|typedef|enum)\s+(?:[\w\s\*\[\]]+)?\b{re.escape(ident)}\b'
                        result = subprocess.run(
                            ["grep", "-rln", "-P", "--include=*.h", decl_pattern, include_dir],
                            capture_output=True, text=True, timeout=5
                        )
                        if result.stdout:
                            header_path = result.stdout.splitlines()[0].strip()
                            rel_inc = os.path.relpath(header_path, include_dir)
                            inc_line = f'#include "{rel_inc}"\n'
                            if inc_line.strip() not in src_content:
                                _log(f"[builder] Auto-fixing missing include -> {inc_line.strip()}")
                                fixes_acc.append({"type": "missing_include", "file": source_path, "content": inc_line.strip()})
                                if source_path not in backups:
                                    backups[source_path] = src_content
                                
                                # insert after first #include or at top
                                inc_match = re.search(r'^#include.*?\n', src_content, re.MULTILINE)
                                if inc_match:
                                    # Insert after the last import instead? Or just after the first match.
                                    src_content = src_content[:inc_match.end()] + inc_line + src_content[inc_match.end():]
                                else:
                                    src_content = inc_line + src_content
                                fixed_something = True
                    except Exception:
                        pass

                # Case 3: identifier 'X' redeclared
                redecl_match = re.match(r"#\s+identifier '([^']+)\(\)' redeclared", msg_line)
                if not redecl_match:
                     redecl_match = re.match(r"#\s+identifier '([^']+)' redeclared", msg_line)
                
                if redecl_match:
                    ident = redecl_match.group(1).strip()
                    # Strip arguments from identifier if present
                    if '(' in ident:
                        ident = ident.split('(')[0].strip()
                    _log(f"[builder] Auto-fix reacting to redeclaration of: {ident}")
                    
                    # Try to find the new declaration in the compiler output
                    # Sometimes the compiler prints "now declared as: '...'"
                    # But the builder node already knows the 'headers' it just tried to add.
                    # Actually, the builder_node logic HAS the state.
                    
                    # We need to find WHERE it was redeclared. The compiler output has:
                    # #      In: include\game\rep_1D58.h
                    file_context = ""
                    # We need to find WHERE it was redeclared. The compiler output has:
                    # #      In: include\game\rep_1D58.h
                    # #    File: src\game\rep_1D58.c
                    file_context = ""
                    for j in range(i, i-20, -1):
                        if j >= 0:
                            if "#      In:" in lines[j]:
                                file_context = lines[j].split("In:")[1].strip()
                                break
                            if "#    File:" in lines[j]:
                                file_context = lines[j].split("File:")[1].strip()
                                break
                    
                    header_site = None
                    if file_context and file_context.endswith(".h"):
                        header_site = os.path.join(_root_dir, file_context) if not os.path.isabs(file_context) else file_context
                    
                    if not header_site:
                        # Fallback: grep for it in include/
                        _log(f"[builder] Fallback searching for {ident} in include/")
                        include_dir = os.path.join(_root_dir, "include")
                        try:
                            # Match Type Ident(args);
                            redecl_pattern = rf'(?m)^([\w\s\*]+?)\s+{re.escape(ident)}\s*\(.*?\)\s*;?'
                            res = subprocess.run(
                                ["grep", "-rln", "-P", "--include=*.h", redecl_pattern, include_dir],
                                capture_output=True, text=True, timeout=5
                            )
                            if res.stdout:
                                header_site = res.stdout.splitlines()[0].strip()
                                _log(f"[builder] Found {ident} in {os.path.basename(header_site)}")
                        except Exception:
                            pass

                    if header_site and os.path.exists(header_site):
                        # Find the NEW declaration we just tried to add
                        with open(source_path, "r") as f:
                            current_src = f.read()
                        
                        # Find the declaration in the source (current_src)
                        # Regex for: void ident(args) {
                        src_decl_match = re.search(rf'(?m)^([\w\s\*]+?)\s+{re.escape(ident)}\s*\((.*?)\)\s*\{{', current_src)
                        if src_decl_match:
                            new_decl = f"{src_decl_match.group(1)} {ident}({src_decl_match.group(2)});"
                            
                            if header_site not in backups:
                                with open(header_site, "r") as f:
                                    backups[header_site] = f.read()
                            
                            h_content = backups[header_site]
                            
                            # Find existing in header
                            h_decl_pattern = rf'(?m)^([\w\s\*]+?)\s+{re.escape(ident)}\s*\(.*?\)\s*;?'
                            h_match = re.search(h_decl_pattern, h_content)
                            if h_match:
                                # Check if the return types are different
                                header_ret_type = h_match.group(1).strip()
                                src_ret_type = src_decl_match.group(1).strip()
                                if header_ret_type != src_ret_type:
                                    _log(f"[builder] Auto-fixing return type mismatch for {ident}: header has '{header_ret_type}', source has '{src_ret_type}'. Updating source.")
                                    # We need to replace the return type in the source file
                                    src_content = re.sub(rf'(?m)^{re.escape(src_ret_type)}\s+{re.escape(ident)}\s*\((.*?)\)\s*\{{', f"{header_ret_type} {ident}({src_decl_match.group(2)}) {{", src_content)
                                    fixed_something = True
                                else:
                                    _log(f"[builder] Rectifying header {os.path.basename(header_site)}: {h_match.group(0).strip()} -> {new_decl}")
                                    fixes_acc.append({"type": "header_rectification", "file": header_site, "content": new_decl})
                                    h_content = h_content[:h_match.start()] + new_decl + h_content[h_match.end():]
                                    with open(header_site, "w") as f:
                                        f.write(h_content)
                                    fixed_something = True


                # Case 4: function has no prototype
                no_proto_match = re.match(r"#\s+function has no prototype", msg_line)
                if no_proto_match:
                    code_text = code_line.split(":", 1)[1].strip() if ":" in code_line else code_line
                    func_match = re.search(r'(\w+)\s*\(', code_text)
                    if func_match:
                        func_name = func_match.group(1)
                        _log(f"[builder] Auto-fix searching for prototype of: {func_name}")
                        include_dir = os.path.join(_root_dir, "include")
                        try:
                            decl_pattern = rf'\b{re.escape(func_name)}\s*\('
                            result = subprocess.run(
                                ["grep", "-rln", "-P", "--include=*.h", decl_pattern, include_dir],
                                capture_output=True, text=True, timeout=5
                            )
                            if result.stdout:
                                header_path = result.stdout.splitlines()[0].strip()
                                rel_inc = os.path.relpath(header_path, include_dir)
                                inc_line = f'#include "{rel_inc}"\n'
                                if inc_line.strip() not in src_content:
                                    _log(f"[builder] Auto-fixing missing prototype include -> {inc_line.strip()}")
                                    fixes_acc.append({"type": "missing_include", "file": source_path, "content": inc_line.strip()})
                                    if source_path not in backups:
                                        backups[source_path] = src_content
                                    inc_match = re.search(r'^#include.*?\n', src_content, re.MULTILINE)
                                    if inc_match:
                                        src_content = src_content[:inc_match.end()] + inc_line + src_content[inc_match.end():]
                                    else:
                                        src_content = inc_line + src_content
                                    fixed_something = True
                        except Exception:
                            pass

                # Case 5: illegal return value in void function
                # The LLM changed the return type in the source but forgot to update the header.
                # Fix: find the function definition in the source, grep the include dir for its
                # void declaration, and replace the header prototype with the non-void one.
                void_ret_match = re.match(r"#\s+illegal return value in void", msg_line)
                if void_ret_match:
                    with open(source_path, "r") as f:
                        current_src = f.read()
                    # Find the first non-void function definition in the source
                    fn_def = re.search(
                        r'(?m)^((?!void\b)[\w][\w\s\*]*?)\s+(fn_\w+|[a-z]\w+)\s*\((.*?)\)\s*\{',
                        current_src
                    )
                    if fn_def:
                        ret_type = fn_def.group(1).strip()
                        func_ident = fn_def.group(2).strip()
                        params = fn_def.group(3).strip()
                        new_decl = f"{ret_type} {func_ident}({params});"
                        include_dir = os.path.join(_root_dir, "include")
                        try:
                            result = subprocess.run(
                                ["grep", "-rln", "-P", "--include=*.h",
                                 rf'\bvoid\s+{re.escape(func_ident)}\s*\(', include_dir],
                                capture_output=True, text=True, timeout=5
                            )
                            if result.stdout:
                                h_site = result.stdout.splitlines()[0].strip()
                                if h_site not in backups:
                                    with open(h_site, "r") as f:
                                        backups[h_site] = f.read()
                                h_content = backups[h_site]
                                h_content = re.sub(
                                    rf'\bvoid\s+{re.escape(func_ident)}\s*\(.*?\)\s*;',
                                    new_decl, h_content
                                )
                                with open(h_site, "w") as f:
                                    f.write(h_content)
                                fixes_acc.append({"type": "header_rectification",
                                                  "file": h_site, "content": new_decl})
                                fixed_something = True
                                _log(f"[builder] Auto-fix: updated void→{ret_type} for "
                                     f"{func_ident} in {os.path.basename(h_site)}")
                        except Exception:
                            pass

        if fixed_something:
            with open(source_path, "w") as f:
                f.write(src_content)

        return fixed_something
    except Exception as e:
        import traceback
        _log(f"[builder] Auto-fix encountered an exception: {e}")
        _log(traceback.format_exc())
        return False
    

def _run_build_and_score(func_name, unit_name, c_code, externs, headers, state=None):
    """Run build + objdiff scoring, similar to DecompOrchestrator but standalone.

    This writes code temporarily, builds, scores, and reverts.
    """
    process_log = []
    fixes_acc = []
    def _log(msg):
        print(msg)
        process_log.append(msg)

    if state is None:
        state = {}
    # Resolve source path from unit name
    source_path = _resolve_source_path(unit_name)
    if not source_path or not os.path.exists(source_path):
        return {"status": "error", "log": f"Source file not found for unit {unit_name}"}

    header_path = source_path.replace('src/', 'include/').replace('.c', '.h')
    externs_path = "include/UnknownHeaders.h"

    # Backup originals
    backups = {}
    masking.clear_registry()
    
    for p in [source_path, header_path, externs_path]:
        if p and os.path.exists(p):
            with open(p, "r") as f:
                content = f.read()
                backups[p] = content
                # Mask the original file before we do anything to it
                masked = masking.mask_text(content)
                if masked != content:
                    _log(f"[builder] Masked vendor syntax in {os.path.basename(p)}")
                    with open(p, "w") as f_masked:
                        f_masked.write(masked)

    try:
        # Write the C code to the source file
        print(f"[builder] === CODE BEING WRITTEN TO {source_path} ===")
        # Mask the incoming code from LLM too, in case it includes vendor syntax
        c_code_masked = masking.mask_text(c_code)
        print(c_code_masked)
        print(f"[builder] === END CODE ===")
        
        if not _commit_code_to_source(source_path, func_name, c_code_masked):
            return {"status": "error", "log": f"Could not find {func_name} stub in {source_path}"}

        # Apply struct modifications using struct_utils
        struct_updates = state.get("struct_updates", [])
        if struct_updates:
            # struct_updates is now a list of {type_name: str, actions: list}
            for update in struct_updates:
                type_name = update.get("type_name")
                actions = update.get("actions", [])
                if not type_name or not actions:
                    continue
                
                _log(f"[builder] Applying struct modifications for {type_name} via struct_utils")
                
                # 1. Find the file containing the struct
                filepath = _find_struct_file(type_name, source_path=source_path)
                if not filepath:
                    _log(f"[builder] WARNING: Could not find file for {type_name}")
                    continue
                
                # 2. Extract current definition
                if filepath not in backups:
                     with open(filepath, "r") as f:
                         backups[filepath] = f.read()
                
                current_file_content = backups[filepath]
                # We need a way to extract JUST the struct block.
                # Use a helper or regex.
                from .llm import _extract_struct_block
                old_struct_def = _extract_struct_block(filepath, type_name)
                if not old_struct_def:
                    _log(f"[builder] WARNING: Could not extract {type_name} from {filepath}")
                    continue
                
                # 3. Use struct_utils to reconcile
                new_struct_def = struct_utils.reconcile_struct(old_struct_def, actions, log=_log)
                
                # 4. Replace in file
                new_content = current_file_content.replace(old_struct_def, new_struct_def)
                with open(filepath, "w") as f:
                    f.write(new_content)
                _log(f"[builder] Updated {type_name} in {filepath}")

        # Apply header additions (prototypes, struct defs)
        if headers and headers.strip():
            _log(f"[builder] Applying header additions to {header_path}")
            # Accumulate lines into logical blocks, keeping multi-line struct/typedef
            # definitions together (brace-depth tracking) so they are never fragmented.
            pending_lines = []
            brace_depth = 0
            for raw_line in headers.strip().splitlines():
                stripped = raw_line.strip()
                if not stripped or stripped.startswith("//") or stripped.startswith("/*"):
                    continue
                pending_lines.append(raw_line)
                brace_depth += stripped.count('{') - stripped.count('}')
                if brace_depth <= 0:
                    brace_depth = 0
                    block = "\n".join(pending_lines).strip()
                    pending_lines = []
                    if not block:
                        continue
                    # Extern lines get type-aware update logic
                    match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', block)
                    if match:
                        type_str = match.group(1).strip()
                        symbol_name = match.group(2).strip()
                        _add_or_update_extern(symbol_name, block, header_path, backups, type_str=type_str, _log=_log)
                    else:
                        _append_to_file_if_missing(header_path, block, backups, _log=_log)
            # Flush any unterminated block (e.g. missing closing brace — let the compiler catch it)
            if pending_lines:
                block = "\n".join(pending_lines).strip()
                _append_to_file_if_missing(header_path, block, backups, _log=_log)

        # Apply extern additions
        if externs and externs.strip():
            _log(f"[builder] Applying extern additions/updates")
            # Ensure the source file includes UnknownHeaders.h if we are adding externs to it
            with open(source_path, "r") as f:
                src_content = f.read()
            if 'include "UnknownHeaders.h"' not in src_content:
                _log(f"[builder] Adding missing #include \"UnknownHeaders.h\" to {source_path}")
                # Insert after the first include or at the top
                inc_match = re.search(r'^#include.*?\n', src_content)
                if inc_match:
                    new_src = src_content[:inc_match.end()] + '#include "UnknownHeaders.h"\n' + src_content[inc_match.end():]
                else:
                    new_src = '#include "UnknownHeaders.h"\n' + src_content
                with open(source_path, "w") as f:
                    f.write(new_src)

            # Process extern declarations with brace-depth tracking so that
            # multi-line blocks (e.g. struct definitions) are kept intact.
            extern_rejection_errors = []
            pending_lines = []
            brace_depth = 0
            for raw_line in externs.strip().splitlines():
                stripped = raw_line.strip()
                if not stripped or stripped.startswith("//") or stripped.startswith("/*"):
                    continue
                pending_lines.append(raw_line)
                brace_depth += stripped.count('{') - stripped.count('}')
                if brace_depth <= 0:
                    brace_depth = 0
                    block = "\n".join(pending_lines).strip()
                    pending_lines = []
                    if not block:
                        continue
                    # Reject anonymous struct/union externs — MWCC C mode does not
                    # support `extern struct { ... } name;` and writing it to the
                    # shared UnknownHeaders.h would break every compilation unit.
                    if re.search(r'\bextern\s+(struct|union)\s*\{', block):
                        sym_match = re.search(r'\}\s*(\w+)\s*;', block)
                        sym_name = sym_match.group(1) if sym_match else "unknown"
                        msg = (
                            f"[builder] REJECTED anonymous struct/union extern for '{sym_name}'. "
                            f"Metrowerks CodeWarrior C mode does not support "
                            f"'extern struct {{ ... }} {sym_name};'. "
                            f"Use a named typedef instead: "
                            f"'typedef struct {{ ... }} TypeName; extern TypeName {sym_name};', "
                            f"or use a simple array extern if no struct fields are needed."
                        )
                        _log(msg)
                        extern_rejection_errors.append(msg)
                        continue
                    match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', block)
                    if match:
                        type_str = match.group(1).strip()
                        symbol_name = match.group(2).strip()
                        _add_or_update_extern(symbol_name, block, externs_path, backups, type_str=type_str, _log=_log)
                    else:
                        _append_to_file_if_missing(externs_path, block, backups)
            # Flush any unterminated block (compiler will catch remaining errors)
            if pending_lines:
                block = "\n".join(pending_lines).strip()
                if re.search(r'\bextern\s+(struct|union)\s*\{', block):
                    _log(f"[builder] REJECTED unterminated anonymous struct/union extern")
                else:
                    _append_to_file_if_missing(externs_path, block, backups)
            if extern_rejection_errors:
                return {"status": "build_error", "log": "\n".join(extern_rejection_errors), "process_log": process_log}

        # Unmask everything before build
        for p in [source_path, header_path, externs_path]:
            if p and os.path.exists(p):
                with open(p, "r") as f:
                    masked_content = f.read()
                unmasked = masking.unmask_text(masked_content)
                if unmasked != masked_content:
                    with open(p, "w") as f:
                        f.write(unmasked)

        # Build
        build_proc = subprocess.run(
            ["ninja"], capture_output=True, text=True, timeout=120,
            cwd=_root_dir,
        )
        
        # --- Auto-fix Logic ---
        if build_proc.returncode != 0:
            combined_error_output = (build_proc.stdout or "") + (build_proc.stderr or "")
            if _attempt_auto_fix(combined_error_output, source_path, backups, _log, fixes_acc):
                _log("[builder] Re-running build after auto-fix...")
                
                # Unmask AGAIN after auto-fix
                for p in [source_path, header_path, externs_path]:
                    if p and os.path.exists(p):
                        with open(p, "r") as f:
                            masked_content = f.read()
                        unmasked = masking.unmask_text(masked_content)
                        if unmasked != masked_content:
                            with open(p, "w") as f:
                                f.write(unmasked)

                build_proc = subprocess.run(
                    ["ninja"], capture_output=True, text=True, timeout=120,
                    cwd=_root_dir,
                )
        # ----------------------
        
        if build_proc.returncode != 0:
            combined = (build_proc.stdout or "") + (build_proc.stderr or "")
            _log(f"[builder] === FULL BUILD ERROR ===")
            _log(combined)
            _log(f"[builder] === END BUILD ERROR ===")
            filtered = _filter_build_output(combined)
            # Detect shared-header corruption: errors originating from a shared
            # include file (not just the target source) indicate that an extern
            # declaration written to that header is syntactically broken and will
            # fail every compilation unit until fixed.  Annotate clearly so the
            # LLM understands what went wrong (the raw CW error is cryptic).
            shared_headers = ["UnknownHeaders.h", "UnknownHomes_Game.h", "mssbTypes.h"]
            for sh in shared_headers:
                if f"In: include" in combined and sh in combined:
                    filtered = (
                        f"[SHARED HEADER CORRUPTION] A syntax error in {sh} is breaking ALL "
                        f"compilation units (not just the target file).  This was caused by an "
                        f"invalid declaration in extern_declarations that was written to the shared "
                        f"header.  Common cause: 'extern struct {{ ... }} name;' is NOT valid in "
                        f"Metrowerks CodeWarrior C mode.  Fix: use a named typedef "
                        f"('typedef struct {{ ... }} TypeName; extern TypeName name;') or a plain "
                        f"scalar/array extern.  The header has been reverted automatically.\n\n"
                        + filtered
                    )
                    break
            return {"status": "build_error", "log": filtered, "process_log": process_log}

        # Score via objdiff
        objdiff_cmd = [
            "objdiff", "diff", "--format", "json",
            "-u", unit_name, "-o", "-", func_name,
        ]
        obj_proc = subprocess.run(
            objdiff_cmd, capture_output=True, text=True, timeout=30,
            cwd=_root_dir,
        )
        if obj_proc.returncode != 0:
            combined = (obj_proc.stdout or "") + (obj_proc.stderr or "")
            return {"status": "objdiff_error", "log": combined, "process_log": process_log}

        data = json.loads(obj_proc.stdout)

        # Extract match percent and mismatch count
        score = 0.0
        mismatch_count = 0
        if "right" in data and "symbols" in data["right"]:
            for sym in data["right"]["symbols"]:
                if sym.get("name") == func_name:
                    score = sym.get("match_percent", 0.0) / 100.0
                    
                    # Count mismatches
                    for inst in sym.get("instructions", []):
                        if inst.get("diff_kind") != "None":
                            mismatch_count += 1
                    break

        # Get textual diff via feedback_diff
        fb_proc = subprocess.run(
            ["python3", os.path.join(_tools_dir, "feedback_diff.py"),
             unit_name, func_name],
            capture_output=True, text=True, timeout=30,
            cwd=_root_dir,
        )
        asm_diff = fb_proc.stdout or fb_proc.stderr or ""

        # Get current compiled assembly
        cur_asm_proc = subprocess.run(
            ["python3", os.path.join(_tools_dir, "feedback_diff.py"),
             unit_name, func_name, "--current-asm"],
            capture_output=True, text=True, timeout=30,
            cwd=_root_dir,
        )
        current_asm = cur_asm_proc.stdout or ""

        # Phase 1.3: Token-Efficient JSON Summarization
        mismatch_summary = feedback_diff.summarize_objdiff_json(data, func_name)

        # Phase 0.2: Git Checkpointing (Moved from builder_node to happen BEFORE revert)
        try:
            if score > 0:
                _log(f"[builder] Checkpointing results before revert...")
                subprocess.run(["git", "add", "-A"], cwd=_root_dir, check=True)
                commit_msg = f"decomp {func_name}: {score*100.0:.1f}% match ({mismatch_count} mismatches)"
                subprocess.run(["git", "commit", "-m", commit_msg], cwd=_root_dir, check=True)
        except Exception as e:
            _log(f"[builder] Git checkpointing failed: {e}")

        return {
            "status": "success",
            "score": score,
            "mismatch_count": mismatch_count,
            "mismatch_summary": mismatch_summary,
            "asm_diff": asm_diff,
            "current_asm": current_asm,
            "unit": unit_name,
            "process_log": process_log,
            "fixes": fixes_acc,
        }

    except subprocess.TimeoutExpired as e:
        return {"status": "build_timeout", "log": f"Process timed out: {e}", "process_log": process_log}
    except Exception as e:
        return {"status": "error", "log": str(e), "process_log": process_log}
    finally:
        # Always revert
        for p, content in backups.items():
            with open(p, "w") as f:
                f.write(content)


def _resolve_source_path(unit_name):
    """Convert objdiff unit name to source file path."""
    # Try objdiff.json first
    objdiff_path = os.path.join(_root_dir, "objdiff.json")
    if os.path.exists(objdiff_path):
        try:
            with open(objdiff_path, "r") as f:
                config = json.load(f)
            for unit in config.get("units", []):
                if unit.get("name") == unit_name:
                    base_path = unit.get("base_path", "")
                    if base_path:
                        parts = base_path.split('/')
                        try:
                            src_idx = parts.index('src')
                            rel = "/".join(parts[src_idx:])
                            if rel.endswith('.o'):
                                rel = rel[:-2] + '.c'
                            return os.path.join(_root_dir, rel)
                        except ValueError:
                            pass
        except Exception:
            pass

    # Fallback: guess from unit name
    # game/game/rep_720 -> src/game/rep_720.c
    parts = unit_name.split('/')
    if len(parts) > 1:
        return os.path.join(_root_dir, f"src/{'/'.join(parts[1:])}.c")


def _add_or_update_extern(symbol_name, new_declaration, default_path, backups=None, type_str=None, _log=print):
    """Find an existing extern for a symbol and update it, or add to default_path."""
    include_dir = os.path.join(_root_dir, "include")

    # If a type_str is provided, check if we need to include a header for it
    if type_str:
        # Extract base type
        base_type = re.sub(r'[\*\s]', '', type_str)
        base_type = base_type.replace('const', '').replace('volatile', '').strip()

        # If it's not a primitive type, try to find where it's defined
        if base_type and base_type not in ('void', 'int', 'char', 'short', 'long',
                                         'u8', 'u16', 'u32', 'u64', 's8', 's16',
                                         's32', 's64', 'f32', 'f64', 'BOOL'):
            try:
                # Search for struct definition
                pattern = rf'(?:typedef\s+)?(?:struct|enum|union)\s+_?{re.escape(base_type)}\b'
                result = subprocess.run(
                    ["grep", "-rln", "-P", "--include=*.h", pattern, include_dir],
                    capture_output=True, text=True, timeout=5
                )
                if result.stdout:
                    for filepath in result.stdout.strip().splitlines():
                        filepath = filepath.strip()
                        if not filepath or not os.path.exists(filepath):
                            continue
                        # If the type is defined in a DIFFERENT header than where we are adding the extern,
                        # we must include that header in the target header.
                        target_header = os.path.join(_root_dir, default_path)
                        if os.path.abspath(filepath) != os.path.abspath(target_header):
                            rel_inc = os.path.relpath(filepath, include_dir)
                            inc_line = f'#include "{rel_inc}"'
                            _log(f"[builder] Type {base_type} found in {rel_inc}. Adding include to {default_path}")
                            _append_to_file_if_missing(default_path, inc_line, backups, _log=_log)
            except Exception as e:
                _log(f"[builder] Warning during type search: {e}")

    # Search for an existing extern declaration in any header
    found_anywhere = False
    found_in_default = False
    try:
        # Grep for 'extern ... symbol_name ... ;'
        # We use a pattern that matches 'extern' and then the symbol name followed by optional array/semicolon
        pattern = rf'extern\s+.*?\b{re.escape(symbol_name)}\b.*?;'
        result = subprocess.run(
            ["grep", "-rln", "-P", "--include=*.h", pattern, include_dir],
            capture_output=True, text=True, timeout=5
        )
        if result.stdout:
            for filepath in result.stdout.strip().splitlines():
                filepath = filepath.strip()
                if not filepath or not os.path.exists(filepath):
                    continue
                found_anywhere = True
                if os.path.abspath(filepath) == os.path.abspath(os.path.join(_root_dir, default_path)):
                    found_in_default = True
                _log(f"[builder] Updating extern {symbol_name} in {os.path.basename(filepath)}")
                _append_to_file_if_missing(filepath, new_declaration, backups, _log=_log)
    except Exception as e:
        _log(f"[builder] Warning during extern search: {e}")

    # If not found in our default path (UnknownHeaders.h usually), add it there too
    # so the current source file can definitely see it (since we ensure source includes UnknownHeaders.h)
    if not found_in_default:
        _log(f"[builder] Extern {symbol_name} not in {default_path}. Adding for visibility.")
        _append_to_file_if_missing(default_path, new_declaration, backups, _log=_log)

def _commit_code_to_source(src_path, func_name, c_code):
    """Temporarily replace a function body in the source file using Tree-Sitter."""
    with open(src_path, "r") as f:
        content = f.read()

    try:
        # Phase 2.2: Tree-Sitter Integration
        C_LANGUAGE = Language(tsc.language())
        parser = Parser(C_LANGUAGE)
        tree = parser.parse(bytes(content, "utf8"))
        
        # Query to find function definition by name
        query = Query(C_LANGUAGE, """
            (function_definition
                declarator: (function_declarator
                    declarator: (identifier) @name)) @func
        """)

        cursor = QueryCursor(query)
        captures = cursor.captures(tree.root_node)
        target_node = None

        if 'name' in captures:
            for node in captures['name']:
                if node.text.decode('utf8') == func_name:
                    # In newer tree-sitter, we need to find the function_definition parent
                    parent = node.parent
                    while parent and parent.type != 'function_definition':
                        parent = parent.parent
                    target_node = parent
                    break
        if target_node:
            start_byte = target_node.start_byte
            end_byte = target_node.end_byte
            
            new_content = (
                content[:start_byte] + 
                "\n" + c_code.strip() + 
                content[end_byte:]
            )
            with open(src_path, "w") as f:
                f.write(new_content)
            return True
            
    except Exception as e:
        print(f"[builder] Tree-Sitter error: {e}, falling back to regex")

    # Regex Fallback
    # Pattern to match function signature and its body
    pattern = (
        rf"(?m)^[ \t]*[^\n;\(]+?\b{re.escape(func_name)}\b"
        rf"\s*\([^)]*\)\s*\{{.*?^\}}"
    )

    match = re.search(pattern, content, re.DOTALL)
    if not match:
        # Looser fallback
        pattern = (
            rf"(?m)^[ \t]*[^\n;\(]+?\b{re.escape(func_name)}\b"
            rf"\s*\([^)]*\)\s*\{{.*?\}}"
        )
        match = re.search(pattern, content, re.DOTALL)

    if not match:
        return False

    new_content = content[:match.start()] + "\n" + c_code.strip() + content[match.end():]

    with open(src_path, "w") as f:
        f.write(new_content)

    return True


def _append_to_file_if_missing(file_path, content_to_add, backups=None, _log=print):
    """Append blocks of code to a file if they are not already present.

    Attempts to parse multi-line blocks (structs, functions) and treat them as units.
    Only blocks not already in the file are appended.
    """
    file_path = os.path.join(_root_dir, file_path) if not os.path.isabs(file_path) else file_path

    if not os.path.exists(file_path):
        _log(f"[builder] Warning: {file_path} does not exist, creating it")
        with open(file_path, "w") as f:
            f.write("")

    with open(file_path, "r") as f:
        existing_content = f.read()

    if backups is not None and file_path not in backups:
        backups[file_path] = existing_content

    # Split into logical blocks
    # A block is:
    # 1. A preprocessor directive (#include, etc)
    # 2. A struct/enum/union definition (multi-line)
    # 3. A function prototype (can be multi-line but usually ends in ;)
    # 4. An extern declaration
    
    # Simple regex-based block splitting
    blocks = []
    
    # Extract blocks that look like structs/enums
    # Pattern: typedef struct { ... } Name; or struct Name { ... };
    struct_pattern = r'(?s)(?:typedef\s+)?(?:struct|enum|union)\s+\w*\s*\{.*?\}(?:\s*\w+)?\s*;'
    for match in re.finditer(struct_pattern, content_to_add):
        blocks.append(match.group(0).strip())
        content_to_add = content_to_add.replace(match.group(0), "")

    # Extract other single-line or semicolon-terminated blocks
    for line in content_to_add.strip().splitlines():
        line = line.strip()
        if not line:
            continue
        
        # EXTRA SAFETY: skip prompt placeholders
        placeholders = [
            "Extern variables to ADD",
            "New prototypes or structs to ADD to the header",
            "The complete C code for the function",
            "Name of the struct to modify",
            "// M2C could not decompile",
            "Brief summary of changes",
        ]
        if any(p in line for p in placeholders):
            continue
        # Skip comments
        if line.startswith("//") or line.startswith("/*"):
            continue
            
        blocks.append(line)

    blocks_to_add = []
    for block in blocks:
        # Detect extern redefinitions
        extern_match = re.match(r'^[ \t]*extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', block)
        if extern_match:
            new_type = extern_match.group(1).strip()
            symbol_name = extern_match.group(2).strip()
            existing_extern_pattern = rf'(?m)^[ \t]*extern\s+.*?\b{re.escape(symbol_name)}\b.*?;'
            existing_match = re.search(existing_extern_pattern, existing_content)

            if existing_match:
                existing_line = existing_match.group(0)
                if existing_line.strip() == block:
                    continue
                existing_type_match = re.match(r'^[ \t]*extern\s+([\w\s\*]+?)\s+\w+', existing_line)
                if existing_type_match:
                    existing_type = existing_type_match.group(1).strip()
                    if existing_type not in ['u32', '?', 'void'] and new_type in ['u32', '?', 'void']:
                        _log(f"[builder] Protecting existing type '{existing_type}' for {symbol_name}")
                        continue
                _log(f"[builder] Redefining {symbol_name} in {os.path.basename(file_path)}")
                existing_content = (
                    existing_content[:existing_match.start()]
                    + block
                    + existing_content[existing_match.end():]
                )
                continue

        # Detect function redefinitions
        func_match = re.match(r'^[ \t]*([\w\s\*]+?)\s+(\w+)\s*\((.*?)\)\s*;', block)
        if func_match:
            func_name = func_match.group(2).strip()
            existing_func_pattern = rf'(?m)^[ \t]*([\w\s\*]+?)\s+{re.escape(func_name)}\s*\(.*?\)\s*;?'
            existing_match = re.search(existing_func_pattern, existing_content)
            if existing_match:
                if existing_match.group(0).strip().replace(' ', '') == block.replace(' ', ''):
                    continue
                _log(f"[builder] Updating prototype {func_name} in {os.path.basename(file_path)}")
                existing_content = (
                    existing_content[:existing_match.start()]
                    + block
                    + existing_content[existing_match.end():]
                )
                continue

        # For structs and other blocks, check for existence
        # We use a normalized search (ignoring whitespace differences)
        normalized_block = re.sub(r'\s+', '', block)
        normalized_existing = re.sub(r'\s+', '', existing_content)
        if normalized_block in normalized_existing:
            continue
            
        blocks_to_add.append(block)

    if blocks_to_add:
        if "#endif" in existing_content:
            last_endif = existing_content.rfind("#endif")
            new_content = (
                existing_content[:last_endif]
                + "\n" + "\n\n".join(blocks_to_add) + "\n\n"
                + existing_content[last_endif:]
            )
        else:
            new_content = existing_content.rstrip() + "\n\n" + "\n\n".join(blocks_to_add) + "\n"

        with open(file_path, "w") as f:
            f.write(new_content)
        _log(f"[builder] Added {len(blocks_to_add)} blocks to {os.path.basename(file_path)}")
    elif existing_content != (backups.get(file_path) if backups else None):
        with open(file_path, "w") as f:
            f.write(existing_content)
    else:
        _log(f"[builder] No new lines to add to {os.path.basename(file_path)}")


def _find_struct_file(type_name, source_path=None):
    """Find which header file (or source file) contains the struct definition."""
    # Grep for it with word boundaries, handling optional typedef/struct/enum/union and whitespace
    # Pattern explanation:
    # (typedef\s+)?           Optional typedef
    # (struct|enum|union)\s+  Required struct/enum/union
    # _?{type_name}\b           Optional leading underscore and the name
    pattern = rf'(typedef\s+)?(struct|enum|union)\s+_?{re.escape(type_name)}\b'

    # 0. Check source file first if provided
    if source_path and os.path.exists(source_path):
        try:
            res = subprocess.run(
                ["grep", "-qE", pattern, source_path],
                capture_output=True, cwd=_root_dir
            )
            if res.returncode == 0:
                return source_path
        except:
            pass

    # We use ripgrep if available for speed, otherwise standard grep
    try:
        # Search all headers in include/
        res = subprocess.run(
            ["grep", "-rlE", pattern, "include"],
            capture_output=True, text=True, cwd=_root_dir
        )
        if res.returncode == 0:
            files = res.stdout.strip().splitlines()
            if files:
                # Prioritize UnknownHeaders.h if multiple matches
                for f in files:
                    if "UnknownHeaders.h" in f:
                        return os.path.join(_root_dir, f)
                return os.path.join(_root_dir, files[0])
    except:
        pass
    return None

# ---------------------------------------------------------------------------
# Node E: Committer (permanent write on match)
# ---------------------------------------------------------------------------

def committer_node(state):
    """Permanently commit matched code to the source file."""
    func_name = state["function_name"]
    unit_name = state["unit_name"]
    current_code = state.get("current_c_code", "")

    print(f"[committer] 100% MATCH! Committing {func_name}...")

    source_path = _resolve_source_path(unit_name)
    if not source_path:
        return {
            "status": "error",
            "feedback": f"Could not find source path for {unit_name}",
        }

    header_path = source_path.replace('src/', 'include/').replace('.c', '.h')
    externs_path = "include/UnknownHeaders.h"

    success = _commit_code_to_source(source_path, func_name, current_code)

    if success:
        # Apply struct modifications permanently
        struct_updates = state.get("struct_updates", [])
        if struct_updates:
            for update in struct_updates:
                type_name = update.get("type_name")
                actions = update.get("actions", [])
                if not type_name or not actions:
                    continue
                
                print(f"[committer] Permanently applying struct modifications for {type_name}")
                filepath = _find_struct_file(type_name, source_path=source_path)
                if filepath:
                    from .llm import _extract_struct_block
                    with open(filepath, "r") as f:
                        content = f.read()
                    old_struct_def = _extract_struct_block(filepath, type_name)
                    if old_struct_def:
                        new_struct_def = struct_utils.reconcile_struct(old_struct_def, actions, log=print)
                        new_content = content.replace(old_struct_def, new_struct_def)
                        with open(filepath, "w") as f:
                            f.write(new_content)
                        print(f"[committer] Updated {type_name} in {filepath}")

        # Apply header additions
        headers = state.get("local_headers", "")
        if headers and headers.strip():
            print(f"[committer] Permanently applying header additions to {header_path}")
            _append_to_file_if_missing(header_path, headers)

        # Apply extern additions
        externs = state.get("externs", "")
        if externs and externs.strip():
            print(f"[committer] Permanently applying extern additions to {externs_path}")
            _append_to_file_if_missing(externs_path, externs)

        # Rebuild to make sure it sticks
        build_proc = subprocess.run(
            ["ninja"], capture_output=True, text=True, timeout=120,
            cwd=_root_dir,
        )
        if build_proc.returncode != 0:
            print("[committer] Final build failed, attempting reactive auto-fix...")
            # We don't need fixes_acc here because we are already in the final phase
            _attempt_auto_fix(build_proc.stdout + build_proc.stderr, source_path, {}, print)
            # Rebuild one last time
            subprocess.run(
                ["ninja"], capture_output=True, text=True, timeout=120,
                cwd=_root_dir,
            )

        print(f"[committer] Successfully committed {func_name} to {source_path}")

        # Phase 0.4: Isolated Branching - Merge back to original branch
        original_branch = state.get("original_branch")
        if original_branch:
            try:
                tmp_branch = f"decomp/{func_name}"
                print(f"[committer] Merging only relevant files from {tmp_branch} back to {original_branch}...")
                
                # Final commit on temp branch to ensure everything is saved
                subprocess.run(["git", "add", "-A"], cwd=_root_dir)
                subprocess.run(["git", "commit", "-m", f"final matched {func_name}"], cwd=_root_dir)
                
                # Switch back to original branch
                # We use -f because files like decomp_debug.log might have untracked changes
                # that were previously tracked, blocking a normal checkout.
                subprocess.run(["git", "checkout", "-f", original_branch], cwd=_root_dir, check=True)
                
                # Instead of merge --squash, we'll just checkout the specific files we modified
                # This ensures NO junk files (logs, tags) ever hit the main branch.
                relevant_files = [source_path, header_path, os.path.join(_root_dir, "key_learnings.md")]
                if externs_path and os.path.exists(externs_path):
                    relevant_files.append(externs_path)
                
                for fpath in relevant_files:
                    if fpath:
                        rel_fpath = os.path.relpath(fpath, _root_dir)
                        print(f"[committer] Checking out {rel_fpath} from {tmp_branch}...")
                        subprocess.run(["git", "checkout", tmp_branch, "--", rel_fpath], cwd=_root_dir, check=True)
                
                # Commit the changes to original branch
                commit_msg = f"matched {func_name}\n\nIteration attempts on {tmp_branch} were discarded."
                subprocess.run(["git", "commit", "-m", commit_msg], cwd=_root_dir, check=True)
                
                # Delete temp branch
                subprocess.run(["git", "branch", "-D", tmp_branch], cwd=_root_dir)
                print(f"[committer] Successfully merged results and switched back to {original_branch}")
            except Exception as e:
                print(f"[committer] Git merge failed: {e}")

        return {
            "status": "matched",
            "messages": [("ai", f"MATCHED! Committed {func_name} to {source_path}.")],
        }
    else:
        print(f"[committer] Failed to commit {func_name}")
        return {
            "status": "error",
            "feedback": f"Failed to commit {func_name} to source.",
        }


# ---------------------------------------------------------------------------
# Node F: Summarizer (Key Learnings)
# ---------------------------------------------------------------------------

def summarizer_node(state):
    """Summarize substantial key learnings from this run and append them globally."""
    from .llm import summarize_key_learnings
    return summarize_key_learnings(state)

