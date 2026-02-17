
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
    "prompt_template": "Refine the following C code based on the assembly:\n\n{asm_content}\n\nCurrent C:\n{c_code}\n\nContext:\n{context}\n\nErrors:\n{errors}"

}

REPORT_CACHE = None

def load_report():
    global REPORT_CACHE
    if REPORT_CACHE:
        return REPORT_CACHE
    
    report_path = "report.json"
    if not os.path.exists(report_path):
        return None
        
    try:
        with open(report_path, "r") as f:
            REPORT_CACHE = json.load(f)
        return REPORT_CACHE
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
        m2c_py = CONFIG.get('m2c_path', 'python3 tools/m2c_repo/m2c.py')
        env = os.environ.copy()
        pythonpath = env.get("PYTHONPATH", "")
        repo_path = os.path.abspath("tools/m2c_repo")
        if repo_path not in pythonpath:
            env["PYTHONPATH"] = f"{repo_path}:{pythonpath}"

        cmd = f"{m2c_py} --target ppc-mwcc-c {asm_file} --function {self.func_name}"
        try:
            print(f"DEBUG: Running m2c cmd: {cmd}")
            res = subprocess.check_output(cmd, shell=True, env=env, text=True, stderr=subprocess.STDOUT)
            print(f"DEBUG: m2c success, output length: {len(res)}")
            return res
        except subprocess.CalledProcessError as e:
            print(f"ERROR: m2c failed: {e.output}")
            return f"/* Error generating draft: {e.output} */"

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

    def commit_to_source(self, c_code):
        src_path = self.resolve_source_path()
        if not src_path or not os.path.exists(src_path):
            print(f"ERROR: could not find source file to commit: {src_path}")
            return False
        
        try:
            with open(src_path, "r") as f:
                content = f.read()
            
            # Pattern to match function definition: type func_name(args) { body }
            # Added more robust type list and account for pointers/spacing
            pattern = rf"(void|u8|u16|u32|s8|s16|s32|int|float|f32|f64|char)\s*\*?\s*{self.func_name}\s*\([^)]*\)\s*\{{[^}}]*\}}"
            
            if not re.search(pattern, content, re.DOTALL):
                print(f"DEBUG: regex did not find function {self.func_name} in {src_path}")
                # Fallback: simpler pattern if the above is too specific
                pattern = rf"{self.func_name}\s*\([^)]*\)\s*\{{[^}}]*\}}"
                if not re.search(pattern, content, re.DOTALL):
                    return False

            new_content = re.sub(pattern, c_code.strip(), content, flags=re.DOTALL)
            
            with open(src_path, "w") as f:
                f.write(new_content)
            
            print(f"DEBUG: Successfully committed {self.func_name} to {src_path}")
            return True
        except Exception as e:
            print(f"ERROR: failed to commit to {src_path}: {e}")
            return False

    def run_build_and_score(self, c_code):
        print("DEBUG: run_build_and_score started")
        src_path = self.resolve_source_path()
        if not src_path or not os.path.exists(src_path):
             return {"status": "error", "log": f"Could not find source file for unit {self.unit_name}"}

        # Backup original source
        original_content = None
        try:
            with open(src_path, "r") as f:
                original_content = f.read()
        except Exception as e:
            return {"status": "error", "log": f"Failed to read source: {e}"}

        # Integrate AI code temporarily for assessment
        if not self.commit_to_source(c_code):
             return {"status": "error", "log": f"Could not find signature for {self.func_name} in {src_path}"}
        
        # 1. Build
        build_cmd_str = CONFIG.get("build_cmd", "ninja")
        print(f"DEBUG: Running build cmd: {build_cmd_str}")
        try:
            build_proc = subprocess.run(build_cmd_str.split(), capture_output=True, text=True)
            if build_proc.returncode != 0:
                print(f"DEBUG: Build failed: {build_proc.stderr[:200]}...")
                # Revert on fail
                with open(src_path, "w") as f: f.write(original_content)
                return {"status": "build_error", "log": build_proc.stderr}
        except Exception as e:
            print(f"ERROR: Build exception: {e}")
            with open(src_path, "w") as f: f.write(original_content)
            return {"status": "build_exception", "log": str(e)}

        # 2. Score
        objdiff_bin = CONFIG.get("objdiff_path", "objdiff")
        cmd = [objdiff_bin, "diff", "--format", "json", "-u", self.unit_name, "-o", "-", self.func_name]
        print(f"DEBUG: Running objdiff: {cmd}")
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
            # Some reports might put it in 'measures', others 'metadata'
            # Based on score_functions.py it is in measures
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
        return {"status": "matched", "round": 0, "result": result, "draft_code": current_c}
    
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
        "result": result
    }


# Mount static files
app.mount("/", StaticFiles(directory="tools/aidecomp/static", html=True), name="static")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
