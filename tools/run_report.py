#!/usr/bin/env python3
import subprocess
import sys
import re
import argparse
import os


def get_functions():
    """
    Retrieve up to 5 unmatched functions from report.json that belong to the
    'game' or 'menus' categories and have a discoverable source file.
    Uses tools/score_functions.py to get a list of candidate functions, then
    validates each by checking if decomp.sh can locate its source file.
    """
    funcs = []
    for module in ('game', 'menus'):
        result = subprocess.run(
            ['python3', 'tools/score_functions.py', '--module', module, '--limit', '10'],
            capture_output=True, text=True, check=True,
        )
        lines = result.stdout.splitlines()
        header_found = False
        for line in lines:
            if line.startswith('Function'):
                header_found = True
                continue
            if not header_found:
                continue
            # Skip separator lines
            if set(line.strip()) == {'-'}:
                continue
            # Expected format: Function Name   Size   Match%   Category   Unit
            parts = line.split()
            if len(parts) < 5:
                continue
            func_name = parts[0]
            # Verify that the source file can be located via decomp.sh
            src_res = subprocess.run(['./decomp.sh', func_name], capture_output=True, text=True)
            for src_line in src_res.stdout.splitlines():
                if src_line.startswith('Source:'):
                    funcs.append(func_name)
                    break
            if len(funcs) >= 5:
                break
        if len(funcs) >= 5:
            break
    return funcs


def run_decomp(func, auto_insert=False):
    """Run decomp.sh to get source path and decompiled C code.
    If auto_insert is True, replace or append the function in its source file."""
    # First, get source file path using decomp.sh (without m2c)
    src_res = subprocess.run(['./decomp.sh', func], capture_output=True, text=True)
    source_path = None
    for line in src_res.stdout.splitlines():
        if line.startswith('Source:'):
            # The path after "Source:" is relative to workspace
            source_path = line.split('Source:')[1].strip()
            break
    # Then, get decompiled code using m2c_helper directly
    decomp_res = subprocess.run(['python3', 'tools/m2c_helper.py', func], capture_output=True, text=True)
    # Filter out non-code lines from m2c output:
    # - "Processing file ..." debug lines from decompctx
    # - "/* Warning: ... */" diagnostic comments from m2c
    code_lines = [
        line for line in decomp_res.stdout.splitlines()
        if not line.startswith('Processing file ')
        and not line.startswith('/* Warning:')
    ]
    new_code = '\n'.join(code_lines).strip()
    if auto_insert and source_path:
        try:
            # decomp_helper.py returns paths from splits.txt (e.g. "game/rep_1E08.c")
            # but actual source files live under src/ (e.g. "src/game/rep_1E08.c")
            if not os.path.exists(source_path) and os.path.exists(os.path.join('src', source_path)):
                source_path = os.path.join('src', source_path)
            source_path = os.path.normpath(source_path)
            with open(source_path, 'r') as f:
                src = f.read()
            # Match the full function definition including return type and body.
            # (?:\w+[ \t\*]+)* matches the return type words (e.g. "void ", "static void ",
            # "struct Foo* ") on the same line before the function name.
            pattern = r'(?s)(?:\w+[ \t\*]+)*' + re.escape(func) + r'\b[^{]*\{.*?\}'
            if re.search(pattern, src):
                src_new = re.sub(pattern, new_code, src, count=1)
                action = "replaced"
            else:
                src_new = src.rstrip() + "\n\n" + new_code + "\n"
                action = "appended"
            with open(source_path, 'w') as f:
                f.write(src_new)
            return f"Auto-inserted ({action}) decompiled code into {source_path}"
        except Exception as e:
            return f"Error during auto-insert: {e}"
    else:
        return new_code


def run_feedback(func):
    """Run feedback_diff.py for the function."""
    result = subprocess.run(['python3', 'tools/feedback_diff.py', func], capture_output=True, text=True)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description="Run decomp, feedback, and optional auto-insert.")
    parser.add_argument('--auto-insert', action='store_true', help='Automatically insert decompiled code into source files.')
    args = parser.parse_args()
    functions = get_functions()
    report_parts = []
    for func in functions:
        feedback = run_feedback(func)
        # Skip functions that already match
        if 'MATCH!' in feedback:
            continue
        report_parts.append(f'=== Feedback for {func} ===\n{feedback}\n')
        # Perform decomp and optional insertion
        decomp_msg = run_decomp(func, auto_insert=args.auto_insert)
        report_parts.append(f'=== Decomp result for {func} ===\n{decomp_msg}\n')
    # Write combined report
    with open('report.txt', 'w') as f:
        f.write('\n'.join(report_parts))
    print('Report generated: report.txt')


if __name__ == '__main__':
    main()
