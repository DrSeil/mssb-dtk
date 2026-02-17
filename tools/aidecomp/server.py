
import subprocess
import json
import os
import re
from fastapi import FastAPI, Request, Query
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import Optional, List, Dict, Any
import sys

# Add tools to path for decomp_helper
sys.path.append("tools")
import decomp_helper

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Configuration Settings
CONFIG = {
    "max_rounds": 5,
    "build_cmd": "ninja",
    "objdiff_path": "/usr/bin/objdiff",
    "include_dir": "./include",
    "target_obj": "build/mssb/main.dol.o",
    "ghidra_path": "/path/to/ghidra/support/analyzeHeadless",
    "m2c_path": "python3 tools/m2c_repo/m2c.py",
    "externs_header": "include/UnknownHeaders.h",
    "prompt_template": "Refine the following C code based on the assembly:\n\n{asm_content}\n\nCurrent C:\n{c_code}\n\nContext:\n{context}\n\nErrors:\n{errors}"

}

REPORT_CACHE = None

def load_report():
    # Try multiple common report paths
    candidates = [
        "build/GYQE01/report.json",
        "report.json"
    ]
    
    report_path = None
    for cand in candidates:
        if os.path.exists(cand):
            report_path = cand
            break
            
    if not report_path:
        return None
        
    try:
        with open(report_path, "r") as f:
            return json.load(f)
    except Exception:
        return None


# ... (imports)

def find_unit_name(func_name):
    # 1. Try to find function in report.json to get unit
    report = load_report()
    unit_path = None
    if report:
        for unit in report.get("units", []):
            for func in unit.get("functions", []):
                if func.get("name") == func_name:
                    unit_path = unit.get("name")
                    break
            if unit_path:
                break
    return unit_path

def find_asm_path(func_name):
    print(f"DEBUG: find_asm_path called for {func_name}")
    unit_path = find_unit_name(func_name)
    
    print(f"DEBUG: func={func_name} unit={unit_path}")
    
    if not unit_path:
        return None
    
    base_name = unit_path
    if base_name.endswith('.o'): base_name = base_name[:-2]
    if base_name.endswith('.c'): base_name = base_name[:-2]
    
    parts = base_name.split('/')
    if not parts: return None
    
    module = parts[0]
    if module == "src":
        if len(parts) > 1:
            module = parts[1]
            base_name = "/".join(parts[1:]) 
        else:
            return None
            
    # ASM path construction
    rel_path = base_name

    candidate = f"build/GYQE01/{module}/asm/{rel_path}.s"
    print(f"DEBUG: checking candidate={candidate}")
    
    if os.path.exists(candidate):
        print(f"DEBUG: Found {candidate}")
        return candidate
        
    if rel_path.startswith(f"{module}/"):
        stripped = rel_path[len(module)+1:]
        candidate_2 = f"build/GYQE01/{module}/asm/{stripped}.s"
        print(f"DEBUG: checking candidate_2={candidate_2}")
        if os.path.exists(candidate_2):
            print(f"DEBUG: Found {candidate_2}")
            return candidate_2
            
