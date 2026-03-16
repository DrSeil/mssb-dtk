"""Graph node functions for the decompilation loop.

Each node takes DecompState and returns a partial state update dict.
"""

import os
import sys
import subprocess
import json
import re

# Add tools directory to path for imports
_script_dir = os.path.dirname(os.path.abspath(__file__))
_tools_dir = os.path.dirname(_script_dir)
_root_dir = os.path.dirname(_tools_dir)
sys.path.insert(0, _tools_dir)

import decomp_helper
import gen_prompt
import feedback_diff


# ---------------------------------------------------------------------------
# Node A: Source Finder
# ---------------------------------------------------------------------------

def source_finder_node(state):
    """Resolve function name → module, source file, ASM, unit, header, Ghidra."""
    func_name = state["function_name"]
    print(f"[source_finder] Resolving {func_name}...")

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

    # Find the ASM path for reference
    rel_path = os.path.splitext(source_file)[0] + ".s"
    if module:
        asm_path = os.path.join(_root_dir, f"build/GYQE01/{module}/asm", rel_path)
    else:
        asm_path = os.path.join(_root_dir, "build/GYQE01/asm", rel_path)

    print(f"[source_finder] Found: module={module}, unit={unit_name}, "
          f"source={source_file}, asm_lines={len(asm_text.splitlines())}")

    return {
        "module": module,
        "source_file": source_file,
        "source_path": src_path or "",
        "asm_path": asm_path,
        "asm_text": asm_text,
        "unit_name": unit_name,
        "header_path": header_path or "",
        "header_content": header_content or "",
        "ghidra_output": ghidra_output or "",
        "status": "running",
    }


# ---------------------------------------------------------------------------
# Node B: Decompiler (M2C)
# ---------------------------------------------------------------------------

def decompiler_node(state):
    """Run M2C to generate the initial C skeleton."""
    func_name = state["function_name"]
    print(f"[decompiler] Running M2C for {func_name}...")

    # Use gen_prompt.run_m2c which wraps m2c_helper.py
    m2c_output = gen_prompt.run_m2c(func_name)

    if not m2c_output:
        print(f"[decompiler] M2C failed, using empty skeleton")
        m2c_output = f"// M2C could not decompile {func_name}\n"

    # Split M2C output into externs / headers / body
    # Import the split function from server.py or implement locally
    split = _split_m2c_draft(m2c_output, func_name)

    current_c_code = split.get("body", m2c_output)
    externs = split.get("externs", "")
    local_headers = split.get("headers", "")

    print(f"[decompiler] M2C output: {len(m2c_output)} chars, "
          f"body: {len(current_c_code)} chars")

    return {
        "m2c_output": m2c_output,
        "current_c_code": current_c_code,
        "externs": externs,
        "local_headers": local_headers,
        "iterations": 0,
        "attempts": [],
        "messages": [("ai", f"Generated initial M2C decompilation for {func_name}.")],
    }


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

    func_name = state["function_name"]
    iteration = state.get("iterations", 0)
    print(f"[refactorer] Iteration {iteration + 1} for {func_name}...")

    prompt = ""
    try:
        llm_result, tier, prompt, raw_response = invoke_refactor(state, escalate_after=escalate_after)
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

    # Post-process: strip #ifndef/#define/#endif guards and #include from header additions
    header_lines = []
    for line in header_adds.splitlines():
        stripped = line.strip()
        if stripped.startswith("#ifndef") or stripped.startswith("#define __") or stripped.startswith("#endif"):
            continue
        if stripped.startswith("#include"):
            continue
        header_lines.append(line)
    header_adds = "\n".join(header_lines).strip()

    print(f"[refactorer] Got {len(func_code)} chars function, "
          f"{len(struct_updates)} struct updates, "
          f"{len(header_adds)} chars header, {len(extern_adds)} chars externs from {tier} LLM")
    print(f"[refactorer] === LLM FUNCTION ===")
    print(func_code)
    print(f"[refactorer] === END LLM FUNCTION ===")
    if struct_updates:
        print(f"[refactorer] === LLM STRUCT UPDATES ({len(struct_updates)}) ===")
        for upd in struct_updates:
            print(f"  @@@ {upd['type_name']}")
            print(upd['definition'])
            print(f"  @@@ END")
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
    feedback_text = result.get("asm_diff", "")
    build_log = result.get("log", "")

    # Determine build error vs diff feedback
    build_error = ""
    if status_code in ("build_error", "build_timeout"):
        build_error = build_log
        feedback_text = ""
        match_percent = 0.0
    elif status_code == "error":
        build_error = build_log or result.get("message", "Unknown error")

    # Build the attempt record
    attempt = {
        "c_code": current_code,
        "match_percent": match_percent,
        "feedback": feedback_text,
        "current_asm": current_asm,
        "build_error": build_error,
    }

    new_status = "running"
    if match_percent >= 100.0:
        new_status = "matched"
    elif status_code == "error":
        new_status = "error"

    print(f"[builder] Result: {match_percent:.1f}% match, status={new_status}")

    res = {
        "feedback": feedback_text,
        "build_log": build_error,
        "match_percent": match_percent,
        "current_asm": current_asm,
        "iterations": iteration + 1,
        "attempts": state.get("attempts", []) + [attempt],
        "status": new_status,
        "messages": [("ai",
            f"Iteration {iteration + 1}: {match_percent:.1f}% match. "
            f"Status: {new_status}."
        )],
    }

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
        content.append(f"@@@ {upd.get('type_name')}\n{upd.get('definition')}\n@@@ END")

    content.append(f"\n\nBUILD LOG")
    content.append(f"=========\n")
    content.append(attempt.get("build_error", "No build errors"))

    content.append(f"\nOBJDIFF FEEDBACK")
    content.append(f"================\n")
    content.append(attempt.get("feedback", "No diff feedback (matched or build error)"))

    with open(log_file, "w") as f:
        f.write("\n".join(content))
    print(f"[builder] Logged attempt to {log_file}")


