---
description: Iteratively decompile a function with up to 5 refinement attempts
---

Use this workflow to decompile a specific function. The goal is to reach a 100% match with the original assembly using iterative AI refinement.

### Step 1: Initialization
1.  Run `./decomp.sh <function_name>` to identify the source file, module, and target assembly.
2.  Note the `objdiff` command suggested by the tool.

### Step 2: Initial Implementation
1.  Open the identified source file.
2.  Create an initial C implementation for the function. Try to match the logic of the provided assembly as closely as possible.
3.  Save the file.

### Step 3: Iterative Refinement (Max 5 attempts)
Repeat the following steps up to 5 times:

1.  **Compile**: Run `ninja` in the root directory.
    - If compilation fails, analyze the compiler errors, fix the C code, and return to Step 3.1.
2.  **Diff**: Run the `feedback_diff.py` tool using the information from Step 1:
    ```bash
    python3 tools/feedback_diff.py <unit_name> <function_name>
    ```
3.  **Check Result**:
    - If the output is `MATCH!`, you are done!
    - Otherwise, analyze the line-by-line feedback (mismatched registers, ops, immediates).
4.  **Refine**: Update the C code based on the feedback to resolve the differences. Save the file and return to Step 3.1.

### Step 4: Finalization
1.  Once a match is achieved, or the 5 attempts are exhausted, summarize the results.
2.  If a match was not achieved, document the remainingDifferences.
