import json
import os
import re

def get_matched_functions_with_source(report_path):
    matched = {} # name -> source_path
    with open(report_path, 'r') as f:
        data = json.load(f)
    for unit in data.get('units', []):
        source_path = unit.get('metadata', {}).get('source_path')
        for func in unit.get('functions', []):
            if func.get('fuzzy_match_percent') == 100.0:
                matched[func['name']] = source_path
    return matched

def find_pattern_in_s_files(root_dir):
    results = []
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
    matched = get_matched_functions_with_source(report_path)
    
    patterns = find_pattern_in_s_files('build/GYQE01')
    
    matched_patterns = [p for p in patterns if p['function'] in matched]
    
    # Filter for game module
    game_matched = [p for p in matched_patterns if matched[p['function']] and 'src/game/' in matched[p['function']]]
    
    print(f"Found {len(game_matched)} matched functions in 'game' module with this pattern:")
    for p in game_matched:
        print(f"Function: {p['function']}")
        print(f"  Source: {matched[p['function']]}")
        print(f"  Pattern: {p['pattern']}")
        print("-" * 20)

if __name__ == '__main__':
    main()
