import json
import sys

def find_matched_functions(report_path):
    try:
        with open(report_path, 'r') as f:
            data = json.load(f)
        
        units = data.get('units', [])
        for unit in units:
            unit_name = unit.get('name', 'unknown')
            unit_measures = unit.get('measures', {})
            unit_match = unit_measures.get('matched_code_percent', 0)
            
            functions = unit.get('functions', [])
            for func in functions:
                func_measures = func.get('measures', {})
                func_match = func_measures.get('matched_code_percent', unit_match if not func_measures else 0)
                
                size = int(func.get('size', 0))
                if func_match == 100.0 and size > 32:
                    print(f"Unit: {unit_name}, Function: {func.get('name')}, Size: {size}")
            
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        find_matched_functions(sys.argv[1])
    else:
        print("Usage: python find_matches.py <report.json>")
