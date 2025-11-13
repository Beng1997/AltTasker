# AltTasker

A lightweight, `htop`-style Linux task monitor written in C. Monitor your system processes with a beautiful, colorful terminal interface.

## ✨ Features

### Core Functionality
- 📊 **Real-time process monitoring** - Live system process information
- 🎨 **Colorful terminal UI** - Beautiful ANSI colors with progress bars
- 💾 **Memory usage visualization** - Visual bars showing memory consumption
- ⚡ **Fast and efficient** - Lightweight C implementation with minimal overhead
- 🎯 **Top processes view** - Shows top 20 processes by memory usage

### Interactive Commands (v2.2)
- 🔄 **Sorting** - Sort by PID (P), CPU (C), Memory (M), User (U)
- 🔍 **Filtering** - Filter by username (F), reset filters (R)
- 🎮 **Process Control** - Kill processes (K), search (S)
- 📜 **Scrolling** - Navigate with Arrow keys, Page Up/Down, Home/End
- ⌨️ **Hotkey Integration** - Optional Ctrl+Alt+Delete support (systemd)

### Display Features
- ✨ **Visual indicators** - ▲ More above / ▼ More below
- 📍 **Position tracking** - "Showing X-Y of Z processes"
- 🎨 **Color coding** - Green (running), Yellow (disk wait), Red (zombie)
- 📊 **System overview** - Uptime, process count, memory usage bar

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
- ✅ Set up Ctrl+Alt+Delete hotkey (optional)
- ✅ Create systemd service for hotkey integration

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
│   ├── 📄 install.sh              # Compilation + installation + systemd modification
│   ├── 📄 uninstall.sh            # Removal + restore defaults
│   └── 📄 test_vm.sh              # Run tests in QEMU (optional)
│
├── 📁 tests/                       # *** Tests ***
│   ├── 📄 test_basic.sh           # Basic functional tests
│   ├── 📄 test_parser.c           # Test /proc parsing
│   └── 📁 mock_proc/              # /proc simulation for testing
│       ├── 📁 1/                  # Mock process PID=1
│       │   ├── stat
│       │   ├── status
│       │   └── cmdline
│       └── 📁 1234/               # Mock process PID=1234
│           ├── stat
│           ├── status
│           └── cmdline
│
├── 📁 docs/                        # *** Detailed Documentation ***
│   ├── 📄 ARCHITECTURE.md         # Architecture, design decisions
│   ├── 📄 INSTALL.md              # Step-by-step installation instructions
│   ├── 📄 TESTING.md              # How to run tests
│   └── 📄 CONTRIBUTING.md         # Contributor guidelines (optional)
│
└── 📁 examples/                    # *** Usage Examples (optional) ***
    ├── 📄 config.example          # Example configuration file
    └── 📄 screenshot.png          # Screenshot of the software in action
```

---

## ⚠️ IMPORTANT: Terminal Requirements

**✅ DO:**
- Run from **WSL terminal** (native Linux terminal)
- Use Windows Terminal with WSL profile
- Run `wsl` command first, then `task`

**❌ DON'T:**
- Run from PowerShell (causes scrolling issues)
- Run from CMD
- Use `RUN-IN-WSL.bat` (deprecated)

### Why?
PowerShell doesn't properly support ANSI escape codes and terminal control sequences. This causes the display to scroll instead of refresh in place.

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

#### Navigation (v2.2)
- `↑` / `↓` - Scroll up/down one line
- `Page Up` / `Page Down` - Scroll one page (20 lines)
- `Home` - Jump to top of list
- `End` - Jump to bottom of list

#### Filtering & Actions
- `F` - Filter processes by username
- `R` - Reset all filters
- `K` - Kill selected process (requires sudo)
- `S` - Search for process by name
- `Q` - Quit application

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
- **[SETUP.md](SETUP.md)** - Detailed setup guide for different environments
- **[QUICKSTART.md](QUICKSTART.md)** - Quick reference card
- **[CHANGELOG.md](CHANGELOG.md)** - Version history and changes
- **[FEATURES-v2.2.md](FEATURES-v2.2.md)** - Latest features (scrolling navigation)
- **[docs/INSTALL.md](docs/INSTALL.md)** - Advanced installation scenarios
- **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Technical architecture
- **[docs/TESTING.md](docs/TESTING.md)** - Testing procedures
- **[docs/RUNNING-IN-WSL-QEMU.md](docs/RUNNING-IN-WSL-QEMU.md)** - Advanced topics

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
sudo apt update
sudo apt install build-essential
```

#### "systemd not found" warning
This is normal if you're on WSL1 or a system without systemd. The installer will skip Ctrl+Alt+Delete setup but the main application will work fine.

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
# Verify service is enabled
sudo systemctl status alttasker-hotkey.service

# Check systemd override
ls -la /etc/systemd/system/ctrl-alt-del.target.d/

# Re-enable if needed
sudo systemctl daemon-reload
sudo systemctl enable alttasker-hotkey.service
```

#### Navigation keys not working
Make sure you're in a proper terminal emulator that supports ANSI escape sequences:
- ✅ WSL Terminal
- ✅ Windows Terminal
- ✅ Linux native terminals
- ❌ PowerShell (limited support)
- ❌ CMD (no support)

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
- **test_vm.sh** - Optional script to run tests in a QEMU virtual machine for safe testing

### Tests (`tests/`)
- **test_basic.sh** - Shell script for basic functional testing
- **test_parser.c** - Unit tests for `/proc` parsing logic
- **mock_proc/** - Simulated `/proc` directory structure for isolated testing without system access

### Documentation (`docs/`)
- **ARCHITECTURE.md** - Technical architecture overview and design rationale
- **INSTALL.md** - Comprehensive installation guide with troubleshooting
- **TESTING.md** - Testing procedures and test suite documentation
- **CONTRIBUTING.md** - Guidelines for contributors (coding standards, PR process)

### Examples (`examples/`)
- **config.example** - Sample configuration file demonstrating available options
- **screenshot.png** - Visual example of AltTasker in action
