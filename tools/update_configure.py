import re
import argparse
import sys

# Regex to find the file header
FILE_HEADER_RE = re.compile(r"^([\w\d\/\._-]+\.c):")

def update_configure(args):
    print(f"Reading splits file: {args.splits_file}")
    source_files = []
    with open(args.splits_file, 'r') as f:
        for line in f:
            if match := FILE_HEADER_RE.match(line):
                source_files.append(match.group(1))

    if not source_files:
        sys.exit("Error: No source files found in splits file.")

    print(f"Found {len(source_files)} source files to add.")

    # Generate the new block of Python code for the REL
    object_lines = []
    for sf in sorted(source_files):
        object_lines.append(f'            Object(NonMatching, "{sf}"),')
    objects_list_str = "\n".join(object_lines)

    new_rel_block = f"""    Rel(
        \"{args.module_name}\",
        [\n{objects_list_str}\n        ]\n    ),"""

    print("Reading configure.py...")
    with open(args.configure_in, 'r') as f:
        lines = f.readlines()

    # Find the insertion point. We'll insert it at the end of the config.libs list.
    insertion_index = -1
    # Find the end of the game Rel, and insert after it.
    # This is a heuristic and might need adjustment if configure.py changes significantly.
    try:
        game_rel_index = next(i for i, line in enumerate(lines) if 'Rel("game"' in line)
        paren_count = 0
        for i in range(game_rel_index, len(lines)):
            if "(" in lines[i]:
                paren_count += lines[i].count("(")
            if ")" in lines[i]:
                paren_count -= lines[i].count(")")
            if paren_count == 0:
                insertion_index = i + 1
                break
    except StopIteration:
        insertion_index = -1

    if insertion_index == -1:
        # Fallback: find the end of the config.libs list
        for i, line in reversed(list(enumerate(lines))):
            if line.strip() == "]":
                for j in range(i - 1, 0, -1):
                    if "config.libs" in lines[j]:
                        insertion_index = i
                        break
                if insertion_index != -1:
                    break

    if insertion_index == -1:
        sys.exit("Error: Could not find a safe place to insert the new module configuration in configure.py.")

    print(f"Inserting new module configuration at line {insertion_index}")
    # Add a newline for spacing
    lines.insert(insertion_index, '\n' + new_rel_block + '\n')

    print(f"Writing new configuration to {args.configure_out}...")
    with open(args.configure_out, 'w') as f:
        f.writelines(lines)

    print("Done.")

def main():
    parser = argparse.ArgumentParser(description="Update configure.py with new source files from a splits file.")
    parser.add_argument("--splits-file", required=True, help="Path to the splits.txt file containing the new source files.")
    parser.add_argument("--configure-in", required=True, help="Path to the input configure.py file.")
    parser.add_argument("--configure-out", required=True, help="Path to write the new configure.py file.")
    parser.add_argument("--module-name", required=True, help="Name of the REL module (e.g., 'menus').")
    args = parser.parse_args()
    update_configure(args)

if __name__ == "__main__":
    main()
