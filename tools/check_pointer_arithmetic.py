#!/usr/bin/env python3
"""Detect pointer arithmetic with casts that should be replaced with struct field access.

Scans C source files for patterns like:
  *(type*)((u8*)ptr + 0xOffset)
  *(type*)((char*)ptr + offset)

These should be replaced with proper struct definitions and field access.

Usage:
    python3 tools/check_pointer_arithmetic.py src/game/game_batter.c
    python3 tools/check_pointer_arithmetic.py src/game/
    python3 tools/check_pointer_arithmetic.py src/ --strict
"""

import argparse
import os
import re
import sys

# Patterns that indicate pointer arithmetic with manual offsets
PATTERNS = [
    # *(type*)((u8*)ptr + 0xNN)
    (
        r"\*\s*\(\s*\w+\s*\*\s*\)\s*\(\s*\(\s*(?:u8|s8|char|unsigned\s+char)\s*\*\s*\)",
        "Cast-based pointer arithmetic with byte offset",
    ),
    # (type*)((u8*)ptr + expr)
    (
        r"\(\s*\w+\s*\*\s*\)\s*\(\s*\(\s*(?:u8|s8|char|unsigned\s+char)\s*\*\s*\)\s*\w+\s*[+-]\s*(?:0x[\da-fA-F]+|\d+)",
        "Pointer cast with manual offset",
    ),
    # ptr + 0xNN where ptr is cast - more general
    (
        r"\(\s*(?:u8|s8|char|unsigned\s+char)\s*\*\s*\)\s*\w+\s*\+\s*0x[\da-fA-F]+",
        "Byte pointer with hex offset (likely struct field access)",
    ),
]


def check_file(filepath, strict=False):
    """Check a single C file for pointer arithmetic patterns.

    Returns list of (line_number, line_text, pattern_description) tuples.
    """
    violations = []

    try:
        with open(filepath, "r") as f:
            lines = f.readlines()
    except (IOError, UnicodeDecodeError):
        return violations

    for line_num, line in enumerate(lines, 1):
        stripped = line.strip()

        # Skip comments
        if stripped.startswith("//") or stripped.startswith("/*"):
            continue

        for pattern, description in PATTERNS:
            if re.search(pattern, stripped):
                violations.append((line_num, stripped, description))
                break  # One violation per line is enough

    return violations


def scan_path(path, strict=False):
    """Scan a file or directory for pointer arithmetic violations."""
    total_violations = 0
    files_with_violations = 0

    if os.path.isfile(path):
        files = [path]
    elif os.path.isdir(path):
        files = []
        for root, dirs, filenames in os.walk(path):
            for fn in filenames:
                if fn.endswith(".c") or fn.endswith(".h"):
                    files.append(os.path.join(root, fn))
    else:
        print(f"Error: {path} is not a file or directory", file=sys.stderr)
        return 1

    for filepath in sorted(files):
        violations = check_file(filepath, strict)
        if violations:
            files_with_violations += 1
            total_violations += len(violations)
            rel_path = os.path.relpath(filepath)
            print(f"\n{rel_path}:")
            for line_num, line_text, description in violations:
                print(f"  Line {line_num}: {description}")
                print(f"    {line_text}")

    if total_violations > 0:
        print(f"\n{'=' * 60}")
        print(
            f"Found {total_violations} violation(s) in {files_with_violations} file(s)."
        )
        print("Replace pointer arithmetic with proper struct field access.")
        if strict:
            return 1
    else:
        print("No pointer arithmetic violations found.")

    return 0


def main():
    parser = argparse.ArgumentParser(
        description="Detect pointer arithmetic that should use struct access."
    )
    parser.add_argument(
        "path", help="File or directory to check"
    )
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Exit with code 1 if violations are found",
    )
    args = parser.parse_args()

    sys.exit(scan_path(args.path, args.strict))


if __name__ == "__main__":
    main()
