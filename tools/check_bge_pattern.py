import json
import os
import re

def get_matched_functions(report_path):
    matched = set()
    with open(report_path, 'r') as f:
        data = json.load(f)
    for unit in data.get('units', []):
        for func in unit.get('functions', []):
            if func.get('fuzzy_match_percent') == 100.0:
                matched.add(func['name'])
    return matched

def find_pattern_in_s_files(root_dir):
    # Pattern: bge followed by b
    # Usually: bge .L_XXXX / b .L_YYYY / .L_XXXX:
    # We look for the instruction sequence.
    
    results = []
    # Regex for bge and b
    # bge is often written as bge, bge+, bge-
    # b is b, b+, b-
    bge_re = re.compile(r'^\s*/\* [0-9A-F ]+ \*/\s+bge[+-]?\s+(.L_[0-9A-F]+)', re.IGNORECASE)
    b_re = re.compile(r'^\s*/\* [0-9A-F ]+ \*/\s+b\s+(.L_[0-9A-F]+)', re.IGNORECASE)
    label_re = re.compile(r'^(.L_[0-9A-F]+):', re.IGNORECASE)
    func_re = re.compile(r'^\.fn\s+([^,]+)', re.IGNORECASE)

    for root, dirs, files in os.walk(root_dir):
        for file in files:
            if file.endswith('.s'):
                path = os.path.join(root, file)
                with open(path, 'r') as f:
                    lines = f.readlines()
                
                current_func = None
                for i in range(len(lines) - 2):
                    func_match = func_re.match(lines[i])
                    if func_match:
                        current_func = func_match.group(1)
                    
                    m1 = bge_re.match(lines[i])
                    if m1:
                        target_label = m1.group(1)
                        m2 = b_re.match(lines[i+1])
                        if m2:
                            # Check if the next line is the target label
                            m3 = label_re.match(lines[i+2])
                            if m3 and m3.group(1) == target_label:
                                results.append({
                                    'function': current_func,
                                    'file': path,
                                    'line': i + 1,
                                    'pattern': f"{lines[i].strip()} / {lines[i+1].strip()}"
                                })
    return results

def main():
    report_path = 'build/GYQE01/report.json'
    matched = get_matched_functions(report_path)
    
    patterns = find_pattern_in_s_files('build/GYQE01')
    
    print(f"Found {len(patterns)} instances of 'bge followed by b' pattern.")
    
    matched_patterns = [p for p in patterns if p['function'] in matched]
    
    print(f"Found {len(matched_patterns)} matched functions with this pattern:")
    for p in matched_patterns:
        print(f"Function: {p['function']}")
        print(f"  File: {p['file']}")
        print(f"  Line: {p['line']}")
        print(f"  Pattern: {p['pattern']}")
        print("-" * 20)

if __name__ == '__main__':
    main()