class DecompOrchestrator:
    def __init__(self, func_name, unit_name):
        self.func_name = func_name
        self.unit_name = unit_name
        self.round = 0

    def generate_m2c_draft(self, asm_file):
        print(f"DEBUG: generate_m2c_draft for {self.func_name} using {asm_file}")
        # Use m2c_helper.py instead of raw m2c.py to get context automatically
        m2c_helper = "python3 tools/m2c_helper.py"
        env = os.environ.copy()
        
        cmd = f"{m2c_helper} {self.func_name} --target ppc-mwcc-c"
        try:
            print(f"DEBUG: Running m2c_helper cmd: {cmd}")
            # Use run instead of check_output so we can separate stdout and stderr
            res = subprocess.run(cmd.split(), env=env, capture_output=True, text=True)
            if res.returncode != 0:
                print(f"ERROR: m2c_helper failed: {res.stderr}")
                return f"/* Error generating draft: {res.stderr} */"
            
            print(f"DEBUG: m2c_helper success, output length: {len(res.stdout)}")
            return res.stdout
        except Exception as e:
            print(f"ERROR: m2c_helper Exception: {e}")
            return f"/* Exception generating draft: {e} */"

    def resolve_source_path(self):
        # Read objdiff.json to find base_path for this unit
        try:
            if os.path.exists("objdiff.json"):
                with open("objdiff.json", "r") as f:
                    config = json.load(f)
                    for unit in config.get("units", []):
                        if unit.get("name") == self.unit_name:
                            base_path = unit.get("base_path")
                            if base_path:
                                # build/GYQE01/src/game/rep_720.o -> src/game/rep_720.c
                                parts = base_path.split('/')
                                try:
                                    src_idx = parts.index('src')
                                    rel_path = "/".join(parts[src_idx:])
                                    if rel_path.endswith('.o'):
                                        rel_path = rel_path[:-2] + '.c'
                                    return rel_path
                                except ValueError:
                                    pass
        except Exception as e:
            print(f"ERROR: failed to resolve source path for {self.unit_name}: {e}")
        
        # Final fallback guessing
        parts = self.unit_name.split('/')
        if len(parts) > 1:
            # game/game/rep_720 -> src/game/rep_720.c
            return f"src/{'/'.join(parts[1:])}.c"
        return None

    def apply_header_changes(self, file_path, lines_text, replace_only=False):
        if not file_path or not os.path.exists(file_path):
            return False
        
        with open(file_path, "r") as f:
            content = f.read()
        
        lines = content.splitlines()
        modified = False
        any_replaced = False
        
        remaining_lines = []
        for new_line in lines_text.splitlines():
            new_line = new_line.strip()
            if not new_line: continue
            
            # Extract function name from the new line if it's a prototype
            match = re.search(r'(\w+)\s*\(', new_line)
            found_func_name = match.group(1) if match else None
            
            replaced = False
            if found_func_name:
                search_pat = rf"\b{re.escape(found_func_name)}\s*\("
                for i, line in enumerate(lines):
                    if re.search(search_pat, line) and ";" in line:
                         # Simple check to avoid replacing comments
                         if "//" not in line.split(found_func_name)[0]:
                             print(f"DEBUG: Replacing prototype for {found_func_name} in {file_path}")
                             lines[i] = new_line
                             replaced = True
                             modified = True
                             any_replaced = True
                             break
            
            if not replaced:
                remaining_lines.append(new_line)
        
        if not replace_only and remaining_lines:
            # Append what wasn't replaced
            for new_line in remaining_lines:
                if new_line in content: continue
                # Append before the last #endif
                insertion_idx = -1
                for i in range(len(lines) - 1, -1, -1):
                    if "#endif" in lines[i]:
                        insertion_idx = i
                        break
                
                if insertion_idx != -1:
                    lines.insert(insertion_idx, new_line)
                    modified = True
                else:
                    lines.append(new_line)
                    modified = True
        
        if modified:
            with open(file_path, "w") as f:
                f.write("\n".join(lines) + "\n")
        
        return any_replaced

    def remove_prototype(self, file_path, func_name):
        if not file_path or not os.path.exists(file_path):
            return
        with open(file_path, "r") as f:
            content = f.read()
        lines = content.splitlines()
        new_lines = []
        modified = False
        for line in lines:
            if f"{func_name}(" in line and ";" in line and "//" not in line.split(f"{func_name}(")[0]:
                print(f"DEBUG: Removing prototype for {func_name} from {file_path}")
                modified = True
                continue
            new_lines.append(line)
        if modified:
            with open(file_path, "w") as f:
                f.write("\n".join(new_lines) + "\n")

    def sync_source_signatures(self, src_path, header_text):
        if not src_path or not os.path.exists(src_path) or not header_text:
            return
            
        with open(src_path, "r") as f:
            content = f.read()
            
        modified = False
        for proto in header_text.splitlines():
            proto = proto.strip()
            if not proto or ";" not in proto: continue
            
            # Get function name
            match = re.search(r'(\w+)\s*\(', proto)
            if not match: continue
            func_name = match.group(1)
            
            # Don't sync the function we're actively committing (it gets full body replacement)
            if func_name == self.func_name: continue
            
            # Find definition signature in source
            # Clean up proto to be a signature (no semicolon, add brace start)
            new_sig = proto.replace(";", "").strip()
            
            # Pattern to find the signature part of a definition
            # ^[ \t]*[^\n;\(]+?\bfunc_name\b\s*\([^)]*\)\s*\{
            def_sig_pat = rf"(?m)^[ \t]*[^\n;\(]+?\b{re.escape(func_name)}\b\s*\([^)]*\)\s*\{{"
            
            if re.search(def_sig_pat, content):
                print(f"DEBUG: Syncing source signature for {func_name} in {src_path}")
                content = re.sub(def_sig_pat, new_sig + " {", content, count=1)
                modified = True
                
        if modified:
            with open(src_path, "w") as f:
                f.write(content)

    def commit_to_source(self, c_code):
        src_path = self.resolve_source_path()
        if not src_path or not os.path.exists(src_path):
            print(f"ERROR: could not find source file to commit: {src_path}")
            return False
        
        try:
            with open(src_path, "r") as f:
                content = f.read()

            print(f"DEBUG: commit_to_source for {self.func_name} in {src_path}")
            
            # Validation: Ensure the code being committed actually declares the function it's supposed to.
            # We look for the function name followed by an open parenthesis.
            if not re.search(rf"\b{re.escape(self.func_name)}\b\s*\(", c_code):
                print(f"ERROR: Code being committed for {self.func_name} does not appear to contain its definition.")
                return False

            # Pattern to match function signature and its body
            # We look for the function name preceded by a type at the start of a line.
            # Then we match everything until a '}' that is at the start of a line.
            pattern = rf"(?m)^[ \t]*[^\n;\(]+?\b{re.escape(self.func_name)}\b\s*\([^)]*\)\s*\{{.*?^\}}"
            
            match = re.search(pattern, content, re.DOTALL)
            if not match:
                # Fallback: a slightly looser pattern that doesn't require ^} if it's the last function or formatted differently
                print(f"DEBUG: strict regex did not find {self.func_name}. Trying fallback.")
                pattern = rf"(?m)^[ \t]*[^\n;\(]+?\b{re.escape(self.func_name)}\b\s*\([^)]*\)\s*\{{.*?\}}"
                match = re.search(pattern, content, re.DOTALL)
            
            if not match:
                print(f"ERROR: could not find function definition for {self.func_name} in {src_path}")
                return False

            print(f"DEBUG: Found match for {self.func_name} at range {match.start()} to {match.end()}")
            
            # Construct new content by replacing the matched range
            new_content = content[:match.start()] + "\n" + c_code.strip() + content[match.end():]
            
            # Ensure UnknownHeaders.h is included if it's our externs target
            externs_file = CONFIG.get("externs_header", "UnknownHeaders.h").split('/')[-1]
            include_line = f'#include "{externs_file}"'
            if include_line not in new_content:
                # Insert after the first include
                lines = new_content.splitlines()
                inserted = False
                for i, line in enumerate(lines):
                    if line.startswith('#include'):
                        lines.insert(i + 1, include_line)
                        inserted = True
                        break
                if not inserted:
                    lines.insert(0, include_line)
                new_content = "\n".join(lines)

            with open(src_path, "w") as f:
                f.write(new_content)
            
            print(f"DEBUG: Successfully committed {self.func_name} to {src_path}")
            return True
        except Exception as e:
            print(f"ERROR: failed to commit to {src_path}: {e}")
            return False

    def commit_all_changes(self, externs, headers, c_code):
        print(f"DEBUG: commit_all_changes for {self.func_name}")
        src_path = self.resolve_source_path()
        header_path = src_path.replace('src/', 'include/').replace('.c', '.h') if src_path else None
        externs_path = CONFIG.get("externs_header", "include/UnknownHeaders.h")
        
        # Reset files to base states if they exist
        for p in [src_path, header_path, externs_path]:
            if p and os.path.exists(p):
                subprocess.run(["git", "checkout", p], capture_output=True)

        if headers and header_path and os.path.exists(header_path):
            self.apply_header_changes(header_path, headers)
            if src_path:
                self.sync_source_signatures(src_path, headers)
            
        # 1. Externs - check if they should actually replace something in the local header first
        if externs and externs_path:
            if not os.path.exists(externs_path):
                os.makedirs(os.path.dirname(externs_path), exist_ok=True)
                with open(externs_path, "w") as f:
                    f.write("#ifndef UNKNOWN_HEADERS_H\n#define UNKNOWN_HEADERS_H\n\n#include \"types.h\"\n\n#endif\n")
            
            # Try to replace in local header first to prevent duplicates
            replaced_in_local = False
            if header_path and os.path.exists(header_path):
                replaced_in_local = self.apply_header_changes(header_path, externs, replace_only=True)
            
            # Only append to externs if NOT replaced in local header
            if not replaced_in_local:
                self.apply_header_changes(externs_path, externs)
            else:
                # If we replaced some but not all, we should still handle the rest
                # For simplicity, we'll try replacing in externs too
                self.apply_header_changes(externs_path, externs, replace_only=True)
            
            # Sync any signatures that moved to local header or were already there
            if src_path:
                self.sync_source_signatures(src_path, externs)

        # 3. Source Code
        if c_code:
            return self.commit_to_source(c_code)
            
        return True

    def run_build_and_score(self, c_code, externs=None, headers=None):
        print("DEBUG: run_build_and_score started")
        src_path = self.resolve_source_path()
        if not src_path or not os.path.exists(src_path):
             return {"status": "error", "log": f"Could not find source file for unit {self.unit_name}"}

        header_path = src_path.replace('src/', 'include/').replace('.c', '.h')
        externs_path = CONFIG.get("externs_header", "include/UnknownHeaders.h")

        # Backups
        backups = {}
        created_files = []
        for p in [src_path, header_path, externs_path]:
            if p and os.path.exists(p):
                with open(p, "r") as f: backups[p] = f.read()

        try:
            # 1. Ensure UnknownHeaders.h exists to prevent build failures
            if externs_path and not os.path.exists(externs_path):
                os.makedirs(os.path.dirname(externs_path), exist_ok=True)
                with open(externs_path, "w") as f:
                    f.write("#ifndef UNKNOWN_HEADERS_H\n#define UNKNOWN_HEADERS_H\n\n#include \"types.h\"\n\n#endif\n")
                created_files.append(externs_path)

            # 2. Temporarily apply changes
            # Local headers first
            if headers and header_path and os.path.exists(header_path):
                self.apply_header_changes(header_path, headers)
                if src_path:
                    self.sync_source_signatures(src_path, headers)

            if externs and externs_path:
                # Check for redeclarations in local header even if they came from externs box
                replaced_in_local = False
                if header_path and os.path.exists(header_path):
                     replaced_in_local = self.apply_header_changes(header_path, externs, replace_only=True)
                
                if not replaced_in_local:
                    self.apply_header_changes(externs_path, externs)
                else:
                    self.apply_header_changes(externs_path, externs, replace_only=True)
                
                if src_path:
                    self.sync_source_signatures(src_path, externs)

            if not self.commit_to_source(c_code):
                 return {"status": "error", "log": f"Could not find signature for {self.func_name} in {src_path}"}

            # 2. Build
            build_cmd_str = CONFIG.get("build_cmd", "ninja")
            print(f"DEBUG: Running build: {build_cmd_str}")
            try:
                build_proc = subprocess.run(build_cmd_str.split(), capture_output=True, text=True, timeout=120)
                if build_proc.returncode != 0:
                    combined_log = (build_proc.stdout or "") + (build_proc.stderr or "")
                    print(f"DEBUG: Build failed full log:\n{combined_log}")
                    return {"status": "build_error", "log": combined_log}
            except subprocess.TimeoutExpired:
                print("ERROR: Build timed out (120s)")
                return {"status": "build_timeout", "log": "Build process timed out after 120 seconds."}

            # 3. Score
            objdiff_bin = CONFIG.get("objdiff_path", "objdiff")
            cmd = [objdiff_bin, "diff", "--format", "json", "-u", self.unit_name, "-o", "-", self.func_name]
            print(f"DEBUG: Running objdiff: {' '.join(cmd)}")
            try:
                res = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
                if res.returncode != 0:
                    combined_log = (res.stdout or "") + (res.stderr or "")
                    print(f"DEBUG: Objdiff failed: {combined_log[:100]}...")
                    return {"status": "objdiff_error", "log": combined_log}
            except subprocess.TimeoutExpired:
                 print("ERROR: Objdiff timed out (30s)")
                 return {"status": "objdiff_timeout", "log": "Score calculation timed out."}
            
            data = json.loads(res.stdout)
            if "right" in data and "symbols" in data["right"]:
                for sym in data["right"]["symbols"]:
                    if sym.get("name") == self.func_name:
                        score = sym.get("match_percent", 0.0) / 100.0
                        print(f"DEBUG: Calculated score: {score}")
                        
                        # Get textual diff via feedback_diff.py for UI display
                        feedback_cmd = ["python3", "tools/feedback_diff.py", self.unit_name, self.func_name]
                        print(f"DEBUG: Running feedback_diff: {' '.join(feedback_cmd)}")
                        try:
                            res_text = subprocess.run(feedback_cmd, capture_output=True, text=True, timeout=30)
                        except subprocess.TimeoutExpired:
                            res_text = type('obj', (object,), {'stdout': '// Feedback diff timed out.', 'stderr': ''})()
                        
                        return {
                            "status": "success", 
                            "score": score, 
                            "diff": f"Match percent: {sym.get('match_percent')}%", 
                            "asm_diff": res_text.stdout or res_text.stderr or "// No differences found (100% match or empty output).",
                            "unit": self.unit_name
                        }
            
            print("DEBUG: Function not found in objdiff output")
            return {"status": "not_found", "score": 0, "diff": "Function not found in objdiff output"}

        except Exception as e:
            return {"status": "error", "log": str(e)}
        finally:
            # Revert modified files
            for p, content in backups.items():
                with open(p, "w") as f: f.write(content)
            # Delete temporary created files
            for p in created_files:
                if os.path.exists(p): os.remove(p)

    def run_build_and_score_old(self, c_code):
        try:
            res = subprocess.run(cmd, capture_output=True, text=True)
            
            # Always revert after the test build to maintain clean state 
            # (unless it's a 100% match,handled in process_function)
            with open(src_path, "w") as f: f.write(original_content)

            if res.returncode != 0:
                print(f"ERROR: objdiff failed: {res.stderr}")
                return {"status": "objdiff_error", "log": f"Objdiff failed: {res.stderr}"}
            
            data = json.loads(res.stdout)
            
            score = 0.0
            if "right" in data and "symbols" in data["right"]:
                for sym in data["right"]["symbols"]:
                    if sym.get("name") == self.func_name:
                        score = sym.get("match_percent", 0.0) / 100.0
                        print(f"DEBUG: objdiff match found. Score: {score}")
                        return {"status": "success", "score": score, "diff": f"Match percent: {sym.get('match_percent')}%"}
                
            print("DEBUG: Function not found in objdiff output")
            return {"status": "not_found", "score": 0, "diff": "Function not found in objdiff output"}
        except Exception as e:
            print(f"ERROR: objdiff exception: {e}")
            return {"status": "objdiff_exception", "log": str(e)}

