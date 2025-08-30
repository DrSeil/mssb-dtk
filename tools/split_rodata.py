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

def is_pattern_match(lines):
    """Checks if a list of lines matches the 20-float pattern."""
    if len(lines) < len(FLOAT_PATTERN):
        return False
    for i, pattern_line in enumerate(FLOAT_PATTERN):
        if pattern_line not in lines[i]:
            return False
    return True

def split_rodata(input_file, output_dir):
    """
    Splits a rodata.s file into multiple rep_XXX.s files based on a
    recurring 20-float pattern.
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(input_file, 'r') as f:
        lines = f.readlines()

    file_starts = []
    # Find all occurrences of the pattern and their preceding address
    for i, line in enumerate(lines):
        match = RODATA_START_RE.match(line)
        if match:
            # We expect the pattern to start 2 lines after the comment,
            # after the .obj line.
            # Example:
            # # .rodata:0x1A0 | 0x1A0 | size: 0x50  <-- line i
            # .obj lbl_2_rodata_1A0, global         <-- line i+1
            # .float 1                             <-- line i+2
            potential_pattern_lines = lines[i+2 : i+2+len(FLOAT_PATTERN)]
            if is_pattern_match(potential_pattern_lines):
                addr = match.group(1)[2:] # Get address without "0x"
                file_starts.append({"addr": addr, "start_line": i})

    if not file_starts:
        print("No rodata sections with the specified float pattern found.")
        return

    # Write the content for each identified file
    for i, start_info in enumerate(file_starts):
        start_line = start_info["start_line"]
        # The end line is the start of the next file, or EOF for the last file
        end_line = file_starts[i+1]["start_line"] if i + 1 < len(file_starts) else len(lines)
        
        content = "".join(lines[start_line:end_line])
        
        output_filename = f"rep_{start_info['addr'].zfill(4)}.s"
        output_path = os.path.join(output_dir, output_filename)
        
        with open(output_path, 'w') as f:
            f.write(content)
        print(f"Wrote {output_path}")


def main():
    parser = argparse.ArgumentParser(description="Split a rodata.s file based on a float pattern.")
    parser.add_argument("input_file", help="Path to the input rodata.s file.")
    parser.add_argument("output_dir", help="Directory to write the output rep_XXX.s files.")
    args = parser.parse_args()
    
    split_rodata(args.input_file, args.output_dir)

if __name__ == "__main__":
    main()
