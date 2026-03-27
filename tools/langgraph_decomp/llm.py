"""Tiered LLM integration for the decompilation refactorer.

Tier 1 (local):  Ollama — used for initial attempts (iterations 1..N)
Tier 2 (cloud):  OpenRouter or Google Gemini — used after escalation

Environment variables:
    OLLAMA_MODEL        Local model name (default: "deepseek-coder-v2")
    OLLAMA_BASE_URL     Ollama server URL (default: "http://localhost:11434")
    OPENROUTER_API_KEY  OpenRouter API key (for cloud escalation)
    OPENROUTER_MODEL    OpenRouter model (default: "anthropic/claude-sonnet-4-20250514")
    GOOGLE_API_KEY      Google Gemini API key (for cloud escalation)
    GEMINI_MODEL        Gemini model name (default: "gemini-2.5-pro")
    LLM_CLOUD_PROVIDER  "openrouter" or "gemini" (default: "openrouter")
"""

import os
import sys
import json
import re
import subprocess
from typing import Optional, List, Dict
from dotenv import load_dotenv

load_dotenv() # Load variables from .env if present

from langchain_openai import ChatOpenAI
from langchain_core.language_models.chat_models import BaseChatModel
from langchain_core.callbacks import StdOutCallbackHandler

def _log(msg: str):
    print(msg, flush=True)

# ---------------------------------------------------------------------------
# Project-specific prompt fragments (imported from gen_prompt.py)
# ---------------------------------------------------------------------------
_script_dir = os.path.dirname(os.path.abspath(__file__))
_tools_dir = os.path.dirname(_script_dir)
_root_dir = os.path.dirname(_tools_dir)
sys.path.insert(0, _tools_dir)

from gen_prompt import BACKGROUND, COMPILER_PATTERNS, CODE_RULES, TYPES_REFERENCE  # noqa: E402


SYSTEM_PROMPT = f"""\
You are a decompilation expert working on a GameCube matching decomp project.

{BACKGROUND}

{COMPILER_PATTERNS}

## Code Quality Rules
{CODE_RULES}

## Core Types
{TYPES_REFERENCE}

## Your Task
You receive:
1. The TARGET assembly for a function.
2. The current C code attempt.
3. Build/diff feedback showing where your code doesn't match.
4. (Optionally) Previous attempts and their results.
5. (Optionally) A Ghidra decompilation as a rough reference.
6. The current header file for context.
7. The compiled assembly resulting from your current C code attempt.

When suggesting `extern_declarations` or `header_additions`, assume that the relevant module's header (e.g., `game/rep_xxxx.h`) is already included where appropriate. You should only declare new items that are not already present in an included header.
If a symbol is already defined in a header that is included by the current source file, do NOT redeclare it. Focus on incremental updates to existing definitions if necessary.

RESPOND ONLY WITH A VALID JSON OBJECT. Do not include any text outside the JSON.
The JSON schema must be:
{{
  "explanation": "A SINGLE CONCISE PARAGRAPH summary of technical rationale and changes.",
  "function_code": "The complete C code for the function",
  "struct_modifications": [

    {{
      "type_name": "Name of the struct to modify",
      "actions": [
        {{
          "action": "add_field",
          "type": "C type",
          "name": "field_name",
          "offset": "0xXXX"
        }}
      ]
    }}
  ],
  "header_additions": "New prototypes or structs to ADD to the header",
  "extern_declarations": "Extern variables to ADD"
  }}

  STRUCT MODIFICATION RULES:

- If a struct needs a new field, DO NOT try to calculate padding manually.
- Instead, use the `struct_modifications` section to specify the field and its offset.
- Our specialized tool will handle the padding and alignment for you.
- ONLY specify fields that are missing or need renaming.
- NEVER specify modifications for fields that ALREADY EXIST in the struct at the requested offset. Use the existing field name exactly as defined.
- Fields MUST exist in the provided codebase context or be added via header_additions.
- Hex offsets MUST be absolute (e.g., 0x479).

CRITICAL RULES:
- The code must compile with Metrowerks CodeWarrior GC/1.3.2.
- `function_code` must contain ONLY the single requested function.
- Do NOT include #include directives in `function_code`.
- NEVER output `?` as a type. Replace ALL `?` with real C types.
- NEVER include assembly instructions or comments in `function_code`.
- Declare any new symbols in `extern_declarations` or `header_additions`.
- **Return type changes**: If the assembly shows the function should return a non-void type but the header declares it as `void`, you MUST include the updated declaration in `header_additions`. Example: if `fn_3_XXXX` should return `SomeType*`, add `SomeType* fn_3_XXXX(void);` to `header_additions`. Without this the build will fail with "illegal return value in void function".
# Read build errors and diff feedback carefully to guide your fixes.
"""

