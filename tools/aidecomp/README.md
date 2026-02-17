# Autonomous Decompilation System (ADS)

## Overview
A code-driven pipeline for decompiling Mario Superstar Baseball functions, leveraging m2c, Ghidra (planned), and AI refinement (simulated/manual hook).

## Setup

1. **Requirements**:
   - Python 3.9+
   - `ninja` build system
   - `objdiff` (installed or in path)
   - `m2c` repository cloned in `tools/m2c_repo`

2. **Installation**:
   Run the setup/runner script. This script automatically creates a virtual environment in `tools/aidecomp/.venv` and installs dependencies:
   ```bash
   ./tools/aidecomp/run.sh
   ```

## Usage

1. **Start the Dashboard**:
   ```bash
   ./tools/aidecomp/run.sh
   ```
   The server will start at `http://0.0.0.0:8000`.

2. **Dashboard Features**:
   - **Dashboard**: Run pipelines for specific functions.
   - **Monitor**: View active tasks.
   - **Explore**: Search for unmatched functions filterable by module, size, and match %.
   - **Config**: Update paths and settings.
   - **Template**: Edit the prompt template used for AI refinement.

3. **Pipeline**:
   - Enter a function name (e.g., `fn_80012345`) and ASM path.
   - The system generates an initial m2c draft.
   - It attempts to build and score the draft.
   - If score is 100%, it commits to git.
   - If not match, it would normally loop with AI (currently simulated/single-pass).
   - On failure, it creates a `decomp/fail-[func]` branch.

## Directory Structure
- `tools/aidecomp/server.py`: Main backend logic.
- `tools/aidecomp/static/`: Frontend assets (HTML/CSS/JS).
- `tools/aidecomp/requirements.txt`: Python deps.
