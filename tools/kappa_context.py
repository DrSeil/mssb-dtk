import subprocess
import sys
import os
import re

def get_function_asm(func_name):
    try:
        result = subprocess.run(["python3", "tools/decomp_helper.py", func_name], capture_output=True, text=True, check=True)
        lines = result.stdout.splitlines()
        asm_lines = []
        in_fn = False
        for line in lines:
            if line.startswith(f".fn {func_name}"):
                in_fn = True
                continue
            if line.startswith(f".endfn {func_name}"):
                in_fn = False
                break
            if in_fn:
                asm_lines.append(line)
        return asm_lines
    except subprocess.CalledProcessError as e:
        print(f"Error running decomp_helper: {e.stderr}", file=sys.stderr)
        return []

def extract_dependencies(asm_lines):
    deps = set()
    for line in asm_lines:
        match = re.search(r"bl\s+([a-zA-Z0-9_]+)", line)
        if match:
            deps.add(match.group(1))
    return sorted(list(deps))

def find_signature(symbol, include_dir):
    try:
        cmd = ["grep", "-rE", rf"\b{symbol}\s*\(", include_dir]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode == 0:
            lines = result.stdout.splitlines()
            for line in lines:
                if ";" in line or "{" in line:
                    return line.strip()
        return f"// Signature for {symbol} not found"
    except Exception as e:
        return f"// Error searching for {symbol}: {str(e)}"

def find_symbol_type(symbol, include_dir):
    """Find the declared type of a global symbol from extern declarations."""
    try:
        result = subprocess.run(
            ["grep", "-rh", "--include=*.h", "-E", rf"extern\s+.*\b{re.escape(symbol)}\b"],
            capture_output=True, text=True, cwd=include_dir
        )
        if result.returncode == 0 and result.stdout.strip():
            for line in result.stdout.splitlines():
                line = line.strip().rstrip(';')
                # Extract type: "extern TypeName sym" or "extern TypeName* sym" or "extern struct T sym"
                m = re.match(r"extern\s+(.*?)\s*\*?\s*\b" + re.escape(symbol) + r"\b", line)
                if m:
                    raw_type = m.group(1).strip().rstrip('*').strip()
                    # Strip leading "struct " keyword if present
                    raw_type = re.sub(r"^struct\s+", "", raw_type).strip()
                    return raw_type, line
    except Exception:
        pass
    return None, None

def find_struct_body(type_name, search_paths):
    """Find and return the struct definition body for a given type name."""
    for path in search_paths:
        try:
            with open(path, 'r', errors='replace') as f:
                content = f.read()
            lines = content.splitlines()
        except Exception:
            continue

        # Look for "struct TypeName {" or "} TypeName;" (typedef end) or "typedef struct ... TypeName {"
        start_line = None
        for i, line in enumerate(lines):
            if re.search(rf"\bstruct\s+{re.escape(type_name)}\s*\{{", line) or \
               re.search(rf"\btypedef\s+struct\s+.*\b{re.escape(type_name)}\s*\{{", line) or \
               re.search(rf"\btypedef\s+struct\s+_{re.escape(type_name)}\s*\{{", line):
                start_line = i
                break

        if start_line is None:
            # Try "typedef struct { ... } TypeName;"
            for i, line in enumerate(lines):
                if re.search(rf"\}}\s*{re.escape(type_name)}\s*;", line):
                    # Find the matching opening brace
                    for j in range(i, -1, -1):
                        if "typedef struct" in lines[j]:
                            start_line = j
                            break
                    break

        if start_line is not None:
            # Collect lines until matching closing brace
            body = []
            depth = 0
            for i in range(start_line, min(start_line + 120, len(lines))):
                body.append(lines[i])
                depth += lines[i].count('{') - lines[i].count('}')
                if depth <= 0 and i > start_line:
                    break
            return path, start_line + 1, "\n".join(body)

    return None, None, None