def _run_build_and_score(func_name, unit_name, c_code, externs, headers, state=None):
    """Run build + objdiff scoring, similar to DecompOrchestrator but standalone.

    This writes code temporarily, builds, scores, and reverts.
    """
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
    for p in [source_path, header_path, externs_path]:
        if p and os.path.exists(p):
            with open(p, "r") as f:
                backups[p] = f.read()

    try:
        # Write the C code to the source file
        print(f"[builder] === CODE BEING WRITTEN TO {source_path} ===")
        print(c_code)
        print(f"[builder] === END CODE ===")
        if not _commit_code_to_source(source_path, func_name, c_code):
            return {"status": "error", "log": f"Could not find {func_name} stub in {source_path}"}

        # Apply struct updates (find and replace existing definitions)
        struct_updates = state.get("struct_updates", [])
        if struct_updates:
            for upd in struct_updates:
                type_name = upd["type_name"]
                new_def = upd["definition"]
                print(f"[builder] Applying struct update for {type_name}")
                # _apply_struct_update returns the filepath it modified
                updated = _apply_struct_update(type_name, new_def, backups)
                if updated:
                    print(f"[builder] Updated {type_name} in {updated}")
                else:
                    # FALLBACK: If update failed (type not found), add it to the primary header
                    print(f"[builder] WARNING: Could not find {type_name} to update, adding to {header_path}")
                    _append_to_file_if_missing(header_path, new_def, backups)

        # Apply header additions (prototypes, struct defs)
        if headers and headers.strip():
            print(f"[builder] Applying header additions to {header_path}")
            for line in headers.strip().splitlines():
                line = line.strip()
                if not line or line.startswith("//") or line.startswith("/*"):
                    continue
                # If it's an extern, handle it globally
                match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', line)
                if match:
                    symbol_name = match.group(2).strip()
                    _add_or_update_extern(symbol_name, line, header_path, backups)
                else:
                    _append_to_file_if_missing(header_path, line, backups)

        # Apply extern additions
        if externs and externs.strip():
            print(f"[builder] Applying extern additions/updates")
            for line in externs.strip().splitlines():
                line = line.strip()
                if not line or line.startswith("//") or line.startswith("/*"):
                    continue
                # Extract symbol name from extern declaration
                # Pattern: extern Type Symbol; or extern Type Symbol[];
                match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', line)
                if match:
                    symbol_name = match.group(2).strip()
                    _add_or_update_extern(symbol_name, line, externs_path, backups)
                else:
                    # Fallback for complex lines
                    _append_to_file_if_missing(externs_path, line, backups)

        # Build
        build_proc = subprocess.run(
            ["ninja"], capture_output=True, text=True, timeout=120,
            cwd=_root_dir,
        )
        if build_proc.returncode != 0:
            combined = (build_proc.stdout or "") + (build_proc.stderr or "")
            print(f"[builder] === FULL BUILD ERROR ===")
            print(combined)
            print(f"[builder] === END BUILD ERROR ===")
            return {"status": "build_error", "log": combined}

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
            return {"status": "objdiff_error", "log": combined}

        data = json.loads(obj_proc.stdout)

        # Extract match percent
        score = 0.0
        if "right" in data and "symbols" in data["right"]:
            for sym in data["right"]["symbols"]:
                if sym.get("name") == func_name:
                    score = sym.get("match_percent", 0.0) / 100.0
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

        return {
            "status": "success",
            "score": score,
            "asm_diff": asm_diff,
            "current_asm": current_asm,
            "unit": unit_name,
        }

    except subprocess.TimeoutExpired as e:
        return {"status": "build_timeout", "log": f"Process timed out: {e}"}
    except Exception as e:
        return {"status": "error", "log": str(e)}
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


