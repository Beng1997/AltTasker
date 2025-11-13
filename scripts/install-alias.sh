#!/bin/bash

# AltTasker - Install Alias Script
# This script installs the 'task' command alias

PROJECT_DIR="/mnt/d/AltTasker"
SHELL_RC="$HOME/.bashrc"

echo "🚀 Installing AltTasker alias..."
echo ""

# Check if .bashrc exists
if [ ! -f "$SHELL_RC" ]; then
    touch "$SHELL_RC"
    echo "✅ Created $SHELL_RC"
fi

# Check if alias already exists
if grep -q "alias task=" "$SHELL_RC"; then
    echo "⚠️  Alias 'task' already exists in $SHELL_RC"
    echo "   Updating..."
    sed -i '/alias task=/d' "$SHELL_RC"
fi

# Add alias
echo "" >> "$SHELL_RC"
echo "# AltTasker alias" >> "$SHELL_RC"
echo "alias task='$PROJECT_DIR/alttasker'" >> "$SHELL_RC"

echo "✅ Alias installed successfully!"
echo ""
echo "To use it:"
echo "  1. Reload your shell: source ~/.bashrc"
echo "  2. Or restart your terminal"
echo "  3. Run: task"
echo ""
echo "Note: Make sure to compile first with: make"
