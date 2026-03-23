"""CLI entry point for the LangGraph decompilation loop.

Usage:
    python3 -m tools.langgraph_decomp.cli <function_name>
    python3 -m tools.langgraph_decomp.cli <function_name> --max-iterations 10
    python3 -m tools.langgraph_decomp.cli --auto --module game
    python3 -m tools.langgraph_decomp.cli <function_name> --human-in-loop --verbose
"""

import argparse
import json
import os
import shutil
import sys
import time
from dotenv import load_dotenv

# Ensure project root is in path
_script_dir = os.path.dirname(os.path.abspath(__file__))
_tools_dir = os.path.dirname(_script_dir)
_root_dir = os.path.dirname(_tools_dir)
sys.path.insert(0, _tools_dir)
sys.path.insert(0, _root_dir)

from tools.langgraph_decomp.graph import build_graph


def find_auto_target(module="game"):
    """Auto-select a small unmatched function with a stub in source."""
    report_path = os.path.join(_root_dir, "report.json")
    if not os.path.exists(report_path):
        print("Error: report.json not found. Run 'ninja report' first.",
              file=sys.stderr)
        sys.exit(1)

    with open(report_path) as f:
        data = json.load(f)

    candidates = []
    for unit in data.get("units", []):
        un = unit.get("name", "")
        if module and not un.startswith(f"{module}/"):
            continue

        for func in unit.get("functions", []):
            fm = func.get("measures", {})
            mp = fm.get("matched_code_percent", 0)
            sz = int(func.get("size", 0))

            if mp >= 100 or sz < 8:
                continue

            fname = func.get("name", "")
            # Check if source file has a stub
            parts = un.split("/")
            if len(parts) > 1:
                src = os.path.join(_root_dir, "src", "/".join(parts[1:]) + ".c")
                if os.path.exists(src):
                    with open(src) as sf:
                        if fname in sf.read():
                            candidates.append((fname, sz, un))

    candidates.sort(key=lambda x: x[1])

    if not candidates:
        print("No suitable unmatched functions found.", file=sys.stderr)
        sys.exit(1)

    return candidates[0][0]


