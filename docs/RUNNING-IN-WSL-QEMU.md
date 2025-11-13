# Running AltTasker in WSL and QEMU

## Quick Start Guide

AltTasker is an htop-like process monitor for Linux. This guide explains how to run it in WSL (Windows Subsystem for Linux) and optionally in QEMU.

## Option 1: Run in WSL (Recommended) ✅

This is the easiest and fastest way to test the application.

### Method A: Using Batch File (Windows)

Simply double-click or run:
```batch
RUN-IN-WSL.bat
```

### Method B: Direct Command

From PowerShell or CMD:
```powershell
wsl bash /mnt/d/AltTasker/scripts/run-in-wsl.sh
```

### Method C: From WSL Terminal

```bash
# Open WSL terminal
wsl

# Navigate to project
cd /mnt/d/AltTasker

# Compile and run
make clean && make
./alttasker
```

## Option 2: Run in QEMU (Advanced)

QEMU requires a full virtualized Linux environment.

### Prerequisites

1. **Install QEMU in WSL**:
   ```bash
   wsl
   sudo apt update
   sudo apt install qemu-system-x86 qemu-utils
   ```

2. **Download a Linux ISO** (Alpine Linux recommended - lightweight):
   ```bash
   cd /mnt/d/AltTasker/qemu-env
   wget https://dl-cdn.alpinelinux.org/alpine/v3.19/releases/x86_64/alpine-virt-3.19.0-x86_64.iso
   ```

3. **Create Virtual Disk**:
   ```bash
   qemu-img create -f qcow2 alpine-disk.qcow2 2G
   ```

4. **Install Linux in QEMU**:
   ```bash
   qemu-system-x86_64 \
     -m 512M \
     -cdrom alpine-virt-3.19.0-x86_64.iso \
     -boot d \
     -hda alpine-disk.qcow2 \
     -nographic
   ```

   Inside the VM, run:
   ```bash
   # Login as root
   setup-alpine
   # Follow the installation wizard
   ```

5. **Boot the Installed System**:
   ```bash
   qemu-system-x86_64 \
     -m 512M \
     -hda alpine-disk.qcow2 \
     -nographic
   ```

6. **Copy AltTasker to VM**:
   
   **Method A: Network Sharing (Recommended)**
   ```bash
   # From WSL, start HTTP server
   cd /mnt/d/AltTasker
   python3 -m http.server 8000
   
   # From QEMU (add -netdev user,id=net0 -device e1000,netdev=net0 to qemu command)
   wget http://10.0.2.2:8000/alttasker
   chmod +x alttasker
   ./alttasker
   ```

### Full QEMU Command

```bash
qemu-system-x86_64 \
  -m 512M \
  -hda alpine-disk.qcow2 \
  -netdev user,id=net0 \
  -device e1000,netdev=net0 \
  -nographic
```

## Comparison: WSL vs QEMU

| Feature | WSL | QEMU |
|---------|-----|------|
| Speed | ⚡ Very fast | 🐢 Slower |
| Ease of use | ✅ Very simple | ❌ Complex |
| Testing | ✅ Excellent | ✅ Excellent |
| Full simulation | ❌ Shares kernel with Windows | ✅ Full VM |
| Resource usage | 💚 Low | 💛 Medium-High |

## Recommendation

**For Testing & Development:** Use WSL (Option 1)
- Fastest and easiest
- Reliable results
- Less configuration

**For Full Simulation:** Use QEMU (Option 2)
- Need different Linux kernel
- Want complete isolation
- Deployment testing

## Troubleshooting

### AltTasker won't compile
```bash
wsl -- bash -c "cd /mnt/d/AltTasker && make clean && make"
```

### Permission denied
```bash
wsl -- chmod +x /mnt/d/AltTasker/alttasker
```

### QEMU not installed
```bash
wsl
sudo apt update
sudo apt install qemu-system-x86 qemu-utils
```

### Display issues
- Ensure terminal supports ANSI escape codes
- Try running from native WSL terminal instead of PowerShell

## Keyboard Shortcuts

- **Ctrl+C** - Exit AltTasker
- **Ctrl+A, X** - Exit QEMU (in -nographic mode)

## Helper Files

- `RUN-IN-WSL.bat` - Quick run from Windows
- `scripts/run-in-wsl.sh` - WSL run script
- `scripts/run-in-qemu.sh` - QEMU info script

---

**Created for:** AltTasker - Linux Task Monitor  
**Date:** November 2025