DEEP_SYSTEM_PROMPT = f"""\
You are a senior decompilation engineer. You are performing a DEEP analysis on a difficult function that has not matched after several attempts.

{BACKGROUND}

{COMPILER_PATTERNS}

## Deep Reasoning Strategy
1. **Analyze the Control Flow**: Look for mismatches in loop structures or branch destinations.
2. **Examine the Stack Frame**: Look for mismatches in local variable offsets (sp-relative). This often indicates the wrong order of variable declarations or incorrect sizes for local structs.
3. **Register Allocation**: Look for mismatches in register selection. This can sometimes be fixed by:
- Changing the order of local variable declarations.
- Using `volatile` to prevent optimization.
- Explicitly casting intermediate results (e.g., to `(u32)` or `(s16)`).
4. **Logic Mismatch**: Ensure all sign-extensions and masks (rlwinm) are correctly represented in C.

## Your Task
Analyze the assembly and the history of failed attempts. Identify the root cause of the mismatch and propose a fix.

YOUR RESPONSE MUST FOLLOW THIS PROTOCOL:
1. Provide a brief analysis and reasoning trace in Markdown.
2. End your response with EXACTLY ONE JSON block containing your patch instructions.

The JSON schema must be:
{{
"explanation": "A SINGLE CONCISE PARAGRAPH summary of technical rationale and changes.",
"function_code": "The complete C code for the function",
"struct_modifications": [

{{
  "type_name": "Name of the struct to modify",
  "actions": [
    {{
      "action": "add_field",
      "type": "C type",
      "name": "field_name",
      "offset": "0xXXX"
    }}
  ]
}}
],
"header_additions": "New prototypes or structs to ADD to the header",
"extern_declarations": "Extern variables to ADD"
}}

JSON block must be enclosed in ```json fences.

"""



def get_local_llm(json_mode: bool = True) -> BaseChatModel:
    """Create an OpenAI-compatible chat model for local inference (LM Studio/Ollama)."""
    
    # 1. Get the model name. 
    model = os.environ.get("LOCAL_LLM_MODEL", "deepseek-coder-v2")
    
    # 2. Get the Base URL. 
    base_url = os.environ.get("LOCAL_LLM_BASE_URL", "http://localhost:11434/v1")

    kwargs = {"temperature": 0.1, "callbacks": [StdOutCallbackHandler()]}
    # Some local servers don't support json_object, so we use text and rely on our parser
    if json_mode:
        kwargs["model_kwargs"] = {"response_format": {"type": "text"}}

    return ChatOpenAI(
        model=model,
        base_url=base_url,
        api_key="lm-studio",
        **kwargs
    )


def get_cloud_llm(tier: str = "fast", json_mode: bool = True) -> BaseChatModel:
    """Create a cloud LLM for attempts (fast cloud or deep cloud)."""
    provider = os.environ.get("LLM_CLOUD_PROVIDER", "gemini").lower()

    _log(f"DEBUG: LLM_CLOUD_PROVIDER from env: {provider}")

    if provider == "gemini" or provider == "google":
        from langchain_google_genai import ChatGoogleGenerativeAI

        if tier == "deep":
            model = os.environ.get("GEMINI_MODEL_DEEP", "gemini-2.5-pro")
        else:
            model = os.environ.get("GEMINI_MODEL_FAST", "gemini-2.0-flash")

        # Clean "export " prefix if it exists (happens with some .env files)
        if model.startswith("export "):
            model = model.replace("export ", "").split("=")[-1].strip()
        
        _log(f"DEBUG: Gemini Model (tier={tier}): {model}")
        api_key = os.environ.get("GOOGLE_API_KEY")
        if not api_key:
            raise ValueError("GOOGLE_API_KEY environment variable is required for Gemini")

        return ChatGoogleGenerativeAI(
            model=model,
            google_api_key=api_key,
            temperature=0.1,
            callbacks=[StdOutCallbackHandler()]
        )
    else:
        # OpenRouter via OpenAI-compatible API
        from langchain_openrouter import ChatOpenRouter

        if tier == "deep":
            model = os.environ.get("OPENROUTER_MODEL_DEEP", "anthropic/claude-3.5-sonnet")
        else:
            model = os.environ.get("OPENROUTER_MODEL_FAST", "google/gemini-2.0-flash-001")

        # Clean "export " prefix
        if model.startswith("export "):
            model = model.replace("export ", "").split("=")[-1].strip()

        api_key = os.environ.get("OPENROUTER_API_KEY")
        if not api_key:
            raise ValueError("OPENROUTER_API_KEY environment variable is required for OpenRouter")

        kwargs = {"temperature": 0.1, "callbacks": [StdOutCallbackHandler()]}
        if json_mode:
            kwargs["model_kwargs"] = {"response_format": {"type": "json_object"}}
        _log(f"DEBUG: OpenRouter Model (tier={tier}): {model}")
        return ChatOpenRouter(
            model=model,
    #         openrouter_provider={
    #     "only": ["arcee-ai/prime"]
    # },
            openrouter_api_key=api_key,
            **kwargs
        )


