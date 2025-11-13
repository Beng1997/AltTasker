# AltTasker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-2.3-blue.svg)](https://github.com/Beng1997/AltTasker/releases)
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)](https://github.com/Beng1997/AltTasker)
[![Language](https://img.shields.io/badge/language-C-orange.svg)](https://github.com/Beng1997/AltTasker)

A lightweight, `htop`-style **Linux task monitor** written in C. Monitor your system processes with a beautiful, colorful terminal interface featuring **custom themes**, **process trees**, and **persistent configuration**.

**Platform Support:** Native Linux systems (Ubuntu, Debian, Fedora, Arch, etc.) and WSL (Windows Subsystem for Linux).

## ✨ Features

### Core Functionality
- 📊 **Real-time process monitoring** - Live system process information
- 🎨 **Colorful terminal UI** - Beautiful ANSI colors with progress bars
- 💾 **Memory usage visualization** - Visual bars showing memory consumption
- ⚡ **Fast and efficient** - Lightweight C implementation with minimal overhead
- 🎯 **Top processes view** - Shows top 20 processes by memory usage (configurable)

### New in v2.3 🎉
- ⚙️ **Configuration System** - Persistent settings in `~/.alttaskerrc`
- 🎨 **Custom Color Themes** - 5 themes: default/dark/light/colorblind/custom (press **T**)
- 📈 **Improved CPU%** - Real-time CPU percentage calculation
- 🔍 **Advanced Filtering** - Filter by name, state, memory threshold
- 🌳 **Process Tree View** - Parent-child hierarchy display (press **V**)

### Interactive Commands
- 🔄 **Sorting** - Sort by PID (P), CPU (C), Memory (M), User (U)
- 🔍 **Filtering** - Filter by username (F), reset filters (R)
- 🎮 **Process Control** - Kill processes (K), search (S)
- 📜 **Scrolling** - Navigate with Arrow keys, Page Up/Down, Home/End
- 🎨 **Themes** - Cycle color themes (T)
- 🌳 **Tree View** - Toggle process hierarchy (V)
- ⌨️ **Hotkey Integration** - Optional Ctrl+Alt+Delete support (systemd)

### Display Features
- ✨ **Visual indicators** - ▲ More above / ▼ More below
- 📍 **Position tracking** - "Showing X-Y of Z processes"
- 🎨 **Color coding** - Green (running), Yellow (disk wait), Red (zombie)
- 📊 **System overview** - Uptime, process count, memory usage bar
- 🌈 **Theme customization** - Choose your preferred color scheme

## 🚀 Quick Start

### Option A: System-Wide Installation (Recommended)

Install AltTasker system-wide with optional Ctrl+Alt+Delete hotkey integration:

```bash
# Clone the repository
git clone https://github.com/Beng1997/AltTasker.git
cd AltTasker

# Run the installer (requires sudo)
sudo ./scripts/install.sh

# Run it!
task
```

The installer will:
- ✅ Build the project automatically
- ✅ Install to `/usr/local/bin/task`
- ✅ Set up Ctrl+Alt+Delete hotkey (optional, systemd required)
- ✅ Create systemd service for hotkey integration

**Works on:** Any Linux distribution with systemd (Ubuntu, Debian, Fedora, Arch, etc.) and WSL2

**Advanced Installation Options:**
```bash
# Skip Ctrl+Alt+Delete setup
sudo ./scripts/install.sh --no-hotkey

# Non-interactive mode (for automation)
sudo ./scripts/install.sh --unattended

# Show help
./scripts/install.sh --help
```

### Option B: Manual Build (Development)

For development or testing without system installation:

```bash
# Compile the project
make

# Run directly
./alttasker

# Or create local alias
alias task='./alttasker'
```

### Uninstallation

Remove AltTasker completely:

```bash
# Standard uninstallation
sudo ./scripts/uninstall.sh

# Keep backup files
sudo ./scripts/uninstall.sh --keep-backups

# Non-interactive mode
sudo ./scripts/uninstall.sh --unattended
```

---

## 📁 Project Structure

```
AltTasker/
│
├── 📄 README.md                    # Main documentation - what it is, how to install, examples
├── 📄 LICENSE                      # MIT License
├── 📄 .gitignore                   # Ignore files (.o, executables)
├── 📄 Makefile                     # Compilation, installation, cleanup
│
├── 📁 src/                         # *** C Source Code ***
│   ├── 📄 main.c                  # Main entry point, main loop
│   ├── 📄 process_monitor.c       # Reading /proc, parsing process information
│   ├── 📄 display.c               # Terminal UI display, colors, formatting
│   └── 📄 signal_handler.c        # Signal handling (Ctrl+C, cleanup)
│
├── 📁 include/                     # *** Header Files ***
│   ├── 📄 process_monitor.h       # Struct definitions, monitor functions
│   ├── 📄 display.h               # Display functions
│   ├── 📄 signal_handler.h        # Signal handling functions
│   └── 📄 common.h                # General definitions, macros, constants
│
├── 📁 scripts/                     # *** Bash Scripts ***
│   ├── 📄 install.sh              # Professional installation with systemd integration
│   └── 📄 uninstall.sh            # Complete removal and system restoration
│
├── 📁 tests/                       # *** Tests ***
│   ├── 📄 test_features.py        # Automated feature testing (18 tests)
│   ├── 📄 test_performance.py     # Performance benchmarks
│   ├── � test_basic.sh           # Basic shell tests
│   └── � README.md               # Testing documentation
│
├── 📁 docs/                        # *** Detailed Documentation ***
│   ├── 📄 RUNNING-IN-WSL-QEMU.md  # WSL and QEMU setup guide
│   └── 📄 INTERACTIVE-COMMANDS.md # Complete interactive command reference
```

---

## ⚙️ System Requirements

### Linux (Native or WSL)
- **OS**: Any Linux distribution (Ubuntu, Debian, Fedora, Arch, RHEL, etc.) or WSL2
- **Kernel**: Linux kernel with `/proc` filesystem support
- **Build Tools**: gcc, make
- **Optional**: systemd (for Ctrl+Alt+Delete hotkey integration)

### Terminal Requirements
AltTasker requires a terminal emulator that supports ANSI escape sequences:
- ✅ **Linux native terminals** (GNOME Terminal, Konsole, xterm, etc.)
- ✅ **WSL2** with Windows Terminal or native WSL terminal
- ✅ **SSH sessions** to Linux servers
- ❌ **PowerShell/CMD** (limited ANSI support, use WSL instead on Windows)

---

## 🎨 Usage

### Running AltTasker

```bash
# If installed system-wide
task

# Or run directly from build directory
./alttasker

# Exit anytime
Ctrl+C or Q
```

### Interactive Commands

Once running, use these keyboard shortcuts:

#### Sorting
- `P` - Sort by PID (Process ID)
- `C` - Sort by CPU usage
- `M` - Sort by Memory usage (default)
- `U` - Sort by User/Owner

#### Navigation
- `↑` / `↓` - Scroll up/down one line
- `Page Up` / `Page Down` - Scroll one page (configurable, default: 20)
- `Home` - Jump to top of list
- `End` - Jump to bottom of list

#### Customization (v2.3) 🎨
- `T` - Cycle color themes (default → dark → light → colorblind → custom)
- `V` - Toggle process tree view (show parent-child hierarchy)

#### Filtering & Actions
- `F` - Filter processes by username
- `R` - Reset all filters
- `K` - Kill selected process (requires sudo)
- `S` - Search for process by name
- `Q` - Quit application

### Configuration File (v2.3)

AltTasker automatically creates `~/.alttaskerrc` on first run. Edit this file to customize:

```ini
# Refresh interval in seconds (1-10)
refresh_interval=2

# Number of visible processes per page (10-100)
visible_processes=20

# Default sort key: P (PID), C (CPU), M (Memory), U (User)
default_sort=M

# Show process tree view: true or false
show_tree_view=false

# Color theme: default, dark, light, colorblind, custom
theme=default
```

### What You'll See

```
╔════════════════════════════════════════════════════════════════╗
║                    ALTTASKER - Process Monitor                 ║
╠════════════════════════════════════════════════════════════════╣
║  Uptime: 5d 12h 34m  |  Processes: 347  |  Memory: 65.2%      ║
║  [████████████████░░░░░░░░░░] 12.4 / 19.0 GB                  ║
╠════════════════════════════════════════════════════════════════╣
║  PID    USER      CPU%   MEM%    VSZ      RSS    STATE  CMD   ║
╠════════════════════════════════════════════════════════════════╣
║  1234   root      12.5   15.2    2.4G     1.2G   R      chrome║
║  5678   user      5.3    8.1     1.8G     890M   S      code  ║
║  ...                                                            ║
╠════════════════════════════════════════════════════════════════╣
║  ▼ More below - Showing 1-20 of 347 processes                 ║
╠════════════════════════════════════════════════════════════════╣
║  Sort: P(ID) C(PU) M(em) U(ser) | Filter: F R | Kill: K       ║
║  Navigation: ↑/↓ Line  PgUp/PgDn Page  Home/End Top/Bottom   ║
╚════════════════════════════════════════════════════════════════╝
```

### Color Coding
- 🟢 **Green** - Running processes (R) / Low usage (<2%)
- 🟡 **Yellow** - Disk wait (D) / Medium usage (2-5%)
- 🔴 **Red** - Zombie processes (Z) / High usage (>5%)
- 🔵 **Cyan** - Headers, borders, and system info

---

## 🛠️ Build Commands

```bash
# Standard build
make          # Compile the project

# Development
make clean    # Clean build artifacts
make rebuild  # Clean + compile
make run      # Compile and run directly

# System installation (use scripts instead)
# sudo make install  # Deprecated - use ./scripts/install.sh
```

**Recommended:** Use the installation scripts for system-wide setup:
```bash
sudo ./scripts/install.sh    # Full installation with optional hotkey
sudo ./scripts/uninstall.sh  # Complete removal
```

---

## 📚 Additional Documentation

- **[USAGE.md](USAGE.md)** - Detailed command reference and usage examples
- **[SETUP.md](SETUP.md)** - Setup guide for different Linux environments
- **[QUICKSTART.md](QUICKSTART.md)** - Quick reference card
- **[CHANGELOG.md](CHANGELOG.md)** - Version history and changes
- **[FEATURES-v2.2.md](FEATURES-v2.2.md)** - Latest features (scrolling navigation)
- **[docs/RUNNING-IN-WSL-QEMU.md](docs/RUNNING-IN-WSL-QEMU.md)** - WSL and QEMU setup (for Windows users)
- **[docs/INTERACTIVE-COMMANDS.md](docs/INTERACTIVE-COMMANDS.md)** - Complete command reference

---

## 🐛 Troubleshooting

### Installation Issues

#### "Permission denied" during installation
```bash
# Make sure scripts are executable
chmod +x scripts/install.sh scripts/uninstall.sh

# Run with sudo
sudo ./scripts/install.sh
```

#### "gcc: command not found" or build errors
```bash
# Install build tools

# Debian/Ubuntu/WSL
sudo apt update
sudo apt install build-essential

# Fedora/RHEL/CentOS
sudo dnf install gcc make

# Arch Linux
sudo pacman -S base-devel
```

#### "systemd not found" warning
This is normal if you're on:
- WSL1 (upgrade to WSL2 for systemd support)
- Systems without systemd (Alpine Linux, older distributions, containers)

The installer will skip Ctrl+Alt+Delete setup, but the main application will work perfectly. You can still run AltTasker manually with the `task` command.

### Runtime Issues

#### "task: command not found" after installation
```bash
# Check if binary is installed
which task
ls -la /usr/local/bin/task

# If missing, reinstall
sudo ./scripts/install.sh
```

#### Screen is scrolling instead of refreshing
You're running from PowerShell! Switch to WSL terminal:
```bash
wsl
task
```

#### Ctrl+Alt+Delete hotkey not working
```bash
# Verify service is enabled (requires systemd)
sudo systemctl status alttasker-hotkey.service

# Check systemd override
ls -la /etc/systemd/system/ctrl-alt-del.target.d/

# Re-enable if needed
sudo systemctl daemon-reload
sudo systemctl enable alttasker-hotkey.service
```

**Note:** Ctrl+Alt+Delete integration only works on:
- Native Linux systems with systemd
- Console/TTY mode (not in GUI desktop sessions)
- WSL2 with systemd enabled (requires configuration)

#### Navigation keys not working
Make sure you're in a proper terminal emulator that supports ANSI escape sequences:
- ✅ Linux native terminals (GNOME Terminal, Konsole, xterm, Alacritty, etc.)
- ✅ WSL2 with Windows Terminal
- ✅ SSH sessions to Linux servers
- ❌ PowerShell (limited support - use WSL on Windows)
- ❌ CMD (no support)

#### Running on Windows
AltTasker is a **Linux application**. On Windows, you must use WSL (Windows Subsystem for Linux):
```bash
# Install WSL if not already installed (PowerShell as Administrator)
wsl --install

# Then run in WSL
wsl
cd /path/to/AltTasker
sudo ./scripts/install.sh
```

### Uninstallation Issues

#### Can't remove AltTasker
```bash
# Force uninstallation (will attempt cleanup)
sudo ./scripts/uninstall.sh --unattended

# Manual cleanup if needed
sudo rm -f /usr/local/bin/task
sudo systemctl disable alttasker-hotkey.service
sudo rm -f /etc/systemd/system/alttasker-hotkey.service
sudo rm -rf /etc/systemd/system/ctrl-alt-del.target.d/
sudo systemctl daemon-reload
```

---

## 📝 License

MIT License - See [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Contributions welcome! Please feel free to submit issues or pull requests.

---

**Made with ❤️ for Linux system monitoring**

*AltTasker is designed for Linux systems. Windows users can run it via WSL (Windows Subsystem for Linux).*

## 📋 File Descriptions

### Core Source Files (`src/`)
- **main.c** - Application entry point, initializes components and runs the main event loop
- **process_monitor.c** - Reads and parses `/proc` filesystem to gather process information (PID, CPU, memory, etc.)
- **display.c** - Handles terminal UI rendering with ANSI colors and formatted output
- **signal_handler.c** - Manages system signals (SIGINT, SIGTERM) for graceful shutdown

### Header Files (`include/`)
- **process_monitor.h** - Data structures and function prototypes for process monitoring
- **display.h** - UI rendering function declarations
- **signal_handler.h** - Signal handling function declarations
- **common.h** - Shared constants, macros, and type definitions used across the project

### Scripts (`scripts/`)
- **install.sh** - Professional installation script with systemd integration, Ctrl+Alt+Delete hotkey setup, and automated build
- **uninstall.sh** - Complete removal script that restores system defaults and cleans up all installed components

### Tests (`tests/`)
- **test_features.py** - Comprehensive Python test suite with 18 automated tests covering all features
- **test_performance.py** - Performance benchmarking script for system monitoring overhead
- **test_basic.sh** - Shell script for basic functional testing
- **README.md** - Testing documentation and guidelines

### Documentation (`docs/`)
- **RUNNING-IN-WSL-QEMU.md** - Guide for running in WSL and QEMU environments
- **INTERACTIVE-COMMANDS.md** - Complete reference for all interactive commands and keyboard shortcuts
