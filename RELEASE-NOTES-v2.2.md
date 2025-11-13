# AltTasker v2.2 Release Notes

**Release Date:** November 13, 2025

## 🎉 What's New in v2.2

### ✨ Major Features

#### **Scrolling Navigation** 🎯
Navigate through large process lists with ease using keyboard controls:
- **Arrow Keys** (↑/↓) - Scroll line by line
- **Page Up/Down** - Jump 20 lines at a time
- **Home/End** - Jump to top/bottom of list
- **Visual Indicators** - See "▲ More above" and "▼ More below"
- **Position Display** - "Showing X-Y of Z processes"

#### **Professional Installation System** 📦
- Automated `install.sh` script with systemd integration
- Optional Ctrl+Alt+Delete hotkey support
- Complete `uninstall.sh` for clean removal
- Multi-distribution support (Ubuntu, Debian, Fedora, Arch)
- Interactive and unattended installation modes

### 🔧 Improvements

#### **Enhanced Display**
- Auto-reset scroll position when sorting/filtering changes
- Smart bounds checking prevents scrolling past list limits
- Navigation help shown when process list exceeds 20 items
- Cleaner, more intuitive user interface

#### **Testing & Quality**
- Comprehensive Python test suite (18 automated tests)
- Performance benchmarking tools
- All tests passing with zero warnings
- Graceful handling of dependencies

#### **Documentation**
- Comprehensive README with Linux-first emphasis
- Multi-distro installation instructions
- Enhanced troubleshooting guide
- Complete command reference documentation

### 📊 Performance

All metrics excellent in testing:
- **Memory Usage:** 3.38 MB (< 10 MB target) ✅
- **CPU Usage:** 0.00% during normal operation ✅
- **Startup Time:** ~103 ms (< 200 ms target) ✅
- **Scrolling:** 10.21 ms per operation (< 50 ms target) ✅
- **Refresh Overhead:** 0.00% CPU (< 3% target) ✅

### 🐛 Bug Fixes

- Fixed terminal control sequence handling for arrow keys
- Improved escape sequence parsing reliability
- Enhanced signal handling for clean shutdown
- Better error handling in process monitoring

---

## 📥 Installation

### Quick Install (Recommended)

```bash
# Clone the repository
git clone https://github.com/Beng1997/AltTasker.git
cd AltTasker

# Install system-wide
sudo ./scripts/install.sh

# Run it!
task
```

### Manual Build

```bash
# Compile
make

# Run directly
./alttasker
```

---

## 🎯 Full Feature List

### Core Features
- Real-time process monitoring from `/proc` filesystem
- Colorful ANSI terminal UI with progress bars
- Memory usage visualization
- System uptime and process count display

### Interactive Commands
- **Sorting:** P (PID), C (CPU), M (Memory), U (User)
- **Navigation:** ↑/↓ (line), PgUp/PgDn (page), Home/End
- **Filtering:** F (by user), R (reset filters)
- **Actions:** K (kill process), S (search), Q (quit)

### Installation Features
- System-wide installation to `/usr/local/bin/task`
- Optional systemd service creation
- Ctrl+Alt+Delete hotkey integration (requires systemd)
- Complete uninstallation with backup restoration

---

## 🔄 Upgrading from v2.1

If you have v2.1 installed:

```bash
# Pull latest changes
git pull origin main

# Rebuild and reinstall
make clean
sudo ./scripts/install.sh
```

Your configuration and systemd settings will be preserved.

---

## 📋 System Requirements

- **OS:** Linux (any distribution) or WSL2
- **Kernel:** Linux kernel with `/proc` filesystem
- **Build Tools:** gcc, make
- **Optional:** systemd (for hotkey integration)
- **Terminal:** ANSI escape sequence support

---

## 🙏 Acknowledgments

Thanks to all users and contributors who provided feedback and suggestions for v2.2!

---

## 📝 Full Changelog

See [CHANGELOG-v2.2.md](CHANGELOG-v2.2.md) for detailed changes.

---

## 🐛 Known Issues

None reported for v2.2. Please report issues on [GitHub Issues](https://github.com/Beng1997/AltTasker/issues).

---

## 🚀 What's Next?

Potential features for v2.3:
- Process tree view
- Advanced filtering options
- Configuration file support
- Custom color themes
- CPU percentage per process

---

**Made with ❤️ for Linux system monitoring**
