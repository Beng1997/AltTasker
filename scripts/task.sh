#!/bin/bash

# Quick run script for AltTasker
# This can be sourced or executed directly

# Navigate to project
cd /mnt/d/AltTasker 2>/dev/null || cd "$(dirname "$0")/.."

# Check if compiled
if [ ! -f "./alttasker" ]; then
    echo "⚠️  AltTasker not compiled yet. Compiling..."
    make clean && make
    if [ $? -ne 0 ]; then
        echo "❌ Compilation failed!"
        exit 1
    fi
fi

# Run AltTasker
./alttasker