def split_m2c_draft(c_code, func_name):
    externs = []
    unit_headers = []
    function_body = []
    
    lines = c_code.splitlines()
    in_function = False
    
    for line in lines:
        stripped = line.strip()
        # Skip assembly source info or metadata lines if any (usually starting with # or /*)
        if stripped.startswith('#'): continue

        if not stripped: 
            if in_function:
                 function_body.append(line)
            continue
        
        # Detect function start - look for the signature with a {
        if func_name in line and '{' in line and '(' in line:
            in_function = True
        
        if in_function:
            function_body.append(line)
        else:
            if stripped.startswith('?') or '/* extern */' in line:
                externs.append(line)
            elif stripped.startswith('extern') or stripped.startswith('static') or stripped.startswith('typedef'):
                unit_headers.append(line)
            else:
                # If it's a declaration before the function that we haven't caught
                if stripped.endswith(';') and '(' in stripped:
                    unit_headers.append(line)
                else:
                    externs.append(line)
                
    return {
        "externs": "\n".join(externs),
        "headers": "\n".join(unit_headers),
        "body": "\n".join(function_body)
    }

class ConfigUpdate(BaseModel):
    max_rounds: Optional[int]
    build_cmd: Optional[str]
    objdiff_path: Optional[str]
    include_dir: Optional[str]
    ghidra_path: Optional[str]
    m2c_path: Optional[str]
    prompt_template: Optional[str]

