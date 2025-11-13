#!/bin/bash

# AltTasker - Run in QEMU Script
# This script sets up and runs AltTasker in a QEMU virtual machine

set -e

PROJECT_DIR="/mnt/d/AltTasker"
QEMU_DIR="$PROJECT_DIR/qemu-env"
DISK_IMAGE="$QEMU_DIR/alttasker.img"
MOUNT_POINT="$QEMU_DIR/mnt"

echo "🖥️  AltTasker - QEMU Runner"
echo "==========================="
echo ""

# Check if QEMU is installed
if ! command -v qemu-system-x86_64 &> /dev/null; then
    echo "❌ Error: QEMU is not installed"
    echo "   Install it with: sudo apt install qemu-system-x86"
    exit 1
fi

# Navigate to project directory
cd "$PROJECT_DIR"

# Build the project
echo "🔨 Building project..."
make clean > /dev/null 2>&1
make
echo "✅ Build successful!"
echo ""

# Create QEMU environment directory
mkdir -p "$QEMU_DIR"
mkdir -p "$MOUNT_POINT"

# For QEMU, we need a full Linux system
# This is a simplified approach - we'll use a lightweight Alpine Linux
echo "📦 QEMU Setup Options:"
echo ""
echo "Option 1: Run directly in WSL (Recommended)"
echo "   - Faster and simpler"
echo "   - Full Linux environment"
echo "   - Run: wsl bash scripts/run-in-wsl.sh"
echo ""
echo "Option 2: Use existing Linux VM with QEMU"
echo "   - You need a pre-built Linux image"
echo "   - More complex setup"
echo ""
echo "For testing AltTasker, WSL is recommended."
echo ""
echo "To create a full QEMU environment, you would need to:"
echo "  1. Download a Linux ISO (e.g., Alpine, Debian)"
echo "  2. Create a virtual disk"
echo "  3. Install Linux in QEMU"
echo "  4. Copy and run AltTasker inside the VM"
echo ""
echo "Quick start in WSL instead:"
echo "  wsl bash /mnt/d/AltTasker/scripts/run-in-wsl.sh"
