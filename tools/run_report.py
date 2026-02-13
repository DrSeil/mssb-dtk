#!/usr/bin/env python3
import subprocess
import sys
import re

def get_functions():
    # Run score_functions.py with limit 5
    result = subprocess.run(['python3', 'tools/score_functions.py', '--limit', '5'], capture_output=True, text=True, check=True)
    lines = result.stdout.splitlines()
    funcs = []
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
        # Extract function name (first column)
        m = re.match(r'\s*(\S+)', line)
        if m:
            funcs.append(m.group(1))
        if len(funcs) >= 5:
            break
    return funcs

def run_decomp(func):
    # Run decomp.sh with m2c mode for the function
    subprocess.run(['./decomp.sh', '--m2c', func], check=False)

def run_feedback(func):
    # Run feedback_diff.py for the function
    result = subprocess.run(['python3', 'tools/feedback_diff.py', func], capture_output=True, text=True)
    return result.stdout

def main():
    functions = get_functions()
    report_parts = []
    for func in functions:
        run_decomp(func)
        feedback = run_feedback(func)
        report_parts.append(f'=== Feedback for {func} ===\n{feedback}\n')
    # Write combined report
    with open('report.txt', 'w') as f:
        f.write('\n'.join(report_parts))
    print('Report generated: report.txt')

if __name__ == '__main__':
    main()
