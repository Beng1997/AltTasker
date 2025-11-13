# AltTasker

A lightweight, `htop`-style Linux task monitor written in C. Monitor your system processes with a beautiful, colorful terminal interface.

## ✨ Features

- 📊 Real-time process monitoring
- 🎨 Colorful terminal UI with progress bars
- 💾 Memory usage visualization  
- ⚡ Fast and efficient C implementation
- 🔄 Auto-refresh every 2 seconds
- 🎯 Shows top 25 processes by memory usage

## 🚀 Quick Start (3 Simple Steps!)

### 1. Open WSL Terminal (Important!)
```bash
# Open WSL - NOT PowerShell!
wsl
```

### 2. Compile the Project
```bash
cd /mnt/d/AltTasker
make
```

### 3. Install and Run
```bash
# Install the 'task' command
./scripts/install-alias.sh
source ~/.bashrc

# Run it!
task
```

**That's it!** Press Ctrl+C to exit.

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

```bash
# Start monitoring
task

# Exit
Ctrl+C
```

### What You'll See

- 🟢 **System Info**: Uptime, total processes, memory usage with visual bar
- 📋 **Process Table**: Top 25 processes sorted by memory
- 🎨 **Color Coding**:
  - Green: Running processes / Low usage
  - Yellow: Disk wait / Medium usage (2-5%)
  - Red: Zombie processes / High usage (>5%)
  - Cyan: Headers and borders

---

## 🛠️ Build Commands

```bash
make          # Compile the project
make clean    # Clean build artifacts
make rebuild  # Clean + compile
make run      # Compile and run directly
make install  # Install to /usr/local/bin (requires sudo)
```

---

## 📚 Additional Documentation

- **[SETUP.md](SETUP.md)** - Detailed setup guide
- **[QUICKSTART.md](QUICKSTART.md)** - Quick reference
- **[docs/RUNNING-IN-WSL-QEMU.md](docs/RUNNING-IN-WSL-QEMU.md)** - Advanced topics

---

## 🐛 Troubleshooting

### "task: command not found"
```bash
source ~/.bashrc
# Or close and reopen your terminal
```

### Screen is scrolling instead of refreshing
You're running from PowerShell! Switch to WSL terminal:
```bash
wsl
task
```

### Permission denied
```bash
chmod +x /mnt/d/AltTasker/alttasker
```

### Compile errors
```bash
# Make sure you have build tools
sudo apt update
sudo apt install build-essential
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
- **install.sh** - Automates compilation, binary installation, and systemd configuration
- **uninstall.sh** - Removes the application and restores original system settings
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
