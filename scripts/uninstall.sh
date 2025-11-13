#!/bin/bash
################################################################################
# AltTasker Uninstallation Script
# Version: 2.2
# Description: Removes AltTasker and restores system configuration
#
# Usage:
#   sudo ./uninstall.sh              # Standard uninstallation
#   sudo ./uninstall.sh --keep-backups  # Keep backup files
#   sudo ./uninstall.sh --unattended    # Non-interactive mode
#
# Requirements:
#   - Root privileges (sudo)
################################################################################

set -e  # Exit on error

# Colors for output
readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly CYAN='\033[0;36m'
readonly BOLD='\033[1m'
readonly RESET='\033[0m'

# Installation paths (must match install.sh)
readonly INSTALL_PREFIX="/usr/local"
readonly BIN_DIR="${INSTALL_PREFIX}/bin"
readonly SYSTEMD_DIR="/etc/systemd/system"
readonly SERVICE_NAME="alttasker-hotkey.service"
readonly TARGET_NAME="task"

# Flags
KEEP_BACKUPS=false
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

################################################################################
# Removal Functions
################################################################################

remove_binary() {
    print_info "Removing binary from ${BIN_DIR}/${TARGET_NAME}..."
    
    if [[ -f "${BIN_DIR}/${TARGET_NAME}" ]]; then
        rm -f "${BIN_DIR}/${TARGET_NAME}"
        print_success "Binary removed"
    else
        print_warning "Binary not found (already removed?)"
    fi
}

remove_systemd_service() {
    print_info "Removing systemd service..."
    
    local service_exists=false
    
    # Stop service if running
    if systemctl is-active "${SERVICE_NAME}" &> /dev/null; then
        print_info "Stopping ${SERVICE_NAME}..."
        systemctl stop "${SERVICE_NAME}" || true
    fi
    
    # Disable service
    if systemctl is-enabled "${SERVICE_NAME}" &> /dev/null; then
        print_info "Disabling ${SERVICE_NAME}..."
        systemctl disable "${SERVICE_NAME}" &> /dev/null || true
        service_exists=true
    fi
    
    # Remove service file
    if [[ -f "${SYSTEMD_DIR}/${SERVICE_NAME}" ]]; then
        rm -f "${SYSTEMD_DIR}/${SERVICE_NAME}"
        service_exists=true
    fi
    
    if [[ "$service_exists" == true ]]; then
        print_success "Systemd service removed"
    else
        print_warning "Systemd service not found (already removed?)"
    fi
}

restore_ctrl_alt_delete() {
    print_info "Restoring Ctrl+Alt+Delete default behavior..."
    
    local restored=false
    
    # Remove override
    if [[ -d "${SYSTEMD_DIR}/ctrl-alt-del.target.d" ]]; then
        rm -rf "${SYSTEMD_DIR}/ctrl-alt-del.target.d"
        restored=true
    fi
    
    # Unmask target
    if systemctl is-masked ctrl-alt-del.target &> /dev/null; then
        systemctl unmask ctrl-alt-del.target &> /dev/null || true
        restored=true
    fi
    
    # Restore from backup if exists
    local latest_backup
    latest_backup=$(find "${SYSTEMD_DIR}" -maxdepth 1 -name "ctrl-alt-del.target.backup.*" \
                    2>/dev/null | sort -r | head -n 1)
    
    if [[ -n "$latest_backup" ]]; then
        print_info "Found backup: $(basename "$latest_backup")"
        cp "$latest_backup" "${SYSTEMD_DIR}/ctrl-alt-del.target"
        restored=true
    fi
    
    # Reload systemd
    if [[ "$restored" == true ]]; then
        systemctl daemon-reload
        print_success "Ctrl+Alt+Delete restored to default"
    else
        print_warning "No Ctrl+Alt+Delete configuration found (already restored?)"
    fi
}

