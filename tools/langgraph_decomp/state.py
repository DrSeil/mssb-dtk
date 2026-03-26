"""State schema for the LangGraph decompilation loop."""

from typing import Annotated, TypedDict, List, Optional
from langgraph.graph.message import add_messages
from .llm import clean_code_block

def merge_struct_updates(old: list, new: list) -> list:
    """Merge struct updates by type_name, latest wins."""
    if not old: old = []
    if not new: return old
    # Create a dict keyed by type_name to handle updates
    updates_dict = {}
    for u in old:
        if u.get('type_name') == "Name of the struct to modify":
            continue
        updates_dict[u['type_name']] = u
        
    for u in new:
        if u.get('type_name') == "Name of the struct to modify":
            continue
        updates_dict[u['type_name']] = u
    return list(updates_dict.values())

import re

def _split_into_declaration_blocks(text: str):
    """Split a header text into logical declaration blocks.

    Uses brace-depth tracking so that multi-line typedefs / struct definitions
    are kept as a single block rather than being shredded line-by-line.
    Returns a list of (key, block_text) pairs where *key* is used for
    deduplication (latest wins during merge).
    """
    blocks = []
    pending = []
    depth = 0

    for line in text.splitlines():
        stripped = line.strip()
        if not stripped or stripped.startswith('#') or stripped.startswith('//'):
            # Preserve blank lines / comments only when inside a block
            if pending:
                pending.append(line)
            continue

        pending.append(line)
        depth += stripped.count('{') - stripped.count('}')

        if depth <= 0:
            depth = 0
            block = '\n'.join(pending).strip()
            pending = []
            if not block:
                continue

            # Derive a deduplication key that distinguishes typedefs from externs
            # for the same symbol name so they don't accidentally clobber each other.
            key = None

            # typedef / struct / enum / union ending with "} TypeName;"
            m = re.search(r'\}\s*(\w+)\s*;$', block, re.MULTILINE)
            if m:
                key = f"typedef_{m.group(1)}"

            if key is None:
                # extern declaration: "extern Type name;" or "extern Type name[];"
                m = re.match(r'extern\s+[\w\s\*]+?\s+(\w+)\s*[\[;]', block)
                if m:
                    key = f"extern_{m.group(1)}"

            if key is None:
                # function prototype: "RetType name(...);"
                m = re.search(r'\b(\w+)\s*\([^)]*\)\s*;$', block, re.MULTILINE)
                if m:
                    key = f"proto_{m.group(1)}"

            if key is None:
                # Unrecognised — use the whole block text as key (exact dedup only)
                key = block

            blocks.append((key, block))

    # Flush any unterminated block (e.g. missing closing brace)
    if pending:
        block = '\n'.join(pending).strip()
        if block:
            blocks.append((block, block))

    return blocks


def merge_unique_symbols(old: str, new: str) -> str:
    """Merge two strings of header declarations/definitions.

    Multi-line blocks (typedef struct, typedef enum, …) are kept intact via
    brace-depth tracking before merging, so they are never shredded line-by-line.
    For each symbol key the *latest* (new) definition wins.  Typedef blocks are
    emitted before extern/prototype lines to satisfy forward-declaration ordering.
    """
    old = clean_code_block(old)
    new = clean_code_block(new)

    if not new: return old
    if not old: return new

    # Skip garbage placeholder lines that some LLMs emit
    def _scrub(text):
        return '\n'.join(
            l for l in text.splitlines()
            if '?' not in re.sub(r'/\*.*?\*/', '', l)
        )
    old = _scrub(old)
    new = _scrub(new)

    old_blocks = _split_into_declaration_blocks(old)
    new_blocks = _split_into_declaration_blocks(new)

    # Ordered dict: key -> block_text, later entries overwrite earlier ones
    merged_order = []
    merged_map = {}

    for key, block in old_blocks + new_blocks:
        if key not in merged_map:
            merged_order.append(key)
        merged_map[key] = block

    # Emit typedef/struct/enum/union blocks first, then everything else,
    # so that forward-declaration ordering is satisfied.
    typedef_blocks = []
    other_blocks = []
    for key in merged_order:
        block = merged_map[key]
        if re.match(r'\s*(?:typedef|struct|enum|union)\b', block):
            typedef_blocks.append(block)
        else:
            other_blocks.append(block)

    # Prune orphaned typedef blocks: if a typedef defines TypeName but TypeName
    # is not referenced in any extern/prototype in the result, it was superseded
    # by a newer typedef with a different name and is no longer needed.
    other_text = '\n'.join(other_blocks)
    surviving_typedefs = []
    for block in typedef_blocks:
        m = re.search(r'\}\s*(\w+)\s*;$', block, re.MULTILINE)
        if m:
            type_name = m.group(1)
            # Keep only if type_name appears in a TYPE position (followed by
            # whitespace + another identifier) in the extern/prototype blocks.
            # This avoids false positives where the symbol is used only as a
            # *variable* name (e.g. `extern NewType lbl_3_data_228;`).
            if re.search(r'\b' + re.escape(type_name) + r'\b\s+\w', other_text):
                surviving_typedefs.append(block)
            # else: orphaned (type no longer referenced) — silently drop it
        else:
            surviving_typedefs.append(block)

    return '\n'.join(surviving_typedefs + other_blocks)