def get_llm(tier: str, prefer_local: bool = False, json_mode: bool = True) -> BaseChatModel:
    """Get the appropriate LLM for the given tier (fast/deep)."""
    if tier == "fast" and prefer_local:
        return get_local_llm(json_mode)
    return get_cloud_llm(tier, json_mode)


# ---------------------------------------------------------------------------
# Key Learnings Summarization
# ---------------------------------------------------------------------------

def get_key_learnings_prompt() -> str:
    """Read key learnings from the project root if it exists."""
    learnings_path = os.path.join(_root_dir, "key_learnings.md")
    if os.path.exists(learnings_path):
        with open(learnings_path, "r", encoding="utf-8") as f:
            content = f.read().strip()
            if content:
                return f"\n\n## Key Learnings from Previous Runs\n{content}\nTake these lessons into account when generating code and structs."
    return ""


def summarize_key_learnings(state: dict) -> str:
    """Analyze attempt history and extract actionable architectural insights."""
    attempts = state.get("attempts", [])
    if not attempts:
        return state

    print("[summarizer] Analyzing execution history for key learnings...")
    
    # Format the attempt history
    history_text = ""
    for i, att in enumerate(attempts):
        score = att.get("match_percent", 0.0)
        err = att.get("build_error", "")
        # If there's a build error, score is effectively 0 for learning purposes
        if err:
            score = 0.0
        
        history_text += f"\n--- Attempt {i+1} ---\n"
        history_text += f"Score: {score:.1f}%\n"
        if err:
            history_text += f"Build Error:\n{err}\n"
        else:
            history_text += f"Diff Feedback length: {len(att.get('feedback', ''))} chars\n"
        # Truncate C code to prevent massive context blows
        code = att.get("c_code", "")
        if len(code) > 1000:
            code = code[:500] + "... [truncated] ..." + code[-500:]
        history_text += f"Code Snippet:\n{code}\n"
        updates = att.get("struct_updates", [])
        if updates:
            history_text += f"Struct Updates: {json.dumps(updates)}\n"

    prompt = f"""You are an expert compiler analyst and reverse engineer.
You just finished an automated decompilation run for a GameCube function targeting Metrowerks CodeWarrior GC/1.3.2.

Here is the history of all code attempts, struct updates, and their resulting scores or compiler errors.

{history_text}

Analyze this history and extract ONLY SUBSTANTIAL, GENERALIZABLE LEARNINNGS.
For example:
- Did you learn something about how padding or `_pad` fields must be handled?
- Did you learn how certain macros (e.g. `g_d_GameSettings`) behave in this codebase?
- Did you learn a strict compiler rule or quirky C89 typing requirement that caused build failures?

DO NOT mention specific addresses, function names (unless it's a global macro/util), or minor syntax typos.
DO NOT summarize the function's logic.
ONLY output bullet points of rules and project-specific architectural traits that you should remember for completely different files.

If there are no substantial new learnings to generalize, output exactly the word "NONE".

Otherwise, output a concise markdown list of your new findings."""

    # We use JSON mode if enabled, but for summarization plain text is much better.
    # However, OpenRouter sometimes enforces JSON if we told it to, so we override it for the summarizer.
    llm = get_llm(state.get("llm_tier", "cloud"), json_mode=False)
    
    from langchain_core.messages import SystemMessage
    try:
        response = llm.invoke([SystemMessage(content=prompt)])
        content = str(response.content).strip()
        print(f"[summarizer] LLM response: {content[:100]}...")
        
        if content and content.upper() != "NONE":
            learnings_path = os.path.join(_root_dir, "key_learnings.md")
            print(f"[summarizer] Appending {len(content)} chars of new learnings to key_learnings.md")
            with open(learnings_path, "a", encoding="utf-8") as f:
                f.write(f"\n### Learnings from {state.get('function_name', 'run')}\n{content}\n")
        else:
            print("[summarizer] No substantial learnings found.")
    except Exception as e:
        print(f"[summarizer] Failed to extract learnings: {e}")

    return state


# ---------------------------------------------------------------------------
# Symbol Resolution — find existing definitions in the codebase
# ---------------------------------------------------------------------------

def _ensure_tags():
    """Ensure ctags index is up-to-date for include/ and src/."""
    tags_file = os.path.join(_root_dir, "tags")
    
    # Create .ctags to exclude noise if not present
    dot_ctags = os.path.join(_root_dir, ".ctags")
    if not os.path.exists(dot_ctags):
        with open(dot_ctags, "w") as f:
            f.write("--exclude=build\n")
            f.write("--exclude=.git\n")
            f.write("--exclude=logs\n")
            f.write("--exclude=tmp\n")
            f.write("--exclude=*.o\n")
            f.write("--exclude=*.s\n")
    
    # Always regenerate (takes ~130ms) to pick up new additions to headers/source
    try:
        subprocess.run(
            ["ctags", "-R", "--languages=C", "include", "src"],
            cwd=_root_dir, check=True, capture_output=True
        )
    except Exception as e:
        # If it fails, we'll try to use the existing tags file if it exists
        if not os.path.exists(tags_file):
            print(f"[tags] Failed to run ctags and no index found: {e}")
            return None
    return tags_file

