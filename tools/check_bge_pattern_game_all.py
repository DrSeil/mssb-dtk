import os
import re

def find_pattern_in_s_files(root_dir):
    results = []
    bge_re = re.compile(r'^\s*/\* [0-9A-F ]+ \*/\s+bge[+-]?\s+(.L_[0-9A-F]+)', re.IGNORECASE)
    b_re = re.compile(r'^\s*/\* [0-9A-F ]+ \*/\s+b\s+(.L_[0-9A-F]+)', re.IGNORECASE)
    label_re = re.compile(r'^(.L_[0-9A-F]+):', re.IGNORECASE)
    func_re = re.compile(r'^\.fn\s+([^,]+)', re.IGNORECASE)

    for root, dirs, files in os.walk(root_dir):
        if 'game' not in root.lower():
            continue
            
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
    patterns = find_pattern_in_s_files('build/GYQE01')
    
    print(f"Found {len(patterns)} functions in 'game' module with this pattern:")
    for p in patterns:
        print(f"Function: {p['function']}")
        print(f"  File: {p['file']}")
        print(f"  Pattern: {p['pattern']}")
        print("-" * 20)

if __name__ == '__main__':
    main()