@app.get("/config")
def get_config():
    return CONFIG

@app.post("/config")
def update_config(conf: ConfigUpdate):
    for k, v in conf.dict(exclude_unset=True).items():
        if v is not None:
            CONFIG[k] = v
    return CONFIG

@app.get("/template")
def get_template():
    return {"template": CONFIG.get("prompt_template", "")}

@app.post("/template")
async def update_template(request: Request):
    data = await request.json()
    if "template" in data:
        CONFIG["prompt_template"] = data["template"]
    return {"status": "success", "template": CONFIG["prompt_template"]}

@app.get("/functions")
def get_functions(
    module: Optional[str] = None,
    max_match: float = 100.0,
    sort_by: str = "size",
    limit: int = 50
):
    report = load_report()
    if not report:
        return {"error": "report.json not found or invalid."}
        
    data = report
    candidates = []
    
    for unit in data.get("units", []):
        unit_name = unit.get("name", "")
        
        # Module filter (simple prefix check)
        if module and not unit_name.startswith(module):
            continue

        for func in unit.get("functions", []):
            func_name = func.get("name", "")
            size = func.get("size", 0)
            # Ensure size is an int. It might be a string (e.g. "0x100" or "256")
            if isinstance(size, str):
                try:
                    size = int(size, 0) # handles 0x prefix
                except ValueError:
                    size = 0
            
            # Get match percent
            # Handle different report formats: 'fuzzy_match_percent' or 'measures.matched_code_percent'
            match_pct = func.get("fuzzy_match_percent")
            if match_pct is None:
                measures = func.get("measures", {})
                match_pct = measures.get("matched_code_percent", 0.0)
            
            if match_pct is None: 
                match_pct = 0.0
                
            # Filter by match threshold
            if match_pct >= max_match:
                continue
                
            # Create candidate object
            candidates.append({
                "name": func_name,
                "size": size,
                "match_pct": match_pct,
                "unit": unit_name
            })
    
    # Sort
    reversed_sort = False
    if sort_by.startswith("-"):
        sort_by = sort_by[1:]
        reversed_sort = True
        
    # Default to size asc
    if sort_by == "size":
        candidates.sort(key=lambda x: x["size"], reverse=reversed_sort)
    elif sort_by == "match_pct":
        candidates.sort(key=lambda x: x["match_pct"], reverse=reversed_sort)
    elif sort_by == "name":
        candidates.sort(key=lambda x: x["name"], reverse=reversed_sort)

    return {
        "total": len(candidates),
        "limit": limit,
        "results": candidates[:limit]
    }