def _read_tags(symbol: str) -> List[Dict]:
    """Search for a symbol in the ctags index using readtags binary."""
    tags_file = _ensure_tags()
    if not tags_file:
        return []
    
    try:
        # readtags -t tags -e -Q '(eq? $name "symbol")'
        result = subprocess.run(
            ["readtags", "-t", tags_file, "-e", "-Q", f"(eq? $name \"{symbol}\")"],
            capture_output=True, text=True, timeout=5
        )
        if result.returncode != 0:
            return []
            
        matches = []
        for line in result.stdout.splitlines():
            parts = line.split("\t")
            if len(parts) >= 3:
                matches.append({
                    "name": parts[0],
                    "file": os.path.join(_root_dir, parts[1]),
                    "pattern": parts[2].strip("/^").strip("$/"),
                    "kind": parts[3] if len(parts) > 3 else "unknown"
                })
        return matches
    except Exception:
        return []

def resolve_symbol_definitions(state: dict) -> str:
    """Find existing definitions for symbols referenced in M2C output and ASM.

    Searches include/ and src/ directories for:
    - extern declarations (variables and functions)
    - static variable declarations in the source file
    - struct/typedef definitions for types used in those declarations
    - function prototypes for called functions

    Returns a formatted string with all found definitions.
    """
    import re

    include_dir = os.path.join(_root_dir, "include")
    src_dir = os.path.join(_root_dir, "src")

    # Also get the specific source file for this function
    source_path = state.get("source_path", "")
    if source_path and not os.path.isabs(source_path):
        source_path = os.path.join(_root_dir, source_path)

    # Gather all referenced symbols from M2C output, ASM, and current code
    symbols = set()
    types_to_resolve = set()

    # Extract from M2C output and state
    m2c = state.get("m2c_output", "")
    current = state.get("current_c_code", "")
    local_headers = state.get("local_headers", "")
    extern_state = state.get("externs", "")
    asm = state.get("asm_text", "")
    
    combined_text = m2c + "\n" + current + "\n" + local_headers + "\n" + extern_state

    # Find extern references: "extern TYPE name;" patterns
    for match in re.finditer(r'extern\s+([\w\s\*]+?)\s+(\w+)\s*[;\[]', combined_text):
        type_name = match.group(1).strip()
        var_name = match.group(2).strip()
        symbols.add(var_name)
        # Extract base type (strip pointers, const, etc.)
        base_type = re.sub(r'[\*\s]', '', type_name)
        base_type = base_type.replace('const', '').replace('volatile', '').strip()
        # Only add valid identifier names (skip '?' etc)
        if base_type and re.match(r'^[a-zA-Z_]\w*$', base_type):
            if base_type not in ('void', 'int', 'char', 'short', 'long',
                               'u8', 'u16', 'u32', 'u64', 's8', 's16',
                               's32', 's64', 'f32', 'f64', 'BOOL'):
                types_to_resolve.add(base_type)

    # Find function calls: "bl FuncName" in ASM
    for match in re.finditer(r'bl\s+(\w+)', asm):
        func_name = match.group(1)
        if not func_name.startswith('.'):  # skip local labels
            symbols.add(func_name)

    # Find global references: "lbl_3_*" and "g_*" from ASM comments/labels
    for match in re.finditer(r'\b(lbl_\w+|g_\w+)\b', asm):
        symbols.add(match.group(1))

    # Find symbol references in M2C/code: variable.field patterns
    for match in re.finditer(r'\b(\w+)\s*\.\s*\w+', combined_text):
        symbols.add(match.group(1))

    if not symbols:
        return ""

    # Helper: run grep across both include/ and src/ (or a specific file)
    def _grep_symbol(sym, pattern=None, file_extensions=None, search_paths=None):
        """Grep for a symbol across include and src directories."""
        # Try readtags first (Phase 1.2)
        tag_matches = _read_tags(sym)
        if tag_matches:
            results = []
            for m in tag_matches:
                # Filter by file extension if requested
                if file_extensions:
                    ext = os.path.splitext(m['file'])[1]
                    if not any(f"*{ext}" == fe for fe in file_extensions):
                        continue
                results.append(f"{m['file']}: {m['pattern']}")
            if results:
                return results

        if pattern is None:
            pattern = sym
        if file_extensions is None:
            file_extensions = ["*.h", "*.c"]
        if search_paths is None:
            search_paths = [include_dir, src_dir]

        results = []
        for search_path in search_paths:
            if not os.path.exists(search_path):
                continue
            try:
                # Use -w for word boundaries to avoid partial matches
                cmd = ["grep", "-rnw"]
                for ext in file_extensions:
                    cmd.extend(["--include", ext])
                cmd.extend([pattern, search_path])
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=5)
                if result.stdout:
                    results.extend(result.stdout.strip().splitlines())
            except Exception:
                pass
        return results

    # First, search the source file specifically for static declarations
    source_defs = []
    if source_path and os.path.exists(source_path):
        try:
            with open(source_path, "r") as f:
                source_content = f.read()
            for sym in symbols:
                # Find static/global declarations of this symbol
                for line_match in re.finditer(
                    rf'^.*\b{re.escape(sym)}\b.*;\s*$', source_content, re.MULTILINE
                ):
                    line = line_match.group(0).strip()
                    # Only include actual declarations (static, extern, or type declarations)
                    if any(kw in line for kw in ['static ', 'extern ', 'struct ', 'void ', 'int ',
                                                   'u8 ', 'u16 ', 'u32 ', 's8 ', 's16 ', 's32 ',
                                                   'f32 ', 'f64 ', 'BOOL ']):
                        source_defs.append((source_path, line))
                        # Extract type from static declarations too
                        static_match = re.search(
                            rf'(?:static\s+)?(?:struct\s+)?(\w+)\s*\*?\s*{re.escape(sym)}',
                            line
                        )
                        if static_match:
                            stype = static_match.group(1)
                            # Only add valid identifier names
                            if stype and re.match(r'^[a-zA-Z_]\w*$', stype):
                                if stype not in ('static', 'extern', 'void', 'int', 'char',
                                               'u8', 'u16', 'u32', 'u64', 's8', 's16',
                                               's32', 's64', 'f32', 'f64', 'BOOL'):
                                    types_to_resolve.add(stype)
                        break  # Only first declaration per symbol
        except Exception:
            pass

    # Track which symbols are found
    resolved_symbols = set()
    found_files = {}  # symbol -> file it was found in

    for sym in symbols:
        lines = _grep_symbol(sym, file_extensions=["*.h"])
        if lines:
            resolved_symbols.add(sym)
            for line in lines:
                if ':' in line:
                    filepath = line.split(':')[0]
                    found_files[sym] = filepath
                    # Check if this line declares the symbol as extern
                    if 'extern' in line and sym in line:
                        ext_match = re.search(
                            rf'extern\s+([\w\s\*]+?)\s+{re.escape(sym)}\s*[;\[]',
                            line
                        )
                        if ext_match:
                            type_str = ext_match.group(1).strip()
                            base = re.sub(r'[\*\s]', '', type_str)
                            base = base.replace('const', '').replace('volatile', '').strip()
                            # Only add valid identifier names
                            if base and re.match(r'^[a-zA-Z_]\w*$', base):
                                if base not in ('void', 'int', 'char',
                                    'u8', 'u16', 'u32', 'u64', 's8', 's16',
                                    's32', 's64', 'f32', 'f64', 'BOOL'):
                                    types_to_resolve.add(base)
        else:
            # Try searching .c files too for non-static symbols
            lines = _grep_symbol(sym, file_extensions=["*.c"])
            if lines:
                for line in lines:
                    if 'static' not in line and sym in line:
                        resolved_symbols.add(sym)
                        break

    # For each type that needs resolution, extract its full struct definition
    resolved_defs = []  # (file, definition_text) pairs
    resolved_types = set()

    def _resolve_type(type_name, depth=0):
        if depth > 3 or type_name in resolved_types:
            return
        resolved_types.add(type_name)

        # 1. Search local_headers FIRST
        if local_headers:
            block = _extract_struct_block(None, type_name, content=local_headers)
            if block:
                resolved_defs.append(("[LOCAL STATE]", block))
                # Find nested types in this block
                for nested in re.finditer(r'\b([A-Z]\w+)\s+\w+', block):
                    nested_type = nested.group(1)
                    if nested_type not in ('typedef', 'struct', 'enum', 'union', 'extern', 'void', 'BOOL', 'Vec', 'Mtx', 'TRUE', 'FALSE', 'NULL'):
                        _resolve_type(nested_type, depth + 1)
                return

        # 2. Search both .h and .c files for struct definitions
        lines = _grep_symbol(type_name, pattern=f"struct _*{type_name}")
        if not lines:
            lines = _grep_symbol(type_name, pattern=f"typedef.*{type_name}")
        
        if lines:
            for line in lines:
                filepath = line.split(':')[0]
                if ('struct' in line or 'typedef' in line) and ('{' in line or 'struct' in line):
                    # Found the definition file -- extract the full block
                    block = _extract_struct_block(filepath, type_name)
                    if block:
                        resolved_defs.append((filepath, block))
                        # Find nested types in this block
                        for nested in re.finditer(
                            r'\b([A-Z]\w+)\s+\w+', block
                        ):
                            nested_type = nested.group(1)
                            if nested_type not in ('typedef', 'struct',
                                'enum', 'union', 'extern', 'void',
                                'BOOL', 'Vec', 'Mtx', 'TRUE', 'FALSE', 'NULL'):
                                _resolve_type(nested_type, depth + 1)
                    break

    for t in list(types_to_resolve):
        _resolve_type(t)

    # Also find function prototypes for called functions
    for sym in symbols:
        lines = _grep_symbol(sym, pattern=f"\\b{sym}\\b.*(", file_extensions=["*.h"])
        if lines:
            for line in lines:
                line_content = line.split(':', 2)[-1].strip() if ':' in line else line
                filepath = line.split(':')[0]
                if '(' in line_content and ';' in line_content and 'extern' not in line_content:
                    resolved_defs.append((filepath, line_content))
                    break

    # Also find extern declarations in headers
    for sym in symbols:
        lines = _grep_symbol(sym, pattern=f"extern.*{sym}", file_extensions=["*.h"])
        if lines:
            for line in lines:
                line_content = line.split(':', 2)[-1].strip() if ':' in line else line
                filepath = line.split(':')[0]
                if 'extern' in line_content:
                    resolved_defs.append((filepath, line_content))
                    break

    # Add source file definitions (these are critical context)
    resolved_defs.extend(source_defs)

    if not resolved_defs:
        return ""

    # Deduplicate
    seen = set()
    unique_defs = []
    for filepath, defn in resolved_defs:
        key = defn.strip()
        if key not in seen:
            seen.add(key)
            unique_defs.append((filepath, defn))

    # Create a mapping of found files for grouping
    by_file = {}
    for filepath, defn in unique_defs:
        rel = os.path.relpath(filepath, _root_dir)
        by_file.setdefault(rel, []).append(defn)

    # Format output
    sections = ["## Existing Definitions from Codebase\n"]
    sections.append(
        "The following definitions already exist in the codebase. "
        "You may ADD new fields to structs but NEVER remove existing fields.\n"
        "Do NOT redeclare variables that are already declared below — "
        "they already exist in the code.\n"
    )

    for rel_path, defs in by_file.items():
        sections.append(f"### From `{rel_path}`\n")
        sections.append("```c")
        for d in defs:
            sections.append(d)
        sections.append("```\n")

    # Add a section for types/symbols that could NOT be resolved
    unknown_types = types_to_resolve - resolved_types
    unknown_symbols = symbols - resolved_symbols
    
    if unknown_types or unknown_symbols:
        sections.append("## Unknown Types/Symbols (You MUST define these)\n")
        sections.append(
            "The following were referenced but NO definition was found in the codebase. "
            "You MUST provide a definition or extern declaration for these if you use them.\n"
        )
        if unknown_types:
            sections.append("### Unknown Types (Define in [HEADER] or [UPDATES])")
            for t in sorted(list(unknown_types)):
                sections.append(f"- `{t}`")
        if unknown_symbols:
            sections.append("\n### Unknown Symbols (Declare in [EXTERNS] or provide in [FUNCTION])")
            for s in sorted(list(unknown_symbols)):
                # Skip the function itself
                if s == state.get("func_name"):
                    continue
                sections.append(f"- `{s}`")
        sections.append("\n")

    return "\n".join(sections)


