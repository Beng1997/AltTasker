# 🚀 AltTasker Quick Start Guide

## What's New (Latest Update v2.1)

✨ **Fixed Display Issues:**
- ✅ **Screen properly refreshes in place** (no more scrolling!)
- ✅ Uses alternate screen buffer (like htop/vim)
- ✅ Better color coding for process states
- ✅ STATE column shows descriptive text (RUN, SLEEP, DISK, ZOMBI, etc.)
- ✅ Improved table alignment
- ✅ Fixed terminal cleanup on exit

🎨 **Enhanced Colors:**
- Green: Running processes, low usage
- Yellow: Disk wait, medium usage (2-5% memory)
- Red: Zombie/dead processes, high usage (>5% memory)
- Cyan: Headers and system info
- Blue: Idle processes

## Quick Run

### Windows (with WSL)

**Double-click this file:**
```
RUN-IN-WSL.bat
```

**Or from PowerShell/CMD:**
```powershell
wsl bash /mnt/d/AltTasker/scripts/run-in-wsl.sh
```

### From WSL Terminal

```bash
wsl
cd /mnt/d/AltTasker
make && ./alttasker
```

## Features

✅ **System Monitoring:**
- Real-time memory usage with visual progress bar
- System uptime
- Total process count
- Color-coded metrics (Green < 50%, Yellow < 75%, Red > 75%)

✅ **Process Information:**
- PID, User, CPU%, Memory%
- Virtual memory size (VIRT)
- Resident memory (RES)
- Process state with colors
- Full command line

✅ **User Experience:**
- Clean, htop-style interface
- No scrolling - updates in place
- Top 30 processes shown
- Easy exit with Ctrl+C

## Color Coding

| Color | Meaning |
|-------|---------|
| 🟢 Green | Running process / Low usage (<50%) |
| 🟡 Yellow | Disk wait / Medium usage (50-75%) |
| 🔴 Red | Zombie/Dead process / High usage (>75%) |
| 🔵 Cyan | Headers and borders |

## Process States

- **R** - Running (Green)
- **S** - Sleeping (default)
- **D** - Disk wait (Yellow)
- **Z** - Zombie (Red)
- **T** - Stopped (Cyan)
- **I** - Idle (Blue)

## Keyboard Controls

- **Ctrl+C** - Exit application gracefully

## Troubleshooting

### Colors not showing?
Your terminal might not support ANSI colors. Try:
- Windows Terminal (recommended)
- Native WSL terminal
- VS Code terminal

### Screen scrolling instead of updating?
This has been fixed! If you still see issues:
```bash
cd /mnt/d/AltTasker
make clean && make
```

### Permission errors?
```bash
wsl -- chmod +x /mnt/d/AltTasker/alttasker
```

## More Information

See `docs/RUNNING-IN-WSL-QEMU.md` for advanced setup options including QEMU.

---

**Version:** 2.0 (Updated UI)  
**Date:** November 2025