remove_backups() {
    if [[ "$KEEP_BACKUPS" == true ]]; then
        print_info "Keeping backup files (--keep-backups)"
        return 0
    fi
    
    print_info "Removing backup files..."
    
    local backup_count=0
    
    # Find and remove all backups
    while IFS= read -r -d '' backup_file; do
        rm -f "$backup_file"
        ((backup_count++))
    done < <(find "${SYSTEMD_DIR}" -maxdepth 1 -name "*.backup.*" -print0 2>/dev/null)
    
    if [[ $backup_count -gt 0 ]]; then
        print_success "Removed $backup_count backup file(s)"
    else
        print_info "No backup files found"
    fi
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
# Main Uninstallation
################################################################################

main() {
    # Parse arguments
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --keep-backups)
                KEEP_BACKUPS=true
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
                echo "  --keep-backups  Don't remove backup files"
                echo "  --unattended    Non-interactive mode (assume yes)"
                echo "  --help          Show this help message"
                echo ""
                echo "Example:"
                echo "  sudo $0"
                echo "  sudo $0 --keep-backups"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                echo "Use --help for usage information"
                exit 1
                ;;
        esac
    done
    
    print_header "AltTasker v2.2 - Uninstallation"
    
    # Pre-flight checks
    check_root
    
    # Check if installed
    local is_installed=false
    [[ -f "${BIN_DIR}/${TARGET_NAME}" ]] && is_installed=true
    [[ -f "${SYSTEMD_DIR}/${SERVICE_NAME}" ]] && is_installed=true
    
    if [[ "$is_installed" == false ]]; then
        print_warning "AltTasker does not appear to be installed"
        if ! ask_user "Continue with cleanup anyway?" "n"; then
            print_info "Uninstallation cancelled"
            exit 0
        fi
    fi
    
    # Show removal plan
    echo -e "${BOLD}Removal Plan:${RESET}"
    echo "  • Remove binary: ${BIN_DIR}/${TARGET_NAME}"
    echo "  • Remove systemd service: ${SERVICE_NAME}"
    echo "  • Restore Ctrl+Alt+Delete default behavior"
    if [[ "$KEEP_BACKUPS" == false ]]; then
        echo "  • Remove backup files"
    fi
    echo ""
    
    if ! ask_user "Proceed with uninstallation?"; then
        print_warning "Uninstallation cancelled by user"
        exit 0
    fi
    
    # Remove binary
    print_header "Step 1: Removing Binary"
    remove_binary
    
    # Remove systemd components
    if command -v systemctl &> /dev/null; then
        print_header "Step 2: Removing System Integration"
        remove_systemd_service
        restore_ctrl_alt_delete
    else
        print_warning "systemd not found - skipping service removal"
    fi
    
    # Clean up backups
    print_header "Step 3: Cleanup"
    remove_backups
    
    # Success message
    print_header "Uninstallation Complete!"
    
    echo -e "${GREEN}${BOLD}✅ AltTasker has been removed!${RESET}"
    echo ""
    
    if [[ "$KEEP_BACKUPS" == true ]]; then
        echo -e "${BOLD}Backups Kept:${RESET}"
        echo "  Location: ${SYSTEMD_DIR}/*.backup.*"
        echo "  Remove manually: ${CYAN}sudo rm ${SYSTEMD_DIR}/*.backup.*${RESET}"
        echo ""
    fi
    
    echo -e "${BOLD}What was removed:${RESET}"
    echo "  • Binary:     ${BIN_DIR}/${TARGET_NAME}"
    echo "  • Service:    ${SYSTEMD_DIR}/${SERVICE_NAME}"
    echo "  • Overrides:  ${SYSTEMD_DIR}/ctrl-alt-del.target.d/"
    echo ""
    
    echo -e "${BOLD}Ctrl+Alt+Delete:${RESET}"
    echo "  Default system behavior has been restored"
    echo ""
    
    print_info "Thank you for using AltTasker!"
    echo ""
}

# Run main function
main "$@"