def _extract_struct_block(filepath, type_name, content=None):
    """Extract a full struct/typedef block from a file or string by type name."""
    if content is None:
        try:
            with open(filepath, "r") as f:
                content = f.read()
        except Exception:
            return None

    import re

    # Find the struct/typedef line
    pattern = rf'(?:typedef\s+)?(?:struct|enum|union)\s+_?{re.escape(type_name)}\b'
    match = re.search(pattern, content)
    if not match:
        return None

    start = match.start()
    # Find the opening brace
    brace_pos = content.find('{', start)
    if brace_pos == -1:
        return None

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

    # Find the semicolon after the closing brace (and optional type name)
    semi = content.find(';', end)
    if semi != -1 and semi - end < 50:  # reasonable distance
        end = semi + 1
    else:
        end += 1

    return content[start:end].strip()

def build_refactor_prompt(state: dict, escalate: bool = False) -> str:
    """Build the user-facing prompt for the refactorer from current state."""
    sections = []
    func_name = state["function_name"]
    iteration = state.get("iterations", 0)

    sections.append(f"# Refactor Attempt for `{func_name}` (Iteration {iteration + 1})\n")

    if escalate:
        sections.append(
            "## Deep Reasoning Escalation\n"
            "Progress has stalled or a regression was detected. You must perform a deep analysis:\n"
            "- **Stack Frame**: Re-examine local variable ordering and alignment padding.\n"
            "- **Register Allocation**: Consider if changing variable declaration order or using `volatile` would match the target's register selection.\n"
            "- **Loop Structure**: Re-evaluate `for` vs `while` vs `do-while` and potential unrolling or compiler-generated induction variables.\n"
            "- **Logic Mismatch**: Ensure all sign-extensions and casts match the assembly instructions (e.g., `extsb`, `rlwinm`).\n"
        )

    # Target assembly
    sections.append(f"## Target Assembly for `{func_name}`\n")
    sections.append(f"```asm\n{state.get('asm_text', '').strip()}\n```\n")

    # Ghidra reference (if available)
    ghidra = state.get("ghidra_output", "")
    if ghidra:
        sections.append("## Ghidra Decompilation (rough reference)\n")
        sections.append(f"```c\n{ghidra.strip()}\n```\n")

    # M2C output (initial skeleton)
    m2c = state.get("m2c_output", "")
    if m2c and state.get("iterations", 0) <= 1:
        sections.append("## M2C Decompilation (initial skeleton)\n")
        sections.append(f"```c\n{m2c.strip()}\n```\n")

    # Header context
    header = state.get("header_content", "")
    if header:
        sections.append(f"## Header File (`{state.get('header_path', '')}`)\n")
        sections.append(f"```c\n{header.strip()}\n```\n")

    # Existing definitions from codebase (structs, externs, prototypes)
    symbol_defs = resolve_symbol_definitions(state)
    if symbol_defs:
        sections.append(symbol_defs)

    # Symbol context (size/type/array hints for referenced lbl_ symbols)
    symbol_context = state.get("symbol_context", "")
    if symbol_context:
        sections.append(symbol_context + "\n")

    # SDA Mapping (r13/r2)
    sda_map = state.get("sda_map", "")
    if sda_map:
        sections.append("## SDA Mapping (r13/r2 bases)\n")
        sections.append(f"```\n{sda_map.strip()}\n```\n")

    # Previous attempts with feedback
    attempts = state.get("attempts", [])
    if attempts:
        sections.append("## Previous Attempts\n")
        for i, attempt in enumerate(attempts):
            pct = attempt.get("match_percent", 0)
            sections.append(f"### Attempt {i + 1} ({pct:.1f}% match)\n")
            
            explanation = attempt.get("explanation", "")
            if explanation:
                sections.append(f"**Rationale:** {explanation.strip()}\n")

            updates = attempt.get("struct_updates", [])
            if updates:
                sections.append("**Struct Updates Tried:**\n")
                # Handle old format (typedef) or new format (modification actions)
                if updates and isinstance(updates[0], dict) and 'actions' in updates[0]:
                    sections.append(f"Applied modifications: {json.dumps(updates, indent=2)}\n")
                else:
                    for upd in updates:
                        sections.append(f"@@@ {upd['type_name']}\n{upd['definition']}\n@@@ END\n")

            header_adds = attempt.get("header_additions", "")
            if header_adds:
                sections.append(f"**Header Additions Tried:**\n```c\n{header_adds.strip()}\n```\n")

            extern_adds = attempt.get("extern_declarations", "")
            if extern_adds:
                sections.append(f"**Extern Declarations Tried:**\n```c\n{extern_adds.strip()}\n```\n")

            sections.append(f"**Function Code Tried:**\n```c\n{attempt.get('c_code', '').strip()}\n```\n")

            build_err = attempt.get("build_error", "")
            if build_err:
                sections.append(f"**Build Error:**\n```\n{build_err.strip()}\n```\n")

            fb = attempt.get("feedback", "")
            if fb:
                sections.append(f"**Diff Feedback:**\n```\n{fb.strip()}\n```\n")

            cur_asm = attempt.get("current_asm", "")
            if cur_asm:
                sections.append(f"**Compiled Assembly:**\n```asm\n{cur_asm.strip()}\n```\n")

    # Current code + latest feedback
    current = state.get("current_c_code", "")
    if current:
        sections.append("## Current Code\n")
        sections.append(f"```c\n{current.strip()}\n```\n")

    feedback = state.get("feedback", "")
    if feedback:
        sections.append("## Latest Diff Feedback\n")
        sections.append(f"```\n{feedback.strip()}\n```\n")

    current_asm = state.get("current_asm", "")
    if current_asm:
        sections.append("## Current Compiled Assembly\n")
        sections.append(f"```asm\n{current_asm.strip()}\n```\n")

    build_log = state.get("build_log", "")
    if build_log:
        sections.append("## Latest Build Errors\n")
        sections.append(f"```\n{build_log.strip()}\n```\n")

    # Error Taxonomy / Classification
    taxonomy = state.get("error_taxonomy", "")
    if taxonomy:
        sections.append(f"## Failure Classification: {taxonomy}\n")
        # Find the hint in the messages if possible, or just state the category
        sections.append(f"Your previous attempt failed due to a **{taxonomy}** issue. Focus your reasoning on resolving this specific category of mismatch.\n")

    sections.append(
        "\n## Instruction\n"
        f"Analyze the assembly and previous feedback to matching C code."
        "\nRespond in JSON format as specified in the system prompt."
    )

    return "\n".join(sections)