def _add_or_update_extern(symbol_name, new_declaration, default_path, backups=None):
    """Find an existing extern for a symbol and update it, or add to default_path."""
    include_dir = os.path.join(_root_dir, "include")
    
    # Search for an existing extern declaration in any header
    try:
        # Grep for 'extern ... symbol_name ... ;'
        # We use a pattern that matches 'extern' and then the symbol name followed by optional array/semicolon
        pattern = rf'extern\s+.*?\b{re.escape(symbol_name)}\b.*?;'
        result = subprocess.run(
            ["grep", "-rln", "--include=*.h", pattern, include_dir],
            capture_output=True, text=True, timeout=5
        )
        if result.stdout:
            for filepath in result.stdout.strip().splitlines():
                filepath = filepath.strip()
                if not filepath or not os.path.exists(filepath):
                    continue
                print(f"[builder] Updating extern {symbol_name} in {os.path.basename(filepath)}")
                _append_to_file_if_missing(filepath, new_declaration, backups)
            return
    except Exception as e:
        print(f"[builder] Warning during extern search: {e}")

    # Not found, add to default path
    _append_to_file_if_missing(default_path, new_declaration, backups)


def _commit_code_to_source(src_path, func_name, c_code):
    """Temporarily replace a function body in the source file."""
    with open(src_path, "r") as f:
        content = f.read()

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


def _append_to_file_if_missing(file_path, headers, backups=None):
    """Append lines to a file if they are not already present.

    Each line from headers is checked individually.
    Only lines not already in the file are appended.
    """
    file_path = os.path.join(_root_dir, file_path) if not os.path.isabs(file_path) else file_path

    if not os.path.exists(file_path):
        print(f"[builder] Warning: {file_path} does not exist, creating it")
        with open(file_path, "w") as f:
            f.write("")

    with open(file_path, "r") as f:
        existing_content = f.read()

    lines_to_add = []
    for line in headers.strip().splitlines():
        line = line.strip()
        if not line:
            continue
        # Skip comments
        if line.startswith("//") or line.startswith("/*"):
            continue

        # Detect extern redefinitions
        # Pattern: extern Type Symbol; or extern Type Symbol[];
        extern_match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', line)
        if extern_match:
            new_type = extern_match.group(1).strip()
            symbol_name = extern_match.group(2).strip()

            # Search existing content for this symbol name
            # Pattern: extern ... Symbol ... ;
            # Use a regex that is slightly more robust than simple string search
            existing_extern_pattern = rf'(?m)^extern\s+.*?\b{re.escape(symbol_name)}\b.*?;'
            existing_match = re.search(existing_extern_pattern, existing_content)

            if existing_match:
                existing_line = existing_match.group(0)
                # If the line is exactly the same, skip adding it
                if existing_line.strip() == line:
                    continue

                # If the symbol matches but the line is different (different type/array), replace it
                print(f"[builder] Redefining {symbol_name} in {os.path.basename(file_path)}")
                existing_content = (
                    existing_content[:existing_match.start()]
                    + line
                    + existing_content[existing_match.end():]
                )
                continue

        # Check if this exact line already exists
        if line in existing_content:
            continue
        lines_to_add.append(line)

    if lines_to_add:
        # Find the right place to insert — before the last #endif if present,
        # otherwise at the end
        if "#endif" in existing_content:
            # Insert before the last #endif
            last_endif = existing_content.rfind("#endif")
            new_content = (
                existing_content[:last_endif]
                + "\n" + "\n".join(lines_to_add) + "\n\n"
                + existing_content[last_endif:]
            )
        else:
            new_content = existing_content.rstrip() + "\n\n" + "\n".join(lines_to_add) + "\n"

        with open(file_path, "w") as f:
            f.write(new_content)

        print(f"[builder] Added {len(lines_to_add)} lines to {os.path.basename(file_path)}")
    elif existing_content != (backups.get(file_path) if backups else None):
        # If we replaced lines but didn't add any new ones, we still need to write
        with open(file_path, "w") as f:
            f.write(existing_content)
    else:
        print(f"[builder] No new lines to add to {os.path.basename(file_path)}")