@app.post("/process/{func_name}")
async def process_function(func_name: str, request: Request):
    print(f"DEBUG: process_function called for {func_name}")
    body = await request.json()
    asm_path = body.get("asm_path")
    unit_name = find_unit_name(func_name)
    if not unit_name:
        print("DEBUG: unit_name resolution failed")
        return {"status": "error", "message": "Could not resolve unit for function."}

    if not asm_path:
        asm_path = find_asm_path(func_name)
        
    if not asm_path:
        print("DEBUG: asm_path resolution failed")
        return {"status": "error", "message": "asm_path is required and could not be resolved automatically."}

    orch = DecompOrchestrator(func_name, unit_name)
    
    current_c = orch.generate_m2c_draft(asm_path)
    result = orch.run_build_and_score(current_c)
    
    if result.get("score") == 1.0:
        print("DEBUG: Full match acheived!")
        orch.commit_to_source(current_c)
        draft_split = split_m2c_draft(current_c, func_name)
        return {
            "status": "matched", 
            "round": 0, 
            "result": result, 
            "draft_code": current_c,
            "draft_split": draft_split
        }
    
    # ... (rest of function)
    print("DEBUG: Reporting in_progress status")
    
    message = None
    if result.get("status") == "build_error":
         log = result.get('log', '')
         if len(log) > 500: log = log[:500] + "..."
         message = f"Build failed: {log}"
    elif result.get("status") == "objdiff_error":
         message = f"Objdiff failed: {result.get('log')}"

    return {
        "status": "in_progress", 
        "message": message,
        "current_score": result.get("score"), 
        "diff": result.get("diff"),
        "draft_code": current_c,
        "draft_split": split_m2c_draft(current_c, func_name),
        "result": result
    }