def invoke_refactor(state: dict, escalate_after: int = 5, escalate: bool = False, prefer_local: bool = False) -> str:
    """Call the appropriate LLM to refactor the C code.

    Returns (result_dict, tier, user_prompt, raw_response)
    """
    iteration = state.get("iterations", 0)
    tier = "fast"
    if escalate or iteration >= escalate_after:
        tier = "deep"

    llm = get_llm(tier, prefer_local=prefer_local)
    user_prompt = build_refactor_prompt(state, escalate=(tier == "deep"))

    from langchain_core.messages import SystemMessage, HumanMessage

    if state.get("debug"):
        print(f"[LLM] {'='*60}")
        print(f"[LLM] USER PROMPT ({len(user_prompt)} chars):")
        print(f"[LLM] {'='*60}")
        print(user_prompt)
        print(f"[LLM] {'='*60}")

    display_tier = f"{tier} (local)" if tier == "fast" and prefer_local else tier
    print(f"[LLM] Invoking {display_tier} LLM (escalate={escalate})...", flush=True)

    system_prompt = DEEP_SYSTEM_PROMPT if tier == "deep" else SYSTEM_PROMPT
    messages = [
        SystemMessage(content=system_prompt + get_key_learnings_prompt()),
        HumanMessage(content=user_prompt),
    ]

    full_response = ""
    try:
        print(f"[LLM] Requesting stream...", flush=True)
        # We use stream so we can see the output as it comes
        stream_iter = llm.stream(messages)
        print(f"[LLM] Stream iterator obtained. Waiting for first chunk...", flush=True)
        for chunk in stream_iter:
            content = chunk.content
            full_response += content
            print(content, end="", flush=True)
        print("\n[LLM] Stream complete.", flush=True)
    except Exception as e:
        print(f"\n[LLM] Exception during streaming: {type(e).__name__}: {e}", flush=True)
        raise e

    sys.stdout.flush()
    raw = full_response.strip()

    # Parse structured response into sections
    result = parse_llm_response(raw)

    return result, tier, user_prompt, raw


