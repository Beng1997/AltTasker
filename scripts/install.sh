#!/bin/bash
################################################################################
# AltTasker Installation Script
# Version: 2.2
# Description: Installs AltTasker system-wide with optional Ctrl+Alt+Delete
#              integration
#
# Usage:
#   sudo ./install.sh              # Standard installation
#   sudo ./install.sh --no-hotkey  # Skip Ctrl+Alt+Delete setup
#   sudo ./install.sh --unattended # Non-interactive mode
#
# Requirements:
#   - Root privileges (sudo)
#   - gcc, make
#   - systemd (for Ctrl+Alt+Delete integration)
################################################################################

set -e  # Exit on error

# Colors for output
readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly CYAN='\033[0;36m'
readonly BOLD='\033[1m'
readonly RESET='\033[0m'

# Installation paths
readonly INSTALL_PREFIX="/usr/local"
readonly BIN_DIR="${INSTALL_PREFIX}/bin"
readonly SYSTEMD_DIR="/etc/systemd/system"
readonly SERVICE_NAME="alttasker-hotkey.service"

# Build configuration
readonly BUILD_DIR="build"
readonly BINARY_NAME="alttasker"
readonly TARGET_NAME="task"  # Shorter command name

# Flags
SKIP_HOTKEY=false
UNATTENDED=false

################################################################################
# Helper Functions
################################################################################

print_header() {
    echo ""
    echo -e "${CYAN}╔══════════════════════════════════════════════════════════╗${RESET}"
    echo -e "${CYAN}║${RESET}${BOLD}  $1${RESET}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════╝${RESET}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✅ $1${RESET}"
}

print_error() {
    echo -e "${RED}❌ $1${RESET}" >&2
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${RESET}"
}

print_info() {
    echo -e "${CYAN}ℹ️  $1${RESET}"
}

check_root() {
    if [[ $EUID -ne 0 ]]; then
        print_error "This script must be run as root (use sudo)"
        exit 1
    fi
}

