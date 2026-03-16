"""State schema for the LangGraph decompilation loop."""

from typing import Annotated, TypedDict, List, Optional
from langgraph.graph.message import add_messages

def merge_struct_updates(old: list, new: list) -> list:
    """Merge struct updates by type_name, latest wins."""
    if not old: old = []
    if not new: return old
    # Create a dict keyed by type_name to handle updates
    updates_dict = {u['type_name']: u for u in old}
    for u in new:
        updates_dict[u['type_name']] = u
    return list(updates_dict.values())

def merge_unique_lines(old: str, new: str) -> str:
    """Merge multi-line strings, keeping only unique lines."""
    if not old: old = ""
    if not new: return old
    old_lines = [l.strip() for l in old.splitlines() if l.strip()]
    new_lines = [l.strip() for l in new.splitlines() if l.strip()]
    
    seen = set(old_lines)
    result = list(old_lines)
    for line in new_lines:
        if line not in seen:
            result.append(line)
            seen.add(line)
    return "\n".join(result)


class AttemptRecord(TypedDict):
    """One iteration's code + result, so the LLM can see its history."""
    c_code: str           # The C code that was tried
    match_percent: float  # Score achieved (0.0 – 100.0)
    feedback: str         # Instruction-level diff from objdiff
    current_asm: str      # Compiled assembly for this attempt
    build_error: str      # Compiler errors (empty string on success)


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

    # --- Code being iterated ---
    current_c_code: str         # latest C code attempt
    externs: Annotated[str, merge_unique_lines]                # extern declarations
    local_headers: Annotated[str, merge_unique_lines]          # local header additions

    # --- Reference material ---
    m2c_output: str             # initial M2C skeleton
    ghidra_output: str          # Ghidra decompilation reference

    # --- Build/diff feedback ---
    feedback: str               # latest text diff from feedback_diff.py
    build_log: str              # compiler output on failure
    match_percent: float        # 0.0 – 100.0

    # --- History ---
    attempts: List[AttemptRecord]  # full history of prior attempts
    messages: Annotated[list, add_messages]  # LLM conversation history

    # --- Control ---
    iterations: int             # loop counter
    status: str                 # "running" | "matched" | "failed" | "error"
    llm_tier: str               # "local" | "cloud"
    debug: bool                 # True for verbose logging
    verbose: bool               # True for extra detail

    # --- Attempt-specific details (for logging/prompting) ---
    explanation: str            # latest LLM explanation of changes
    last_prompt: str            # the full prompt sent to the LLM
    last_raw_response: str      # the unparsed LLM response
    current_asm: str            # latest compiled assembly for current C code
    struct_updates: Annotated[List[dict], merge_struct_updates]  # list of {'type_name': str, 'definition': str}
