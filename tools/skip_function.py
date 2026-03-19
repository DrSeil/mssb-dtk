#!/usr/bin/env python3
import sys
import subprocess
import os

def run_command(cmd):
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running command: {' '.join(cmd)}")
        print(result.stderr)
        return False, result.stdout, result.stderr
    return True, result.stdout, result.stderr

def skip_function(func_name):
    print(f"Skipping function: {func_name}")
    
    # 1. Get current branch name
    success, stdout, _ = run_command(['git', 'rev-parse', '--abbrev-ref', 'HEAD'])
    if not success:
        return
    current_branch = stdout.strip()
    
    # 2. Check if there are changes to commit
    success, stdout, _ = run_command(['git', 'status', '--porcelain'])
    if not success:
        return
    
    if stdout.strip():
        # Create a new branch and commit changes
        skip_branch = f"skip/{func_name}"
        print(f"Creating branch {skip_branch} to save work...")
        
        # Check if branch exists
        success, _, _ = run_command(['git', 'show-ref', '--verify', '--quiet', f'refs/heads/{skip_branch}'])
        if success:
            print(f"Branch {skip_branch} already exists. Appending to it.")
            # Maybe just commit on top if we're already on it? 
            # For simplicity, we'll try to checkout it.
        
        # Create and checkout the branch
        run_command(['git', 'checkout', '-b', skip_branch])
        run_command(['git', 'add', '.'])
        run_command(['git', 'commit', '-m', f"WIP: decomp of {func_name} (skipped)"])
        
        # Switch back to the original branch
        print(f"Switching back to {current_branch}")
        run_command(['git', 'checkout', current_branch])
    else:
        print("No changes to save.")

    # 3. Add to skip list
    skip_list_path = "config/skip_list.txt"
    os.makedirs(os.path.dirname(skip_list_path), exist_ok=True)
    
    already_skipped = False
    if os.path.exists(skip_list_path):
        with open(skip_list_path, 'r') as f:
            if func_name in f.read().splitlines():
                already_skipped = True
    
    if not already_skipped:
        with open(skip_list_path, 'a') as f:
            f.write(f"{func_name}\n")
        print(f"Added {func_name} to {skip_list_path}")
    else:
        print(f"{func_name} is already in the skip list.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: ./tools/skip_function.py <function_name>")
        sys.exit(1)
    
    skip_function(sys.argv[1])
