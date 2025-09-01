import re
import argparse
from collections import defaultdict

# Regex to find the file header and section data
FILE_HEADER_RE = re.compile(r"^([\w\d\/._-]+\.c):")
SECTION_RE = re.compile(r"^\s*\.(text|rodata|data|bss)\s+start:(0x[\da-fA-F]+)\s+end:(0x[\da-fA-F]+)")

def validate_splits(file_path):
    """Parses a splits.txt file and checks for overlapping address ranges."""
    sections = defaultdict(list)
    current_file = "Unknown"
    
    try:
        with open(file_path, 'r') as f:
            for i, line in enumerate(f, 1):
                if header_match := FILE_HEADER_RE.match(line):
                    current_file = header_match.group(1)
                
                if section_match := SECTION_RE.search(line):
                    section, start_hex, end_hex = section_match.groups()
                    start_addr = int(start_hex, 16)
                    end_addr = int(end_hex, 16)
                    
                    if start_addr >= end_addr:
                        print(f"Warning on line {i}: Invalid range in {current_file} for .{section}. Start address 0x{start_addr:X} >= End address 0x{end_addr:X}")
                        continue

                    sections[f".{section}"].append({
                        'start': start_addr,
                        'end': end_addr,
                        'file': current_file,
                        'line': i
                    })
    except FileNotFoundError:
        print(f"Error: File not found at {file_path}")
        return

    print(f"--- Validating {file_path} ---")
    overlap_found = False
    for section_name, ranges in sections.items():
        if not ranges or len(ranges) < 2:
            continue
            
        # Sort by start address
        sorted_ranges = sorted(ranges, key=lambda r: r['start'])
        
        # Check for overlaps
        for i in range(1, len(sorted_ranges)):
            prev_range = sorted_ranges[i-1]
            curr_range = sorted_ranges[i]
            
            if curr_range['start'] < prev_range['end']:
                overlap_found = True
                print(f"\n!!! OVERLAP DETECTED in {section_name} !!!")
                print(f"  File A (line {prev_range['line']}): {prev_range['file']}")
                print(f"    Range: 0x{prev_range['start']:08X} - 0x{prev_range['end']:08X}")
                print(f"  File B (line {curr_range['line']}): {curr_range['file']}")
                print(f"    Range: 0x{curr_range['start']:08X} - 0x{curr_range['end']:08X}")
                print("-" * 35)

    if not overlap_found:
        print("\nValidation successful: No address overlaps found.")
    else:
        print("\nValidation failed: Overlaps were found.")

def main():
    parser = argparse.ArgumentParser(description="Validate a splits.txt file for overlapping address ranges.")
    parser.add_argument("file", help="Path to the splits.txt file to validate.")
    args = parser.parse_args()
    validate_splits(args.file)

if __name__ == "__main__":
    main()
