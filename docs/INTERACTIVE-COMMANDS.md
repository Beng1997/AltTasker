# AltTasker Interactive Commands Guide

## Overview
AltTasker provides an interactive command-line interface for monitoring and managing Linux processes. This guide explains all available commands and features.

## Command Reference

### Sorting Commands

| Command | Description | Sort Order |
|---------|-------------|------------|
| **P** (or **p**) | Sort by Process ID | Ascending (lowest PID first) |
| **C** (or **c**) | Sort by CPU Usage | Descending (highest CPU first) |
| **M** (or **m**) | Sort by Memory Usage | Descending (highest memory first) |
| **U** (or **u**) | Sort by Username | Alphabetical (A-Z) |

**Note**: All sorting commands are case-insensitive.

### Filter Commands

#### F - Filter by User
Filter the process list to show only processes owned by a specific user.

**How to use**:
1. Press **F** (or **f**)
2. The display will clear and prompt: `Enter username to filter (or press Enter for all):`
3. Type the username (e.g., `root`, `beng1997`, etc.)
4. Press **Enter**
5. Only processes from that user will be displayed

**Example**:
```
Enter username to filter (or press Enter for all): root
```
Result: Shows only processes owned by `root`

**Tips**:
- Leave empty and press Enter to cancel filtering
- Use **R** command to quickly reset the filter
- Usernames are case-sensitive

#### R - Reset Filter
Clears any active filter and shows all processes again.

**How to use**:
- Simply press **R** (or **r**)
- All processes will be displayed immediately

### Exit Commands

| Command | Description |
|---------|-------------|
| **Q** (or **q**) | Quit AltTasker |
| **Ctrl+C** | Exit (same as Q) |

Both commands ensure graceful exit with proper terminal restoration.

## Display Information

### System Information Section
Located at the top of the screen:
- **Uptime**: Shows how long the system has been running (format: Xd Xh Xm Xs)
- **Memory Bar**: Visual progress bar showing memory usage
- **Memory Stats**: Used/Total memory with percentage

### Process Table Columns

| Column | Description | Format |
|--------|-------------|--------|
| **PID** | Process ID | Integer (1-99999) |
| **USER** | Process owner | Username (up to 10 chars) |
| **CPU%** | CPU usage percentage | Decimal (0.0-999.9) |
| **MEM%** | Memory usage percentage | Decimal (0.00-99.99) |
| **VIRT** | Virtual memory size | Human-readable (KB/MB/GB) |
| **RES** | Resident Set Size (physical RAM) | Human-readable (KB/MB/GB) |
| **STATE** | Process state | RUN/SLEEP/DISK/ZOMBI/STOP/IDLE |
| **COMMAND** | Process name or command | Truncated to 45 chars |

### Process State Colors

| State | Color | Meaning |
|-------|-------|---------|
| **RUN** | 🟢 Green | Currently running on CPU |
| **SLEEP** | ⚪ White | Sleeping (waiting for event) |
| **DISK** | 🟡 Yellow | Waiting for disk I/O |
| **ZOMBI** | 🔴 Red | Zombie process (finished but not cleaned up) |
| **STOP** | 🔵 Cyan | Stopped (paused) |
| **IDLE** | 🔵 Blue | Idle kernel thread |

### Memory Usage Colors

Process rows are colored based on memory usage:
- **🔴 Red**: High memory usage (> 5%)
- **🟡 Yellow**: Moderate memory usage (2-5%)
- **⚪ White**: Low memory usage (< 2%)

## Command Menu

The command menu is always visible at the bottom of the screen:

```
╔══════════════════════════════════════════════════════════════════════════════════════╗
║ Commands                                                                              ║
╠══════════════════════════════════════════════════════════════════════════════════════╣
║ Sort: PID  CPU  Memory  User   Current: MEM↓                                         ║
║ Filter: F User  R Reset                                                               ║
║ Actions: K Kill process  S Search  Q/Ctrl+C Quit                                      ║
╚══════════════════════════════════════════════════════════════════════════════════════╝
Auto-refresh: 2s  |  Press any key above to execute
```

The `Current:` indicator shows your active sorting mode with a ↓ arrow.

## Tips and Best Practices

### Performance Tips
1. **Default Sorting**: Memory sorting (M) is default because it's most useful for finding resource hogs
2. **Process Limit**: Only 20 processes are shown to keep the display readable
3. **Auto-Refresh**: The display updates every 2 seconds automatically
4. **Instant Response**: Commands execute immediately without waiting for next refresh

### Workflow Examples

#### Find Memory-Hungry Processes
1. Launch `task` (memory sort is default)
2. Look at the top processes - they use the most RAM
3. Identify high memory usage by red-colored rows

#### Monitor Your Own Processes
1. Press **F**
2. Enter your username
3. Press **M** to sort by memory
4. Watch your processes in real-time

#### Find Specific User's Activity
1. Press **F**
2. Enter username (e.g., `root`)
3. Press **C** to sort by CPU
4. See what that user's processes are doing

#### Check System-Wide Activity
1. Press **R** to ensure no filter is active
2. Press **U** to sort by user
3. See which users have the most processes

### Common Use Cases

| Task | Commands |
|------|----------|
| Find memory leaks | **M** (default), look for growing memory usage |
| Find CPU hogs | **C**, check top processes |
| Monitor specific user | **F** → enter username |
| Check all processes by user | **R** → **U** |
| Quick system overview | Just launch `task` (default is memory sort) |

## Keyboard Input Notes

- **Case-Insensitive**: All commands work with uppercase or lowercase
- **Instant Execution**: Commands execute immediately when you press the key
- **No Enter Needed**: Except for filter username input
- **Non-Blocking**: The display continues updating while waiting for input

## Troubleshooting

### Filter Not Working
- Make sure you typed the username correctly (case-sensitive!)
- Check if that user has any running processes
- Press **R** to reset and verify processes exist

### Display Not Refreshing
- Make sure you're running in a native WSL terminal
- PowerShell may not support terminal control codes properly
- Try `wsl` first, then run `task`

### Terminal Looks Weird After Exit
- This shouldn't happen with proper cleanup
- If it does: type `reset` and press Enter
- Or close and reopen your terminal

## Advanced Features (Coming Soon)

The command menu shows planned features:
- **K**: Kill process (interactive PID entry)
- **S**: Search for process by name

These features will be implemented in future versions.

---

For more information, see the main [README.md](../README.md) file.
