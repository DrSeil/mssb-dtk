#!/bin/bash
set -e

# Change to project root
cd "$(dirname "$0")/../.."

# Ensure tools/m2c_repo exists (basic check)
if [ ! -d "tools/m2c_repo" ]; then
    echo "Error: tools/m2c_repo not found."
    exit 1
fi

VENV_DIR="tools/aidecomp/.venv"

# check if venv exists
if [ ! -d "$VENV_DIR" ]; then
    echo "Creating virtual environment in $VENV_DIR..."
    python3 -m venv "$VENV_DIR"
fi

# Activate venv
source "$VENV_DIR/bin/activate"

# Install requirements if not already satisfied
# (pip usually checks this fast, but we can double check)
echo "Checking dependencies..."
pip install -r tools/aidecomp/requirements.txt

echo "Starting ADS Dashboard..."
# Set PYTHONPATH to include project root and m2c_repo
export PYTHONPATH=$PYTHONPATH:$(pwd):$(pwd)/tools/m2c_repo

# Run the server using the venv python
python3 tools/aidecomp/server.py
