---
name: decompile-commit
description: Final verification and git commit for functions that have achieved a 100% binary match.
---

# Decompilation Finalization and Commit

This skill guides the final verification and git commit process for a matched function in the `mssb-dtk` project.

## Workflow

### 1. Final Verification
- Run `ninja` to ensure a clean build.
- Run `python3 tools/feedback_diff.py <function_name>` and confirm it reports `MATCH!`.
- **Double Check**: Ensure the function is not already matched in the repository.

### 2. Git Commit
- `git add <modified_files>` (source file, potential header changes).
- Commit with a clear message: `matched <function_name>`.

### 3. Documentation
- Update any local notes if necessary.
- Note any new structs or symbols discovered during the process.

## Constraints
- **NEVER** commit if `feedback_diff.py` does not report `MATCH!`.
- Ensure all struct definitions are complete and correctly typed.
- Follow the project's commit message style (usually `matched <function_name>`).
