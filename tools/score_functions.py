import json
import argparse
import os
import sys
import re


def main():
    parser = argparse.ArgumentParser(
        description="Find easiest unmatched functions to decompile."
    )
    parser.add_argument(
        "--report",
        default="build/GYQE01/report.json",
        help="Path to report.json (default: build/GYQE01/report.json)",
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
        "--file",
        default=None,
        help="Filter by source file name substring (e.g., rep_1E08)",
    )
    parser.add_argument(
        "--named-only",
        action="store_true",
        help="Only show functions with real names (not fn_XXXXXXXX)",
    )
    parser.add_argument(
        "--unstarted",
        action="store_true",
        help="Only show functions that aren't in any .c file yet (auto-generated units)",
    )
    parser.add_argument(
        "--max-match",
        type=float,
        default=None,
        metavar="PCT",
        help="Only show functions with match%% at or below this value (e.g. 50 for <=50%%)",
    )
    parser.add_argument(
        "--sort",
        choices=["size", "match-desc", "match-asc"],
        default="size",
        help="Sort results by size, match percentage descending, or match percentage ascending (default: size)",
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

    # Load skip list
    skip_list = set()
    if os.path.exists("config/skip_list.txt"):
        with open("config/skip_list.txt", "r") as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("#"):
                    skip_list.add(line)

    # Load objdiff.json if it exists to help with source path detection
    objdiff_data = {}
    if os.path.exists("objdiff.json"):
        try:
            with open("objdiff.json", "r") as f:
                objdiff_data = json.load(f)
        except Exception:
            pass

    unit_to_source = {}
    for unit in objdiff_data.get("units", []):
        unit_to_source[unit.get("name")] = unit.get("metadata", {}).get("source_path")

    candidates = []

    for unit in data.get("units", []):
        unit_name = unit.get("name", "unknown")
        unit_measures = unit.get("measures", {})
        unit_metadata = unit.get("metadata", {})

        # Skip fully matched units
        if unit_measures.get("matched_code_percent", 0) == 100.0:
            continue

        # Determine category from unit name
        category = "dol"
        u_lower = unit_name.lower()
        if unit_name.startswith("game/") or "/game/" in unit_name or "_game_" in unit_name:
            category = "game"
        elif unit_name.startswith("menus/") or "/menus/" in unit_name or "_menus_" in unit_name:
            category = "menus"
        elif unit_name.startswith("challenge/") or "/challenge/" in unit_name or "_challenge_" in unit_name:
            category = "challenge"
        elif "game" in u_lower:
            category = "game"
        elif "menu" in u_lower:
            category = "menus"
        elif "challenge" in u_lower:
            category = "challenge"

        if args.module and category != args.module:
            continue

        if args.file and args.file not in unit_name:
            continue

        # Skip if already has a source file and user only wants unstarted
        if args.unstarted:
            # Check auto_generated flag first
            if not unit_metadata.get("auto_generated", False):
                # If not auto-generated, it might already be in splits and have a C file
                source_path = unit_to_source.get(unit_name)
                if source_path and os.path.exists(source_path):
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

            # Skip if in skip list
            if func_name in skip_list:
                continue

            # Get match percent (prioritize fuzzy_match_percent as the server does)
            match_pct = func.get("fuzzy_match_percent")
            if match_pct is None:
                func_measures = func.get("measures", {})
                match_pct = func_measures.get("matched_code_percent", 0.0)

            if match_pct is None:
                match_pct = 0.0

            # Skip if already 100% matched
            if match_pct >= 100.0:
                continue

            # Skip if above max-match threshold
            if args.max_match is not None and match_pct > args.max_match:
                continue

            candidates.append(
                {
                    "name": func_name,
                    "size": size,
                    "unit": unit_name,
                    "category": category,
                    "match_pct": match_pct,
                }
            )

    if args.sort == "match-desc":
        candidates.sort(key=lambda x: (-x["match_pct"], x["size"], x["name"]))
    elif args.sort == "match-asc":
        candidates.sort(key=lambda x: (x["match_pct"], x["size"], x["name"]))
    else:
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
        # Avoid rounding 99.99% to 100.0%
        pct_str = f"{c['match_pct']:>6.1f}%"
        if c['match_pct'] < 100.0 and pct_str.strip() == "100.0%":
            pct_str = " 99.9%*"
            
        print(
            f"{c['name']:<40} {c['size']:>6} {pct_str} {c['category']:<10} {c['unit']}"
        )

    total = len(candidates)
    if total > args.limit:
        print(f"\n... and {total - args.limit} more. Use --limit to see more.")
    print(f"\nTotal unmatched functions matching criteria: {total}")


if __name__ == "__main__":
    main()
