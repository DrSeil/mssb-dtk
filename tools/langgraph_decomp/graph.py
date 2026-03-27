"""LangGraph StateGraph definition for the decompilation loop.

Graph topology:
    START → source_finder → decompiler → refactorer → builder → check_progress
                                              ↑                        |
                                              └────── (mismatch) ──────┘
                                                      (match) → committer → END
                                                      (max iter) → END
                                                      (error) → END
"""

from langgraph.graph import StateGraph, START, END
from langgraph.checkpoint.memory import MemorySaver

from .state import DecompState
from .nodes import (
    source_finder_node,
    decompiler_node,
    refactorer_node,
    builder_node,
    committer_node,
)


# Default safety cap — can be overridden
MAX_ITERATIONS = 15
# Default escalation threshold — can be overridden
ESCALATE_AFTER = 5


import re

def classifier_node(state: DecompState) -> dict:
    """Analyze build/diff output to classify the error and provide hints."""
    match_percent = state.get("match_percent", 0.0)
    build_log = state.get("build_log", "")
    feedback = state.get("feedback", "")
    
    classification = "Logic Mismatch"
    hint = ""
    
    if build_log:
        if "undefined identifier" in build_log or "function has no prototype" in build_log:
            classification = "Type Error"
            hint = "One or more identifiers are undefined or missing a prototype. Check for missing includes or extern declarations."
        elif "redeclared" in build_log:
            classification = "Type Error"
            hint = "Symbol redeclaration detected. Check if a prototype in your header conflicts with a definition."
        else:
            classification = "Syntax Error"
            hint = "The code failed to compile. Check for syntax errors or mismatched braces."
    elif feedback:
        # Analyze diff for specific patterns
        if "r13" in feedback or "r2" in feedback:
            classification = "SDA Access"
            hint = "Mismatch in SDA (r13/r2) access detected. Verify symbol addresses and ensure they are within SDA boundaries."
        elif "0x" in feedback and "(sp)" in feedback:
            classification = "Stack Frame"
            hint = "Mismatch in stack-relative offsets detected. Check local variable ordering and struct field sizes."
        elif any(op in feedback for op in ["extsb", "extsh", "rlwinm", "clrlwi"]):
            classification = "Register Allocation"
            hint = "Mismatch in sign-extension or bit-masking. Check your casts and intermediate variable types."

    print(f"[classifier] Category: {classification}")
    
    return {
        "error_taxonomy": classification,
        "messages": [("ai", f"Failure classified as: {classification}. Hint: {hint}")] if hint else []
    }

def check_progress(state: DecompState) -> str:
    """Conditional edge: decide whether to loop, commit, or stop."""
    if state.get("match_percent", 0) >= 100.0 or state.get("status") == "matched":
        return "committer"
    if state.get("iterations", 0) >= MAX_ITERATIONS:
        print(f"[graph] Max iterations ({MAX_ITERATIONS}) reached.")
        return END
    if state.get("status") == "error":
        print(f"[graph] Hard error detected, stopping.")
        return END
    
    # Otherwise, loop back for more refinement
    return "refactorer"


def build_graph(
    max_iterations: int = 15,
    escalate_after: int = 5,
    human_in_loop: bool = False,
) -> tuple:
    """Build and compile the decompilation StateGraph.

    Args:
        max_iterations: Safety cap on refinement loops.
        escalate_after: Switch from local to cloud LLM after this many iterations.
        human_in_loop: If True, interrupt before the refactorer node for manual inspection.

    Returns:
        (compiled_graph, config_dict) tuple.
    """
    global MAX_ITERATIONS, ESCALATE_AFTER
    MAX_ITERATIONS = max_iterations
    ESCALATE_AFTER = escalate_after

    # Create a refactorer wrapper that passes escalate_after
    def _refactorer(state):
        return refactorer_node(state, escalate_after=escalate_after)

    # Build the graph
    builder = StateGraph(DecompState)

    builder.add_node("source_finder", source_finder_node)
    builder.add_node("decompiler", decompiler_node)
    builder.add_node("refactorer", _refactorer)
    builder.add_node("builder", builder_node)
    builder.add_node("classifier", classifier_node)
    builder.add_node("committer", committer_node)

    # Edges
    builder.add_edge(START, "source_finder")
    builder.add_edge("source_finder", "decompiler")
    builder.add_edge("decompiler", "refactorer")
    builder.add_edge("refactorer", "builder")
    builder.add_edge("builder", "classifier")

    # Conditional edge from classifier
    builder.add_conditional_edges("classifier", check_progress)

    builder.add_edge("committer", END)

    # Compile with checkpointer
    checkpointer = MemorySaver()

    compile_kwargs = {"checkpointer": checkpointer}
    if human_in_loop:
        compile_kwargs["interrupt_before"] = ["refactorer"]

    graph = builder.compile(**compile_kwargs)

    config = {
        "configurable": {"thread_id": "decomp-1"},
        "max_iterations": max_iterations,
        "escalate_after": escalate_after,
    }

    return graph, config
