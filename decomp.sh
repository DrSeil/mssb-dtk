#!/bin/bash

M2C=0
if [ "$1" == "--m2c" ]; then
    M2C=1
    shift
fi

if [ -z "$1" ]; then
    echo "Usage: ./decomp.sh [--m2c] <function_name>"
    exit 1
fi

FUNC=$1

if [ $M2C -eq 1 ]; then
    python3 tools/m2c_helper.py "$FUNC"
    exit 0
fi

# Run the helper to find the source and ASM
RESULT=$(python3 tools/decomp_helper.py "$FUNC")
echo "$RESULT"

SOURCE=$(echo "$RESULT" | grep "Source:" | cut -d' ' -f2)
ASM_PATH=$(echo "$RESULT" | grep "Expected ASM Path:" | cut -d' ' -f4)

if [ -z "$SOURCE" ]; then
    echo "Could not find source for $FUNC"
    exit 1
fi

echo "------------------------------------------------"
echo "Editing $SOURCE..."
# In a real environment, you might open an editor here.
# For Antigravity, we'll just wait or provide the info.

echo "Once you've made changes, run 'ninja' to compile."
echo "Then run 'objdiff diff' to check the results."
echo "------------------------------------------------"