def main():
    load_dotenv() # Load variables from .env if present
    parser = argparse.ArgumentParser(
        description="LangGraph Decompilation Loop — automated decomp with LLM refinement"
    )
    parser.add_argument(
        "function", nargs="?", default=None,
        help="Function name to decompile",
    )
    parser.add_argument(
        "--auto", action="store_true",
        help="Auto-select a function from score_functions",
    )
    parser.add_argument(
        "--module", default="game",
        help="Module to search in for --auto (default: game)",
    )
    parser.add_argument(
        "--max-iterations", type=int, default=15,
        help="Maximum refinement iterations (default: 15)",
    )
    parser.add_argument(
        "--escalate-after", type=int, default=5,
        help="Switch to deep cloud LLM after N iterations (default: 5)",
    )
    parser.add_argument(
        "--use-local", action="store_true",
        help="Use local LLM (Ollama) instead of fast cloud LLM for initial attempts",
    )
    parser.add_argument(
        "--human-in-loop", action="store_true",
        help="Pause before each refactorer call for manual inspection",
    )
    parser.add_argument(
        '--verbose', action='store_true',
        help='Show detailed output including LLM prompts',
    )
    parser.add_argument(
        '--debug', action='store_true',
        help='Show FULL debug output: complete LLM prompts, responses, build errors, and diffs',
    )
    parser.add_argument(
        '--clear-checkpoints', action='store_true',
        help='Clear the persistent LangGraph checkpoints before running',
    )
    args = parser.parse_args()

    if args.clear_checkpoints:
        checkpoints_dir = os.path.join(_root_dir, ".langgraph_checkpoints")
        if os.path.exists(checkpoints_dir):
            shutil.rmtree(checkpoints_dir)
            print("Cleared persistent checkpoints.")

    # Determine target function
    if args.function:
        func_name = args.function
    elif args.auto:
        func_name = find_auto_target(args.module)
        print(f"Auto-selected: {func_name}")
    else:
        parser.print_help()
        sys.exit(1)

    print(f"\n{'='*60}")
    print(f"LangGraph Decompilation Loop")
    print(f"{'='*60}")
    print(f"  Function:       {func_name}")
    print(f"  Max iterations: {args.max_iterations}")
    print(f"  Escalate after: {args.escalate_after}")
    print(f"  Human-in-loop:  {args.human_in_loop}")
    print(f"{'='*60}\n")

    # Build the graph
    debug_mode = args.debug or args.verbose
    graph, config = build_graph(
        max_iterations=args.max_iterations,
        escalate_after=args.escalate_after,
        human_in_loop=args.human_in_loop,
    )

    # Detect current branch
    original_branch = "main"
    try:
        import subprocess
        res = subprocess.run(["git", "branch", "--show-current"], capture_output=True, text=True, cwd=_root_dir)
        if res.returncode == 0 and res.stdout.strip():
            original_branch = res.stdout.strip()
    except:
        pass

    # Initial state
    initial_state = {
        "function_name": func_name,
        "module": "",
        "source_file": "",
        "asm_path": "",
        "asm_text": "",
        "unit_name": "",
        "header_path": "",
        "header_content": "",
        "original_branch": original_branch,
        "current_c_code": "",
        "externs": "",
        "local_headers": "",
        "struct_updates": [],
        "m2c_output": "",
        "ghidra_output": "",
        "feedback": "",
        "build_log": "",
        "match_percent": 0.0,
        "attempts": [],
        "messages": [],
        "iterations": 0,
        "status": "running",
        "llm_tier": "fast",
        "prefer_local": args.use_local,
        "escalate": False,
        "debug": args.debug,
        "verbose": args.verbose,
        "explanation": "",
        "last_prompt": "",
        "last_raw_response": "",
        "current_asm": "",
    }

    # Run the graph
    start_time = time.time()

    try:
        if args.human_in_loop:
            _run_with_interrupts(graph, initial_state, config, args.verbose, args.debug)
        else:
            _run_streaming(graph, initial_state, config, args.verbose, args.debug)
    except KeyboardInterrupt:
        print("\n\nInterrupted by user.")
    except Exception as e:
        print(f"\nFatal error: {e}")
        if args.verbose or args.debug:
            import traceback
            traceback.print_exc()

    elapsed = time.time() - start_time
    print(f"\nTotal time: {elapsed:.1f}s")


def _run_streaming(graph, initial_state, config, verbose, debug):
    """Run the graph and stream node updates."""
    for event in graph.stream(initial_state, config):
        for node_name, node_output in event.items():
            _print_node_update(node_name, node_output, verbose, debug)

    # Get final state
    final = graph.get_state(config)
    _print_final_summary(final.values)


def _run_with_interrupts(graph, initial_state, config, verbose, debug):
    """Run the graph with human-in-the-loop pauses before refactorer."""
    # Initial run until first interrupt
    for event in graph.stream(initial_state, config):
        for node_name, node_output in event.items():
            _print_node_update(node_name, node_output, verbose, debug)

    while True:
        state = graph.get_state(config)

        if not state.next:
            # Graph is done
            break

        if "refactorer" in state.next:
            print(f"\n{'─'*40}")
            print("PAUSED before refactorer.")
            print(f"Current match: {state.values.get('match_percent', 0):.1f}%")
            print(f"Iteration: {state.values.get('iterations', 0)}")
            print(f"{'─'*40}")

            resp = input("Continue? [y/n/code]: ").strip().lower()
            if resp == 'n':
                print("Stopped by user.")
                break
            elif resp == 'code':
                # Show current code
                print(f"\n{state.values.get('current_c_code', 'No code yet')}\n")
                resp = input("Continue? [y/n]: ").strip().lower()
                if resp == 'n':
                    break

        # Resume
        for event in graph.stream(None, config):
            for node_name, node_output in event.items():
                _print_node_update(node_name, node_output, verbose, debug)

    final = graph.get_state(config)
    _print_final_summary(final.values)