def parse_llm_response(raw: str) -> dict:
    """Parse the LLM's structured response into sections.

    Returns dict with keys: 'function', 'updates', 'header', 'externs', 'explanation'
    'updates' is a list of struct modification actions.
    """
    data = {}
    try:
        # Priority 1: Extract JSON from the last fenced block (standard Markdown+JSON mode)
        json_blocks = re.findall(r'```(?:json)?\s*(\{.*?\})\s*```', raw, re.DOTALL)
        if json_blocks:
            try:
                data = json.loads(json_blocks[-1])
            except json.JSONDecodeError:
                pass
        
        # Priority 2: Try to parse the whole string as JSON (Strict JSON mode)
        if not data:
            try:
                data = json.loads(raw)
            except json.JSONDecodeError:
                pass
                
        # Priority 3: Looser regex search for any JSON-like block
        if not data:
            match = re.search(r'(\{.*\})', raw, re.DOTALL)
            if match:
                try:
                    data = json.loads(match.group(1))
                except json.JSONDecodeError:
                    pass
                    
        # If all JSON parsing failed, handle manually or return error
        if not data:
             return {"function": raw, "updates": [], "header": "", "externs": "", "explanation": "Failed parse"}

    except Exception as e:
        print(f"[llm] Fatal error during response parsing: {e}")
        return {"function": raw, "updates": [], "header": "", "externs": "", "explanation": f"Parse Error: {e}"}

    result = {
        "function": clean_code_block(data.get("function_code", "")),
        "updates": data.get("struct_modifications", []),
        "header": clean_code_block(data.get("header_additions", "")),
        "externs": clean_code_block(data.get("extern_declarations", "")),
        "explanation": data.get("explanation", "")
    }

    return result