def find_callers(func_name, asm_dir):
    """Find all functions in the asm directory that call func_name via bl."""
    callers = []
    try:
        result = subprocess.run(
            ["grep", "-rl", f"bl {func_name}", asm_dir],
            capture_output=True, text=True
        )
        if result.returncode != 0 or not result.stdout.strip():
            return []

        for asm_file in result.stdout.splitlines():
            try:
                with open(asm_file, 'r', errors='replace') as f:
                    lines = f.readlines()
            except Exception:
                continue

            current_fn = None
            for line in lines:
                fn_match = re.match(r"\.fn\s+(\S+)", line)
                if fn_match:
                    current_fn = fn_match.group(1)
                if current_fn and re.search(rf"\bbl\s+{re.escape(func_name)}\b", line):
                    callers.append(current_fn)

    except Exception:
        pass
    return sorted(set(callers))

def extract_access_patterns(asm_lines):
    """Extract all load/store offset accesses, grouped by base register."""
    # Map from register -> what global it was last loaded from
    reg_source = {}
    accesses = []  # (instruction, offset, base_reg, source_symbol)

    load_store_re = re.compile(
        r"(l[bhwf]z?u?x?|s[bhwf]x?u?|stfs?|lfs?|l[fd]s?u?|stfd?)\s+\w+,\s*(-?(?:0x[0-9a-fA-F]+|\d+))\((\w+)\)"
    )
    global_re = re.compile(r"(lbl_[0-9A-Fa-f_]+|g_[a-zA-Z0-9_]+)@[hl]")
    addi_re = re.compile(r"addi\s+(\w+),\s*(\w+),\s*([^\s]+)@[hl]")
    mulli_re = re.compile(r"mulli\s+\w+,\s*\w+,\s*(0x[0-9a-fA-F]+|\d+)")

    strides = []
    for line in asm_lines:
        # Track global address loads: "addi rN, rM, symbol@l" or "lis rN, symbol@ha"
        lis_match = re.search(r"lis\s+(\w+),\s*(lbl_[0-9A-Fa-f_]+|g_[a-zA-Z0-9_]+)@ha", line)
        if lis_match:
            reg_source[lis_match.group(1)] = lis_match.group(2)

        addi_match = re.search(r"addi\s+(\w+),\s*(\w+),\s*(lbl_[0-9A-Fa-f_]+|g_[a-zA-Z0-9_]+)@l", line)
        if addi_match:
            reg_source[addi_match.group(1)] = addi_match.group(3)

        # lwzu: "lwzu rN, symbol@l(rM)" — the base gets updated to rN
        lwzu_match = re.search(r"lwzu\s+(\w+),\s*(lbl_[0-9A-Fa-f_]+|g_[a-zA-Z0-9_]+)@l\((\w+)\)", line)
        if lwzu_match:
            reg_source[lwzu_match.group(3)] = lwzu_match.group(2)
            reg_source[lwzu_match.group(1)] = lwzu_match.group(2)

        # Track load-with-update of a dereffed pointer
        # "lwz rN, 0x0(rM)" where rM is a known global -> rN = *global
        lwz_match = re.search(r"lwz\s+(\w+),\s*(0x[0-9a-fA-F]+|\d+)\((\w+)\)", line)
        if lwz_match:
            dest, off, base = lwz_match.group(1), lwz_match.group(2), lwz_match.group(3)
            if off in ("0x0", "0") and base in reg_source:
                reg_source[dest] = f"*{reg_source[base]}"

        # mulli strides
        mulli_match = re.search(r"mulli\s+\w+,\s*\w+,\s*(0x[0-9a-fA-F]+|\d+)", line)
        if mulli_match:
            stride = mulli_match.group(1)
            strides.append(int(stride, 16) if stride.startswith("0x") else int(stride))

        # Load/store with offset
        ls_match = re.search(
            r"\b(l[bhw]zu?|lha|lf[sd]u?|st[bhw]u?|stf[sd]u?)\s+\w+,\s*(-?(?:0x[0-9a-fA-F]+|\d+))\((\w+)\)",
            line
        )
        if ls_match:
            instr, offset, base = ls_match.group(1), ls_match.group(2), ls_match.group(3)
            # Convert offset to int
            neg = offset.startswith('-')
            abs_off = offset.lstrip('-')
            off_int = int(abs_off, 16) if abs_off.startswith("0x") else int(abs_off)
            if neg:
                off_int = -off_int
            source = reg_source.get(base, base)
            accesses.append((instr, off_int, base, source))

    return accesses, strides