class CommitRequest(BaseModel):
    externs: str
    headers: str
    body: str

@app.post("/commit/{func_name}")
async def commit_to_base(func_name: str, body: CommitRequest):
    unit_name = find_unit_name(func_name)
    if not unit_name:
        return {"status": "error", "message": "Could not resolve unit."}
        
    orch = DecompOrchestrator(func_name, unit_name)
    success = orch.commit_all_changes(body.externs, body.headers, body.body)
    
    if success:
        return {"status": "success", "message": "Changes committed and verified."}
    else:
        return {"status": "error", "message": "Commit failed."}

@app.post("/verify/{func_name}")
async def verify_draft(func_name: str, body: CommitRequest):
    unit_name = find_unit_name(func_name)
    if not unit_name:
        return {"status": "error", "message": "Could not resolve unit."}
        
    orch = DecompOrchestrator(func_name, unit_name)
    result = orch.run_build_and_score(body.body, externs=body.externs, headers=body.headers)
    
    return {"status": "success", "result": result}
@app.post("/generate_prompt/{func_name}")
async def generate_prompt(func_name: str, body: CommitRequest):
    unit_name = find_unit_name(func_name)
    if not unit_name:
        return {"status": "error", "message": "Could not resolve unit."}
        
    orch = DecompOrchestrator(func_name, unit_name)
    asm_path = find_asm_path(func_name)
    asm_content = ""
    if asm_path:
        asm_content = decomp_helper.extract_asm(asm_path, func_name) or ""
    
    # Get feedback/errors
    result = orch.run_build_and_score(body.body, externs=body.externs, headers=body.headers)
    errors = result.get("asm_diff") or result.get("log") or ""
    
    # Context (Unit Header)
    context = body.headers
    
    # Template
    template = CONFIG.get("prompt_template", "")
    full_c = f"{body.externs}\n\n{body.headers}\n\n{body.body}"
    
    try:
        prompt = template.format(
            asm_content=asm_content,
            c_code=full_c,
            context=context,
            errors=errors
        )
        return {"status": "success", "prompt": prompt}
    except Exception as e:
        return {"status": "error", "message": f"Template formatting failed: {e}"}

# Mount static files
app.mount("/", StaticFiles(directory="tools/aidecomp/static", html=True), name="static")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
