#!/bin/bash

# AltTasker - Run in WSL Script
# This script compiles and runs AltTasker in WSL

set -e

PROJECT_DIR="/mnt/d/AltTasker"

echo "🚀 AltTasker - WSL Runner"
echo "========================="
echo ""

# Navigate to project directory
cd "$PROJECT_DIR"

# Clean and build
echo "🔨 Building project..."
make clean > /dev/null 2>&1
make

echo ""
echo "✅ Build successful!"
echo ""
echo "📊 Starting AltTasker..."
echo "   Press Ctrl+C to exit"
echo ""
sleep 2

# Run the application
./alttasker