class AttemptRecord(TypedDict):
    """One iteration's code + result, so the LLM can see its history."""
    c_code: str           # The C code that was tried
    match_percent: float  # Score achieved (0.0 – 100.0)
    mismatch_count: int   # Number of differing instructions
    feedback: str         # Instruction-level diff from objdiff
    current_asm: str      # Compiled assembly for this attempt
    build_error: str      # Compiler errors (empty string on success)
    struct_updates: Optional[List[dict]] # Any struct updates tried in this attempt


class DecompState(TypedDict):
    """Shared state flowing through the decompilation graph."""

    # --- Target identification ---
    function_name: str          # e.g. "fn_3_104A3C"
    module: str                 # e.g. "game", "menus", ""
    source_file: str            # e.g. "game/rep_3090.c"
    asm_path: str               # path to .s file
    asm_text: str               # raw assembly text
    unit_name: str              # objdiff unit name
    header_path: str            # include/game/rep_3090.h
    header_content: str         # current header file content
    original_branch: str        # the branch we started from

    # --- Code being iterated ---
    current_c_code: str         # latest C code attempt
    externs: Annotated[str, merge_unique_symbols]                # extern declarations
    local_headers: Annotated[str, merge_unique_symbols]          # local header additions

    # --- Reference material ---
    m2c_output: str             # initial M2C skeleton
    ghidra_output: str          # Ghidra decompilation reference

    # --- Build/diff feedback ---
    feedback: str               # latest text diff from feedback_diff.py
    build_log: str              # compiler output on failure
    match_percent: float        # 0.0 – 100.0
    mismatch_count: int         # latest number of mismatched instructions
    prev_match_percent: float   # match percent from previous iteration
    prev_mismatch_count: int    # mismatch count from previous iteration

    # --- History ---
    attempts: List[AttemptRecord]  # full history of prior attempts
    messages: Annotated[list, add_messages]  # LLM conversation history

    # --- Control ---
    iterations: int             # loop counter
    status: str                 # "running" | "matched" | "failed" | "error"
    llm_tier: str               # "fast" | "deep"
    prefer_local: bool          # if True, use local model for "fast" tier
    escalate: bool              # True if escalation is triggered (stagnant or regression)
    debug: bool                 # True for verbose logging
    verbose: bool               # True for extra detail

    # --- Attempt-specific details (for logging/prompting) ---
    explanation: str            # latest LLM explanation of changes
    last_prompt: str            # the full prompt sent to the LLM
    last_raw_response: str      # the unparsed LLM response
    current_asm: str            # latest compiled assembly for current C code
    struct_updates: Annotated[List[dict], merge_struct_updates]  # list of {'type_name': str, 'definition': str}
    sda_map: str                # info about SDA-relative symbols (r13/r2)
    symbol_context: str         # annotated symbol size/type/array hints from symbols.txt
    error_taxonomy: str         # classification of current failure (e.g. "SDA Access")
