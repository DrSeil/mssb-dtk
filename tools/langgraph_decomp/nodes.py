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
from . import struct_utils


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
        "struct_updates": state.get("struct_updates", []),
        "process_log": result.get("process_log", []),
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

    print(f"[builder] Logged attempt to {log_file}")


def _attempt_auto_fix(compiler_output: str, source_path: str, backups: dict, _log=print) -> bool:
    """Attempt to parse compiler errors and automatically fix the source file."""
    try:
        import re
        import subprocess
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
                filepath = _find_struct_file(type_name)
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
                new_struct_def = struct_utils.reconcile_struct(old_struct_def, actions)
                
                # 4. Replace in file
                new_content = current_file_content.replace(old_struct_def, new_struct_def)
                with open(filepath, "w") as f:
                    f.write(new_content)
                _log(f"[builder] Updated {type_name} in {filepath}")

        # Apply header additions (prototypes, struct defs)
        if headers and headers.strip():
            _log(f"[builder] Applying header additions to {header_path}")
            for line in headers.strip().splitlines():
                line = line.strip()
                if not line or line.startswith("//") or line.startswith("/*"):
                    continue
                # If it's an extern, handle it globally
                match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', line)
                if match:
                    type_str = match.group(1).strip()
                    symbol_name = match.group(2).strip()
                    _add_or_update_extern(symbol_name, line, header_path, backups, type_str=type_str, _log=_log)
                else:
                    _append_to_file_if_missing(header_path, line, backups, _log=_log)

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

            for line in externs.strip().splitlines():
                line = line.strip()
                if not line or line.startswith("//") or line.startswith("/*"):
                    continue
                # Extract symbol name from extern declaration
                # Pattern: extern Type Symbol; or extern Type Symbol[];
                match = re.match(r'^extern\s+([\w\s\*]+?)\s+(\w+)(\[.*?\])?\s*;', line)
                if match:
                    type_str = match.group(1).strip()
                    symbol_name = match.group(2).strip()
                    _add_or_update_extern(symbol_name, line, externs_path, backups, type_str=type_str, _log=_log)
                else:
                    # Fallback for complex lines
                    _append_to_file_if_missing(externs_path, line, backups)

        # Build
        build_proc = subprocess.run(
            ["ninja"], capture_output=True, text=True, timeout=120,
            cwd=_root_dir,
        )
        
        # --- Auto-fix Logic ---
        if build_proc.returncode != 0:
            combined_error_output = (build_proc.stdout or "") + (build_proc.stderr or "")
            if _attempt_auto_fix(combined_error_output, source_path, backups, _log):
                _log("[builder] Re-running build after auto-fix...")
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
            return {"status": "build_error", "log": combined, "process_log": process_log}

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
            "process_log": process_log,
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
    try:
        # Grep for 'extern ... symbol_name ... ;'
        # We use a pattern that matches 'extern' and then the symbol name followed by optional array/semicolon
        pattern = rf'extern\s+.*?\b{re.escape(symbol_name)}\b.*?;'
        result = subprocess.run(
            ["grep", "-rln", "-P", "--include=*.h", pattern, include_dir],
            capture_output=True, text=True, timeout=5
        )
        if result.stdout:
            found_globally = False
            for filepath in result.stdout.strip().splitlines():
                filepath = filepath.strip()
                if not filepath or not os.path.exists(filepath):
                    continue
                if filepath == os.path.join(_root_dir, default_path):
                    found_globally = True
                _log(f"[builder] Updating extern {symbol_name} in {os.path.basename(filepath)}")
                _append_to_file_if_missing(filepath, new_declaration, backups, _log=_log)
            
            if found_globally:
                return
            
            _log(f"[builder] Extern {symbol_name} found elsewhere but NOT in {default_path}. Adding to {default_path} for visibility.")
    except Exception as e:
        _log(f"[builder] Warning during extern search: {e}")

    # Not found, add to default path
    _append_to_file_if_missing(default_path, new_declaration, backups, _log=_log)


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


def _append_to_file_if_missing(file_path, headers, backups=None, _log=print):
    """Append lines to a file if they are not already present.

    Each line from headers is checked individually.
    Only lines not already in the file are appended.
    """
    file_path = os.path.join(_root_dir, file_path) if not os.path.isabs(file_path) else file_path

    if not os.path.exists(file_path):
        _log(f"[builder] Warning: {file_path} does not exist, creating it")
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

                # --- SYMBOL PROTECTION ---
                # Detect the types in both lines
                # Pattern: extern Type Symbol ... ;
                existing_type_match = re.match(r'^extern\s+([\w\s\*]+?)\s+\w+', existing_line)
                if existing_type_match:
                    existing_type = existing_type_match.group(1).strip()
                    # If the existing type is already something specific (not 'u32' or '?'),
                    # and the new type is just 'u32', DO NOT REDEFINE.
                    if existing_type not in ['u32', '?', 'void'] and new_type in ['u32', '?', 'void']:
                        _log(f"[builder] Protecting existing type '{existing_type}' for {symbol_name} (ignoring '{new_type}')")
                        continue

                # If the symbol matches but the line is different, replace it
                _log(f"[builder] Redefining {symbol_name} in {os.path.basename(file_path)}: {existing_line.strip()} -> {line.strip()}")
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

        _log(f"[builder] Added {len(lines_to_add)} lines to {os.path.basename(file_path)}")
    elif existing_content != (backups.get(file_path) if backups else None):
        # If we replaced lines but didn't add any new ones, we still need to write
        with open(file_path, "w") as f:
            f.write(existing_content)
    else:
        _log(f"[builder] No new lines to add to {os.path.basename(file_path)}")


def _find_struct_file(type_name):
    """Find which header file contains the struct definition."""
    # Common headers
    headers = [
        "include/UnknownHeaders.h",
        "include/game/GameInitVariables.h",
    ]
    
    # Grep for it
    pattern = rf'(?:typedef\s+)?(?:struct|enum|union)\s+_?{type_name}\b'
    
    # Use grep -l to find files
    try:
        res = subprocess.run(
            ["grep", "-rl", pattern, "include"],
            capture_output=True, text=True, cwd=_root_dir
        )
        if res.returncode == 0:
            files = res.stdout.strip().splitlines()
            if files:
                return os.path.join(_root_dir, files[0])
    except:
        pass
        
    for h in headers.copy():
        path = os.path.join(_root_dir, h)
        if os.path.exists(path):
            with open(path, "r") as f:
                if re.search(pattern, f.read()):
                    return path
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
                filepath = _find_struct_file(type_name)
                if filepath:
                    from .llm import _extract_struct_block
                    with open(filepath, "r") as f:
                        content = f.read()
                    old_struct_def = _extract_struct_block(filepath, type_name)
                    if old_struct_def:
                        new_struct_def = struct_utils.reconcile_struct(old_struct_def, actions)
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