def _apply_struct_update(type_name, new_definition, backups=None):
    """Find and replace an existing struct/typedef definition across all include files.

    Searches include/ directory for the type, extracts the full block, and replaces it.
    If backups dict is provided, backs up the file before modifying.
    Returns the file path if successful, None otherwise.
    """
    include_dir = os.path.join(_root_dir, "include")

    # Search for the type definition
    try:
        result = subprocess.run(
            ["grep", "-rln", "--include=*.h",
             f"struct _*{type_name}", include_dir],
            capture_output=True, text=True, timeout=5
        )
        if not result.stdout:
            # Try typedef
            result = subprocess.run(
                ["grep", "-rln", "--include=*.h",
                 f"typedef.*{type_name}", include_dir],
                capture_output=True, text=True, timeout=5
            )
    except Exception:
        return None

    if not result.stdout:
        print(f"[builder] Struct {type_name} not found, treating as a new addition")
        return None

    # Try each matching file
    for filepath in result.stdout.strip().splitlines():
        filepath = filepath.strip()
        if not filepath or not os.path.exists(filepath):
            continue

        try:
            with open(filepath, "r") as f:
                content = f.read()
        except Exception:
            continue

        # Find the struct/typedef block for this type
        pattern = rf'(?:typedef\s+)?(?:struct|enum|union)\s+_?{re.escape(type_name)}\b'
        match = re.search(pattern, content)
        if not match:
            continue

        start = match.start()
        # Find opening brace
        brace_pos = content.find('{', start)
        if brace_pos == -1:
            continue

        # Find matching closing brace
        depth = 0
        end = brace_pos
        for i in range(brace_pos, len(content)):
            if content[i] == '{':
                depth += 1
            elif content[i] == '}':
                depth -= 1
                if depth == 0:
                    end = i
                    break

        # Find semicolon after closing brace (and optional type name + comment)
        rest = content[end+1:]
        semi_match = re.match(r'[^;]*;[^\n]*', rest)
        if semi_match:
            end = end + 1 + semi_match.end()
        else:
            end += 1

        old_block = content[start:end]
        print(f"[builder] Replacing {len(old_block)} chars in {os.path.basename(filepath)}")

        # Backup the file before modifying (if backups dict provided)
        if backups is not None and filepath not in backups:
            backups[filepath] = content

        # Replace
        new_content = content[:start] + new_definition.strip() + content[end:]
        with open(filepath, "w") as f:
            f.write(new_content)

        return filepath

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
        # Apply struct updates permanently
        struct_updates = state.get("struct_updates", [])
        if struct_updates:
            for upd in struct_updates:
                type_name = upd["type_name"]
                new_def = upd["definition"]
                print(f"[committer] Permanently applying struct update for {type_name}")
                updated = _apply_struct_update(type_name, new_def)
                if updated:
                    print(f"[committer] Updated {type_name} in {updated}")
                else:
                    print(f"[committer] WARNING: Could not find {type_name} to update")

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
        subprocess.run(
            ["ninja"], capture_output=True, text=True, timeout=120,
            cwd=_root_dir,
        )
        print(f"[committer] Successfully committed {func_name} to {source_path}")
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
