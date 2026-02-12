#!/usr/bin/env python3
"""Find the easiest unmatched functions to decompile, ranked by size.

Smaller functions are generally easier to match. This tool parses
report.json to find unmatched functions and sorts them by code size.

Usage:
    python3 tools/score_functions.py
    python3 tools/score_functions.py --module game
    python3 tools/score_functions.py --max-size 200
    python3 tools/score_functions.py --min-size 8 --max-size 100 --limit 20
"""

import json
import argparse
import os
import sys


def main():
    parser = argparse.ArgumentParser(
        description="Find easiest unmatched functions to decompile."
    )
    parser.add_argument(
        "--report",
        default="report.json",
        help="Path to report.json (default: report.json)",
    )
    parser.add_argument(
        "--module",
        default=None,
        help="Filter by module/category (e.g., game, menus, challenge, dol)",
    )
    parser.add_argument(
        "--max-size",
        type=int,
        default=None,
        help="Maximum function size in bytes",
    )
    parser.add_argument(
        "--min-size",
        type=int,
        default=8,
        help="Minimum function size in bytes (default: 8, skips trivial stubs)",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=30,
        help="Number of results to show (default: 30)",
    )
    parser.add_argument(
        "--named-only",
        action="store_true",
        help="Only show functions with real names (not fn_XXXXXXXX)",
    )
    args = parser.parse_args()

    if not os.path.exists(args.report):
        print(
            f"Error: {args.report} not found. Run 'ninja report' to generate it.",
            file=sys.stderr,
        )
        sys.exit(1)

    with open(args.report, "r") as f:
        data = json.load(f)

    candidates = []

    for unit in data.get("units", []):
        unit_name = unit.get("name", "unknown")
        unit_measures = unit.get("measures", {})

        # Skip fully matched units
        if unit_measures.get("matched_code_percent", 0) == 100.0:
            continue

        # Determine category from unit name
        category = "dol"
        if unit_name.startswith("game/"):
            category = "game"
        elif unit_name.startswith("menus/"):
            category = "menus"
        elif unit_name.startswith("challenge/"):
            category = "challenge"

        if args.module and category != args.module:
            continue

        for func in unit.get("functions", []):
            size = int(func.get("size", 0))
            func_name = func.get("name", "")

            # Skip zero-size
            if size < args.min_size:
                continue

            if args.max_size and size > args.max_size:
                continue

            if args.named_only and func_name.startswith("fn_"):
                continue

            # Check if function is already matched
            func_measures = func.get("measures", {})
            if func_measures.get("matched_code_percent", 0) == 100.0:
                continue

            # If function has no measures, check unit-level match
            # (no measures typically means undecompiled)
            candidates.append(
                {
                    "name": func_name,
                    "size": size,
                    "unit": unit_name,
                    "category": category,
                    "match_pct": func_measures.get("matched_code_percent", 0),
                }
            )

    # Sort by size (smallest first), then by name
    candidates.sort(key=lambda x: (x["size"], x["name"]))

    if not candidates:
        print("No unmatched functions found matching criteria.")
        return

    # Display results
    shown = candidates[: args.limit]
    print(f"{'Function':<40} {'Size':>6} {'Match%':>7} {'Category':<10} Unit")
    print("-" * 100)
    for c in shown:
        print(
            f"{c['name']:<40} {c['size']:>6} {c['match_pct']:>6.1f}% {c['category']:<10} {c['unit']}"
        )

    total = len(candidates)
    if total > args.limit:
        print(f"\n... and {total - args.limit} more. Use --limit to see more.")
    print(f"\nTotal unmatched functions matching criteria: {total}")


if __name__ == "__main__":
    main()