def clean_code_block(text: str) -> str:
    """Clean a code block: strip fences, whitespace, and filter out natural language."""
    text = text.strip()
    # Remove markdown code fences if present (robust version)
    if text.startswith("```"):
        lines = text.split("\n")
        if lines[-1].strip() == "```":
            lines = lines[1:-1]
        elif lines[0].strip().startswith("```"):
            lines = lines[1:]
        text = "\n".join(lines).strip()
    
    text = text.replace("```", "").strip()

    no_change_phrases = [
        "no new", "no changes", "none needed", "no additional",
        "no extern", "no header", "not needed", "nothing to add",
        "no modifications", "n/a", "no updates", "already defined", "already declared",
        "New prototypes or structs to ADD to the header",
        "Extern variables to ADD",
        "The complete C code for the function",
        "Name of the struct to modify",
        "Brief summary of changes",
        "C type", "field_name", "0xXXX"
    ]
    lower = text.lower().strip()

    for phrase in no_change_phrases:
        if phrase in lower:
            has_code = any(c in text for c in ['{', '}', ';', '#define', '#include'])
            if not has_code:
                return ""

    if lower and not any(c in text for c in ['{', '}', ';', '#', '(']):
        if len(text) < 150 and text.count('\n') < 3:
            return ""

    return text