def summarize_accesses(accesses, strides):
    """Group accesses by source symbol and print a summary."""
    from collections import defaultdict
    grouped = defaultdict(list)
    for instr, off, base, source in accesses:
        grouped[source].append((instr, off))

    lines = []
    for source, items in sorted(grouped.items()):
        offsets = sorted(set(items), key=lambda x: x[1])
        offset_strs = [f"  0x{off:X} ({instr})" for instr, off in offsets]
        lines.append(f"  {source}:")
        lines.extend(offset_strs)

    if strides:
        stride_strs = [f"0x{s:X}" for s in sorted(set(strides))]
        lines.append(f"  [mulli strides: {', '.join(stride_strs)}]")

    return lines

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 kappa_context.py <function_name>")
        sys.exit(1)

    func_name = sys.argv[1]
    include_dir = "include"
    struct_search_paths = ["in_game.h"] + [
        os.path.join(root, f)
        for root, _, files in os.walk(include_dir)
        for f in files if f.endswith(".h")
    ]

    print(f"--- Kappa Context for {func_name} ---")

    asm_lines = get_function_asm(func_name)
    if not asm_lines:
        print("Could not retrieve ASM lines.")
        sys.exit(1)

    # --- Callers ---
    asm_dir = "build/GYQE01/game/asm"
    callers = find_callers(func_name, asm_dir)
    if callers:
        print(f"\nCallers ({len(callers)}):")
        for c in callers[:10]:
            print(f"  {c}")
        if len(callers) > 10:
            print(f"  ... and {len(callers) - 10} more")
    else:
        print("\nCallers: none found in game asm")

    # --- Dependencies ---
    deps = extract_dependencies(asm_lines)
    print(f"\nDependencies found: {len(deps)}")
    for dep in deps:
        sig = find_signature(dep, include_dir)
        print(f"  {sig}")

    # --- Symbols + struct definitions ---
    lbls = set()
    for line in asm_lines:
        match = re.search(r"\b(lbl_[a-zA-Z0-9_]+|g_[a-zA-Z0-9_]+)\b", line)
        if match:
            lbls.add(match.group(1))

    print(f"\nSymbols found: {len(lbls)}")
    seen_types = set()
    for lbl in sorted(list(lbls)):
        type_name, decl = find_symbol_type(lbl, include_dir)
        if decl:
            print(f"  {decl}")
        else:
            print(f"  {lbl}  // type unknown")

        # Look up struct definition for this type
        if type_name and type_name not in seen_types and type_name not in {"u8", "s8", "u16", "s16", "u32", "s32", "f32", "f64", "BOOL", "void"}:
            seen_types.add(type_name)
            path, lineno, body = find_struct_body(type_name, struct_search_paths)
            if body:
                src = os.path.relpath(path) if path else "?"
                print(f"\n  // Struct definition: {type_name} (from {src}:{lineno})")
                for struct_line in body.splitlines():
                    print(f"  {struct_line}")
                print()

    # --- Access pattern summary ---
    accesses, strides = extract_access_patterns(asm_lines)
    if accesses or strides:
        print("\nAccess patterns (offsets by base):")
        for line in summarize_accesses(accesses, strides):
            print(line)

    print("\n--- Assembly ---")
    for line in asm_lines:
        print(line)

if __name__ == "__main__":
    main()
