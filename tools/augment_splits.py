import re
import os
import argparse
from collections import defaultdict
import sys

# Regex for parsing assembly files
TEXT_OBJ_RE = re.compile(r"# \.text:(0x[\da-fA-F]+) .* size: (0x[\da-fA-F]+)")
FN_DIRECTIVE_RE = re.compile(r"\.fn\s+([\w\d]+),\s+global")
GLOBAL_LABEL_RE = re.compile(r"\.obj\s+([\w\d]+),\s+global")

# Regex for parsing the input splits file
SECTION_LINE_RE = re.compile(r"^\s*(\.\w+)")


def parse_individual_rodata_file(rodata_s_path):
    labels = set()
    if not os.path.exists(rodata_s_path):
        print(f"Warning: Cannot find file {rodata_s_path}", file=sys.stderr)
        return labels
    with open(rodata_s_path, 'r') as f:
        for line in f:
            if label_match := GLOBAL_LABEL_RE.search(line):
                labels.add(label_match.group(1))
    return labels

def parse_text_functions(text_s_path):
    functions = []
    with open(text_s_path, 'r') as f:
        lines = f.readlines()
    i = 0
    while i < len(lines):
        line = lines[i]
        obj_match = TEXT_OBJ_RE.match(line)
        if obj_match and i + 1 < len(lines):
            fn_match = FN_DIRECTIVE_RE.match(lines[i+1])
            if fn_match:
                start_addr = int(obj_match.group(1), 16)
                size = int(obj_match.group(2), 16)
                name = fn_match.group(1)
                content_start_line = i + 2
                content_end_line = content_start_line
                for j in range(content_start_line, len(lines)):
                    if f".endfn {name}" in lines[j]:
                        content_end_line = j
                        break
                content = "".join(lines[content_start_line:content_end_line])
                functions.append({
                    "name": name,
                    "start": start_addr,
                    "end": start_addr + size,
                    "content": content
                })
                i = content_end_line + 1
                continue
        i += 1
    return functions

def augment_splits(args):
    print("Reading input splits file...")
    input_splits = []
    with open(args.splits_in, 'r') as f:
        for line in f:
            if line.strip().endswith(":"):
                input_splits.append({"header": line.strip(), "sections": []})
            elif input_splits and line.strip():
                input_splits[-1]["sections"].append(line.strip())

    print("Parsing rodata labels from individual files...")
    vfile_to_labels = defaultdict(set)
    for split_data in input_splits:
        header = split_data['header']
        if match := re.search(r"(rep_[\da-fA-F]+)\.c", header):
            base_name = match.group(1)
            s_file_path = os.path.join(args.rodata_dir, base_name + ".s")
            vfile_to_labels[header].update(parse_individual_rodata_file(s_file_path))

    print("Parsing text functions...")
    functions = parse_text_functions(args.text_s)
    if not functions:
        sys.exit(f"Error: No functions found in {args.text_s}.")

    print("Mapping functions to rodata usage...")
    func_to_vfiles = defaultdict(set)
    for func in functions:
        for vfile_header, labels in vfile_to_labels.items():
            if any(label in func['content'] for label in labels):
                func_to_vfiles[func['name']].add(vfile_header)

    print("Separating private and shared functions...")
    private_funcs = defaultdict(list)
    shared_funcs = []
    for func_name, vfiles in func_to_vfiles.items():
        func_data = next((f for f in functions if f['name'] == func_name), None)
        if func_data:
            if len(vfiles) == 1:
                private_funcs[list(vfiles)[0]].append(func_data)
            else:
                shared_funcs.append(func_data)

    print("Augmenting splits with text sections...")
    vfile_text_ranges = {}
    for header, funcs in private_funcs.items():
        if funcs:
            min_start = min(f['start'] for f in funcs)
            max_end = max(f['end'] for f in funcs)
            vfile_text_ranges[header] = (min_start, max_end)

    captured_shared_funcs = set()
    for func in shared_funcs:
        for start, end in vfile_text_ranges.values():
            if start <= func['start'] and func['end'] <= end:
                captured_shared_funcs.add(func['name'])

    orphaned_shared_funcs = [f for f in shared_funcs if f['name'] not in captured_shared_funcs]

    for vfile_data in input_splits:
        vfile_data["sections"] = [s for s in vfile_data["sections"] if not s.strip().startswith(".text")]
        if header := vfile_data.get("header"): 
            if text_range := vfile_text_ranges.get(header):
                min_start, max_end = text_range
                vfile_data["sections"].append(f".text       start:0x{min_start:08X} end:0x{max_end:08X}")

    if orphaned_shared_funcs:
        print("Creating individual splits for orphaned shared functions...")
        path_prefix = os.path.dirname(input_splits[0]["header"]) if input_splits and os.path.dirname(input_splits[0]["header"]) else "menus"
        for func in orphaned_shared_funcs:
            start, end = func['start'], func['end']
            # Create a unique name based on the address
            shared_header = f"{path_prefix}/shared/auto_{start:08X}.c:"
            shared_section = f".text       start:0x{start:08X} end:0x{end:08X}"
            input_splits.append({"header": shared_header, "sections": [shared_section]})

    print(f"Writing new splits to {args.output}...")
    with open(args.output, 'w') as f:
        for vfile_data in input_splits:
            f.write(vfile_data['header'] + '\n')
            def get_start_addr(section_line):
                match = re.search(r"start:(0x[\da-fA-F]+)", section_line)
                return int(match.group(1), 16) if match else -1
            for section_line in sorted(vfile_data['sections'], key=get_start_addr):
                f.write(f"\t{section_line}\n")
            f.write('\n')
    print("Done.")

def main():
    parser = argparse.ArgumentParser(description="Augment a splits.txt file with .text sections based on rodata usage.")
    parser.add_argument("--splits-in", required=True, help="Path to the input splits.txt file.")
    parser.add_argument("--rodata-dir", required=True, help="Path to the directory with individual .rodata.s files.")
    parser.add_argument("--text-s", required=True, help="Path to the .text.s assembly file.")
    parser.add_argument("--output", required=True, help="Path for the new, augmented splits.txt file.")
    args = parser.parse_args()
    augment_splits(args)

if __name__ == "__main__":
    main()
