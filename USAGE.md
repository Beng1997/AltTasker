# 🎮 AltTasker Usage Guide

## Interactive Commands

AltTasker provides a rich interactive CLI for real-time process monitoring and management.

### Keyboard Commands

| Key | Action | Description |
|-----|--------|-------------|
| **P** | Sort by PID | Sort processes by Process ID (ascending) |
| **C** | Sort by CPU | Sort by CPU usage (descending, highest first) |
| **M** | Sort by Memory | Sort by memory usage (descending, **default**) |
| **U** | Sort by User | Sort by username (alphabetical) |
| **↑** | Scroll Up | Move up one line in the process list |
| **↓** | Scroll Down | Move down one line in the process list |
| **Page Up** | Page Up | Scroll up 20 lines (one page) |
| **Page Down** | Page Down | Scroll down 20 lines (one page) |
| **Home** | Jump to Top | Go to the first process |
| **End** | Jump to Bottom | Go to the last process |
| **F** | Filter by User | Interactive prompt to filter processes by username |
| **R** | Reset Filter | Clear any active filter, show all processes |
| **K** | Kill Process | Interactive prompt to kill a process by PID |
| **S** | Search | Search for processes by name or command |
| **Q** | Quit | Exit AltTasker |
| **Ctrl+C** | Exit | Same as Q, gracefully exits |

### How to Navigate Through Processes

When you have more than 20 processes, AltTasker shows only 20 at a time. Use these keys to navigate:

1. **Arrow Keys (↑/↓)** - Scroll line by line
2. **Page Up/Page Down** - Jump 20 lines at a time
3. **Home** - Go to the very first process
4. **End** - Go to the very last process

**Scroll Indicators**:
- Status shows your position: `Showing 1-20 of 150 processes`
- Visual indicators appear when there's more to see:
  - `▲ More above` - Processes above current view
  - `▼ More below` - Processes below current view

**Auto-Reset**: Scroll position resets to top when you change sorting or filtering.

### How to Kill a Process

1. Press **K**
2. Enter the PID (Process ID) of the process you want to kill
3. The program will send SIGTERM signal to that process
4. You'll see a success or error message
5. Press Enter to continue

**Note**: You need appropriate permissions to kill a process. You can only kill:
- Your own processes
- Root/system processes if you're running as root

### How to Search for Processes

1. Press **S**
2. Enter a search term (part of process name or command)
3. All matching processes will be displayed with their details
4. Press Enter to return to main view

**Tips**:
- Search is case-sensitive
- Searches in both process name and full command line
- Shows PID, User, Memory%, and full command for each match

### How to Use Filter

1. Press **F**
2. Enter a username (e.g., `root`, `www-data`, your username)
3. Press Enter
4. Only processes from that user will be displayed
5. The filter indicator will show at the bottom: `Filter: User: <username>`
6. Press **R** to reset and show all processes

### Display Information

The display is divided into three sections:

#### 1. System Information (Top)
- System uptime (days, hours, minutes, seconds)
- Memory usage with visual progress bar
- Total processes count

#### 2. Process Table (Middle)
Shows top 20 processes with columns:
- **PID**: Process ID
- **USER**: Process owner (truncated if > 10 chars)
- **CPU%**: CPU usage percentage
- **MEM%**: Memory usage percentage (color-coded)
- **VIRT**: Virtual memory size (human-readable)
- **RES**: Resident memory size (RAM used)
- **STATE**: Process state with colors:
  - 🟢 **RUN** (green) - Currently running
  - ⚪ **SLEEP** - Sleeping/waiting
  - 🟡 **DISK** (yellow) - Waiting for I/O
  - 🔴 **ZOMBI** (red) - Zombie process
  - 🔵 **STOP/IDLE** (cyan/blue) - Stopped or idle
- **COMMAND**: Process name/command (truncated to 45 chars)

#### 3. Command Menu (Bottom)
Interactive command reference showing:
- Available sort options with current selection highlighted
- Filter status
- Quick command reference

### Color Coding

Process rows are color-coded by memory usage:
- **🔴 Red**: High memory (> 5%)
- **🟡 Yellow**: Medium memory (2-5%)
- **⚪ White**: Low memory (< 2%)

### Auto-Refresh

- Display automatically refreshes every **2 seconds**
- Commands execute **immediately** when pressed
- No need to wait for refresh to see sorting/filtering changes

## Examples

### Example 1: Find Memory Hogs
```bash
# Launch AltTasker (memory sort is default)
task

# Top processes are the ones using most RAM
# Look for red-colored rows (>5% memory)
```

### Example 2: Monitor Your Processes
```bash
task
# Press F
# Enter your username
# Press M (to ensure memory sort)
# Your processes are now filtered and sorted by memory
```

### Example 3: Check CPU Usage
```bash
task
# Press C
# Top processes now show highest CPU usage
# Useful for finding CPU-intensive programs
```

### Example 4: User-Specific Monitoring
```bash
task
# Press F
# Enter 'root'
# Press U to sort by user
# See all root processes grouped together
```

### Example 5: Reset and Overview
```bash
# If you have a filter active:
# Press R
# Press M (memory sort)
# Get full system overview sorted by memory
```

## Tips

1. **Default is Memory Sort**: When you launch `task`, it starts with memory sorting (most useful)
2. **Quick Filter Reset**: Press R to instantly clear any filter
3. **Case Insensitive**: All commands work with lowercase or uppercase (P/p, C/c, etc.)
4. **Instant Response**: Commands execute immediately, no Enter key needed (except for filter input)
5. **Multiple Filters**: You can only filter by one user at a time
6. **Sorting Persists**: Your sort choice persists even when changing filters

## Troubleshooting

### Display Not Updating
- Make sure you're in a native WSL terminal (not PowerShell)
- Type `wsl` first if running from Windows
- Terminal must support ANSI escape codes

### Filter Shows No Processes
- Check username spelling (case-sensitive!)
- User might not have any running processes
- Press R to reset and verify processes exist

### Terminal Looks Broken After Exit
- This shouldn't happen (cleanup is automatic)
- If it does: type `reset` and press Enter
- Or close and reopen terminal

### Colors Not Showing
- Ensure TERM environment variable is set: `echo $TERM`
- Should be `xterm-256color` or similar
- Update with: `export TERM=xterm-256color`

---

**Need More Help?** See [README.md](README.md) for installation and setup information.
