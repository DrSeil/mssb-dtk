import os
import re

# Match standard prologue/epilogue and ONE branch (bl or b)
def is_simple_wrapper(lines):
    instructions = []
    for line in lines:
        line = line.strip()
        if not line or line.startswith(".") or line.startswith("#") or line.startswith("/*"):
            # Try to extract instruction from comment if it's there
            if "*/" in line:
                instr = line.split("*/")[1].strip()
                if instr:
                    instructions.append(instr)
            continue
        instructions.append(line)
    
    if not instructions:
        return False, None



    # Count branches
    branches = [i for i in instructions if i.startswith("bl ") or i.startswith("b ")]
    if len(branches) != 1:
        return False, None
    
    branch = branches[0]
    target = branch.split()[1]
    
    # Check if it's a tail call b (no frame) or a bl with a frame
    if branch.startswith("b "):
        # Simple tail call: only b <target> and maybe some arg setup
        # But for now let's just look at the exact pattern
        if len(instructions) == 1:
            return True, target
        return False, None
    
    # For bl, expect standard prologue/epilogue
    # Length should be around 8 instructions
    if len(instructions) == 8:
        # Check standard 16-byte frame pattern
        if (instructions[0].startswith("stwu r1,") and
            instructions[1] == "mflr r0" and
            instructions[2].startswith("stw r0,") and
            instructions[3].startswith("bl ") and
            instructions[4].startswith("lwz r0,") and
            instructions[5] == "mtlr r0" and
            instructions[6].startswith("addi r1,") and
            instructions[7] == "blr"):
            return True, target
            
    return False, None

def find_all_wrappers(asm_dir):
    results = []
    for root, dirs, files in os.walk(asm_dir):
        for file in files:
            if file.endswith(".s"):
                path = os.path.join(root, file)
                with open(path, "r") as f:
                    content = f.read()
                
                # Find all .fn ... .endfn blocks
                functions = re.findall(r"\.fn\s+(\S+),.*?\n(.*?)\.endfn", content, re.DOTALL)
                for func_name, body in functions:
                    func_name = func_name.rstrip(",")
                    lines = body.strip().split("\n")
                    simple, target = is_simple_wrapper(lines)
                    if simple:
                        results.append((func_name, target, file))
    return results

if __name__ == "__main__":
    # We can search in all of build/GYQE01/
    asm_path = "build/GYQE01/"
    wrappers = find_all_wrappers(asm_path)
    for func, target, file in sorted(wrappers):
        print(f"{func} calls {target} (in {file})")
