import re
import os
import argparse

# The 20 floats that act as a delimiter
FLOAT_PATTERN = [
    ".float 1",
    ".float 1.5707964",
    ".float 1",
    ".float -1",
    ".float 4.712389",
    ".float 3.1415927",
    ".float -1",
    ".float 0",
    ".float -1",
    ".float 1",
    ".float 1",
    ".float 1.5707964",
    ".float 1",
    ".float -1",
    ".float 4.712389",
    ".float 3.1415927",
    ".float -1",
    ".float 0",
    ".float -1",
    ".float 1",
]

# Regex to find the address at the start of a rodata object
RODATA_START_RE = re.compile(r"# \.rodata:(0x[0-9a-fA-F]+)")
RODATA_OBJ_RE = re.compile(r"# \.rodata:(0x[0-9a-fA-F]+) .* size: (0x[0-9a-fA-F]+)")

def is_pattern_match(lines):
    """Checks if a list of lines matches the 20-float pattern."""
    if len(lines) < len(FLOAT_PATTERN):
        return False
    for i, pattern_line in enumerate(FLOAT_PATTERN):
        if pattern_line not in lines[i]:
            return False
    return True

def create_splits(input_file, base_addr, output_prefix, output_file=None):
    """
    Finds rodata sections based on a float pattern and prints splits.txt entries.
    """
    with open(input_file, 'r') as f:
        lines = f.readlines()

    split_starts = []
    # Find all occurrences of the pattern and their preceding address
    for i, line in enumerate(lines):
        match = RODATA_START_RE.match(line)
        if match:
            # Pattern starts 2 lines after comment, after the .obj line
            potential_pattern_lines = lines[i+2 : i+2+len(FLOAT_PATTERN)]
            if is_pattern_match(potential_pattern_lines):
                offset = int(match.group(1), 16)
                split_starts.append(offset)

    # Determine output stream
    if output_file:
        outfile = open(output_file, 'w')
    else:
        import sys
        outfile = sys.stdout

    if not split_starts:
        outfile.write(f"# No rodata sections with the specified float pattern found in {os.path.basename(input_file)}.\n")
        if output_file:
            outfile.close()
        return

    # Find the end of the entire rodata section to cap the last split
    last_obj_end = 0
    for line in lines:
        match = RODATA_OBJ_RE.match(line)
        if match:
            offset = int(match.group(1), 16)
            size = int(match.group(2), 16)
            if (offset + size) > last_obj_end:
                last_obj_end = offset + size
    
    total_rodata_size = last_obj_end

    # Process the splits
    split_offsets = sorted(split_starts)
    
    outfile.write(f"# Splits generated for {os.path.basename(input_file)}\n")
    for i in range(len(split_offsets)):
        start_offset = split_offsets[i]
        end_offset = split_offsets[i+1] if i + 1 < len(split_offsets) else total_rodata_size

        start_addr = base_addr + start_offset
        end_addr = base_addr + end_offset

        split_filename = f"{output_prefix}rep_{start_offset:04X}.c"

        outfile.write(f"\n{split_filename}:\n")
        outfile.write(f"\t.rodata     start:0x{start_addr:08X} end:0x{end_addr:08X}\n")

    if output_file:
        outfile.close()
        print(f"Splits successfully written to {output_file}")

def main():
    parser = argparse.ArgumentParser(description="Generate splits.txt entries for rodata sections.")
    parser.add_argument("input_file", help="Path to the input rodata.s file.")
    parser.add_argument("--base-addr", required=True, type=lambda x: int(x, 0),
                        help="Base address of the .rodata section (e.g., 0x800E82D8). Hex or decimal.")
    parser.add_argument("--output-prefix", default="menus/",
                        help="Prefix for the generated file paths in the splits (e.g., 'menus/').")
    parser.add_argument("-o", "--output-file", help="Optional: Path to the output file. If not provided, prints to console.")
    args = parser.parse_args()
    
    create_splits(args.input_file, args.base_addr, args.output_prefix, args.output_file)

if __name__ == "__main__":
    main()