check_dependencies() {
    local missing_deps=()
    
    for cmd in gcc make; do
        if ! command -v "$cmd" &> /dev/null; then
            missing_deps+=("$cmd")
        fi
    done
    
    if [[ ${#missing_deps[@]} -gt 0 ]]; then
        print_error "Missing required dependencies: ${missing_deps[*]}"
        print_info "Install with: sudo apt install build-essential"
        exit 1
    fi
}

################################################################################
# Build Functions
################################################################################

build_project() {
    print_info "Building AltTasker..."
    
    # Navigate to project root
    cd "$(dirname "$0")/.." || exit 1
    
    # Clean previous build
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
    fi
    
    # Build
    if make clean &> /dev/null && make &> /dev/null; then
        print_success "Build completed successfully"
    else
        print_error "Build failed"
        print_info "Try running 'make' manually to see detailed errors"
        exit 1
    fi
    
    # Verify binary exists
    if [[ ! -f "$BINARY_NAME" ]]; then
        print_error "Binary '$BINARY_NAME' not found after build"
        exit 1
    fi
}

################################################################################
# Installation Functions
################################################################################

install_binary() {
    print_info "Installing binary to ${BIN_DIR}/${TARGET_NAME}..."
    
    # Create bin directory if it doesn't exist
    mkdir -p "$BIN_DIR"
    
    # Copy binary
    cp "$BINARY_NAME" "${BIN_DIR}/${TARGET_NAME}"
    chmod 755 "${BIN_DIR}/${TARGET_NAME}"
    
    # Verify installation
    if [[ -x "${BIN_DIR}/${TARGET_NAME}" ]]; then
        print_success "Binary installed: ${BIN_DIR}/${TARGET_NAME}"
    else
        print_error "Failed to install binary"
        exit 1
    fi
}

create_systemd_service() {
    print_info "Creating systemd service for Ctrl+Alt+Delete..."
    
    cat > "${SYSTEMD_DIR}/${SERVICE_NAME}" <<EOF
[Unit]
Description=AltTasker - Alternative Task Manager
Documentation=https://github.com/Beng1997/AltTasker
After=graphical.target

[Service]
Type=simple
ExecStart=${BIN_DIR}/${TARGET_NAME}
StandardInput=tty
TTYPath=/dev/tty1
TTYReset=yes
TTYVHangup=yes
Restart=no
User=root

# Security settings
ProtectSystem=strict
ProtectHome=yes
NoNewPrivileges=true
PrivateTmp=yes

[Install]
WantedBy=graphical.target
EOF

    chmod 644 "${SYSTEMD_DIR}/${SERVICE_NAME}"
    print_success "Service file created: ${SYSTEMD_DIR}/${SERVICE_NAME}"
}

setup_ctrl_alt_delete() {
    if [[ "$SKIP_HOTKEY" == true ]]; then
        print_warning "Skipping Ctrl+Alt+Delete setup (--no-hotkey)"
        return 0
    fi
    
    print_info "Setting up Ctrl+Alt+Delete integration..."
    
    # Backup existing ctrl-alt-del.target
    if [[ -f "${SYSTEMD_DIR}/ctrl-alt-del.target" ]]; then
        cp "${SYSTEMD_DIR}/ctrl-alt-del.target" \
           "${SYSTEMD_DIR}/ctrl-alt-del.target.backup.$(date +%Y%m%d_%H%M%S)"
        print_info "Backed up existing ctrl-alt-del.target"
    fi
    
    # Create symlink to our service
    systemctl mask ctrl-alt-del.target 2>/dev/null || true
    
    # Create override
    mkdir -p "${SYSTEMD_DIR}/ctrl-alt-del.target.d"
    cat > "${SYSTEMD_DIR}/ctrl-alt-del.target.d/override.conf" <<EOF
[Unit]
Description=AltTasker replaces Ctrl+Alt+Delete
Requires=${SERVICE_NAME}

[Install]
WantedBy=multi-user.target
EOF

    # Reload systemd
    systemctl daemon-reload
    
    # Enable service
    systemctl enable "${SERVICE_NAME}" &> /dev/null || true
    
    print_success "Ctrl+Alt+Delete now launches AltTasker"
    print_warning "Note: Ctrl+Alt+Delete will work in console/TTY mode"
}

################################################################################
# Interactive Functions
################################################################################

ask_user() {
    local prompt="$1"
    local default="${2:-y}"
    
    if [[ "$UNATTENDED" == true ]]; then
        return 0  # Assume yes in unattended mode
    fi
    
    local response
    if [[ "$default" == "y" ]]; then
        read -p "$prompt [Y/n]: " response
        response=${response:-y}
    else
        read -p "$prompt [y/N]: " response
        response=${response:-n}
    fi
    
    [[ "$response" =~ ^[Yy] ]]
}

################################################################################
# Main Installation
################################################################################

main() {
    # Parse arguments
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --no-hotkey)
                SKIP_HOTKEY=true
                shift
                ;;
            --unattended|-y)
                UNATTENDED=true
                shift
                ;;
            --help|-h)
                echo "Usage: sudo $0 [OPTIONS]"
                echo ""
                echo "Options:"
                echo "  --no-hotkey     Skip Ctrl+Alt+Delete integration"
                echo "  --unattended    Non-interactive mode (assume yes)"
                echo "  --help          Show this help message"
                echo ""
                echo "Example:"
                echo "  sudo $0"
                echo "  sudo $0 --no-hotkey"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                echo "Use --help for usage information"
                exit 1
                ;;
        esac
    done
    
    print_header "AltTasker v2.2 - Installation"
    
    # Pre-flight checks
    check_root
    check_dependencies
    
    # Show installation plan
    echo -e "${BOLD}Installation Plan:${RESET}"
    echo "  • Build AltTasker from source"
    echo "  • Install binary to: ${BIN_DIR}/${TARGET_NAME}"
    if [[ "$SKIP_HOTKEY" == false ]]; then
        echo "  • Configure Ctrl+Alt+Delete hotkey"
        echo "  • Create systemd service"
    fi
    echo ""
    
    if ! ask_user "Proceed with installation?"; then
        print_warning "Installation cancelled by user"
        exit 0
    fi
    
    # Build
    print_header "Step 1: Building"
    build_project
    
    # Install binary
    print_header "Step 2: Installing Binary"
    install_binary
    
    # Setup systemd (if not skipped)
    if [[ "$SKIP_HOTKEY" == false ]]; then
        print_header "Step 3: System Integration"
        
        if command -v systemctl &> /dev/null; then
            create_systemd_service
            setup_ctrl_alt_delete
        else
            print_warning "systemd not found - skipping Ctrl+Alt+Delete setup"
        fi
    fi
    
    # Success message
    print_header "Installation Complete!"
    
    echo -e "${GREEN}${BOLD}✅ AltTasker installed successfully!${RESET}"
    echo ""
    echo -e "${BOLD}Quick Start:${RESET}"
    echo "  Run directly:    ${CYAN}task${RESET}"
    echo "  With sudo:       ${CYAN}sudo task${RESET}"
    echo ""
    
    if [[ "$SKIP_HOTKEY" == false ]] && command -v systemctl &> /dev/null; then
        echo -e "${BOLD}Ctrl+Alt+Delete:${RESET}"
        echo "  Press Ctrl+Alt+Delete in console/TTY to launch AltTasker"
        echo "  (May require logout/login to take effect)"
        echo ""
    fi
    
    echo -e "${BOLD}Commands:${RESET}"
    echo "  Sort:    P (PID)  C (CPU)  M (Memory)  U (User)"
    echo "  Scroll:  ↑/↓ (line)  PgUp/PgDn (page)  Home/End"
    echo "  Filter:  F (by user)  R (reset)"
    echo "  Actions: K (kill)  S (search)  Q (quit)"
    echo ""
    
    print_info "For more info: ${CYAN}https://github.com/Beng1997/AltTasker${RESET}"
    echo ""
}

# Run main function
main "$@"