def _print_node_update(node_name, output, verbose, debug=False):
    """Print a concise update for each node execution."""
    # Guard against non-dict outputs (e.g. LangGraph interrupt events)
    if not isinstance(output, dict):
        print(f"   [{node_name}] (non-dict event: {type(output).__name__})")
        return

    status = output.get("status", "")
    match_pct = output.get("match_percent")
    tier = output.get("llm_tier", "")

    icon = {
        "source_finder": "🔍",
        "decompiler": "⚙️ ",
        "refactorer": "🤖",
        "builder": "🔨",
        "committer": "✅",
    }.get(node_name, "  ")

    parts = [f"{icon} [{node_name}]"]

    if match_pct is not None:
        parts.append(f"match={match_pct:.1f}%")
    if tier:
        parts.append(f"llm={tier}")
    if status:
        parts.append(f"status={status}")

    iteration = output.get("iterations")
    if iteration is not None:
        parts.append(f"iter={iteration}")

    print(" | ".join(parts))
    sys.stdout.flush()

    # Debug mode: show everything
    if debug:
        # Show current C code if this node produced it
        c_code = output.get("current_c_code", "")
        if c_code:
            print(f"\n   {'─'*50}")
            print(f"   C CODE:")
            print(f"   {'─'*50}")
            for line in c_code.strip().splitlines():
                print(f"   {line}")
            print(f"   {'─'*50}")

        # Show full feedback
        fb = output.get("feedback", "")
        if fb:
            print(f"\n   {'─'*50}")
            print(f"   OBJDIFF FEEDBACK:")
            print(f"   {'─'*50}")
            for line in fb.strip().splitlines():
                print(f"   {line}")
            print(f"   {'─'*50}")

        # Show full build errors
        build_log = output.get("build_log", "")
        if build_log:
            print(f"\n   {'─'*50}")
            print(f"   BUILD ERROR:")
            print(f"   {'─'*50}")
            for line in build_log.strip().splitlines():
                print(f"   {line}")
            print(f"   {'─'*50}")

        # Show M2C output
        m2c = output.get("m2c_output", "")
        if m2c:
            print(f"\n   {'─'*50}")
            print(f"   M2C OUTPUT:")
            print(f"   {'─'*50}")
            for line in m2c.strip().splitlines():
                print(f"   {line}")
            print(f"   {'─'*50}")

        # Show ASM
        asm = output.get("asm_text", "")
        if asm:
            print(f"\n   {'─'*50}")
            print(f"   TARGET ASM:")
            print(f"   {'─'*50}")
            for line in asm.strip().splitlines():
                print(f"   {line}")
            print(f"   {'─'*50}")

    elif verbose:
        # Verbose: show condensed versions
        fb = output.get("feedback", "")
        if fb and "MATCH!" not in fb:
            lines = fb.strip().splitlines()
            if len(lines) > 10:
                print(f"   Feedback ({len(lines)} lines):")
                for line in lines[:5]:
                    print(f"      {line}")
                print(f"      ... ({len(lines) - 5} more lines)")
            else:
                for line in lines:
                    print(f"   {line}")

        build_log = output.get("build_log", "")
        if build_log:
            print(f"   Build error:")
            for line in build_log.strip().splitlines()[:15]:
                print(f"      {line}")
            total_lines = len(build_log.strip().splitlines())
            if total_lines > 15:
                print(f"      ... ({total_lines - 15} more lines)")


def _print_final_summary(state):
    """Print the final result summary."""
    func = state.get("function_name", "?")
    status = state.get("status", "unknown")
    match_pct = state.get("match_percent", 0)
    iterations = state.get("iterations", 0)
    attempts = state.get("attempts", [])

    print(f"\n{'='*60}")
    print(f"RESULT: {func}")
    print(f"{'='*60}")
    print(f"  Status:     {status}")
    print(f"  Match:      {match_pct:.1f}%")
    print(f"  Iterations: {iterations}")

    if attempts:
        print(f"\n  Attempt History:")
        for i, a in enumerate(attempts):
            pct = a.get("match_percent", 0)
            err = "BUILD ERROR" if a.get("build_error") else ""
            print(f"    {i+1}. {pct:>5.1f}% {err}")

    if status == "matched":
        print(f"\n  🎉 Function matched and committed!")
    elif status == "failed" or iterations >= state.get("iterations", 15):
        print(f"\n  ⚠️  Max iterations reached. Best: "
              f"{max((a.get('match_percent', 0) for a in attempts), default=0):.1f}%")
    elif status == "error":
        print(f"\n  ❌ Error: {state.get('feedback', 'Unknown error')}")

    print(f"{'='*60}")


if __name__ == "__main__":
    main()
