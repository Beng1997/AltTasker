# 🎉 AltTasker v2.1 - Kill & Search Update

## Latest Changes (v2.1)

### New Features

#### 🔴 Kill Process (K)
- Interactive process termination
- Enter PID to kill a process with SIGTERM signal
- Permission checking with error messages
- Confirmation prompt before returning to main view

#### 🔍 Search Processes (S)
- Real-time process search by name or command
- Case-sensitive matching in process name and command line
- Shows matching processes with full details (PID, User, Memory%, Command)
- Displays count of found processes

#### 🎨 Enhanced Color Scheme
- **Yellow border** on system info header for better visibility
- **Color-coded command menu**:
  - Yellow: Section headers (Sort, Filter, Actions)
  - Red: Dangerous actions (Kill)
  - Cyan: Info actions (Search)
  - Green: Active selections and current sort mode
- **Memory bar colors** based on usage:
  - Green: < 50% (healthy)
  - Yellow: 50-75% (warning)
  - Red: > 75% (critical)
- **Icons** added to system info (⏱️ Uptime, 💾 Memory, 📊 Processes)

### Technical Details

#### Kill Process Implementation
```c
case 'k':
case 'K':
    // Prompt for PID
    // Send SIGTERM signal with kill()
    // Display success/error message
    // Wait for Enter to continue
```

#### Search Implementation
```c
case 's':
case 'S':
    // Prompt for search term
    // Iterate through all processes
    // Match against name and cmdline using strstr()
    // Display all matches with formatting
```

### UI Improvements

Before (v2.0):
```
Sort: PID  CPU  Memory  User   Current: MEM↓
Filter: F User  R Reset
Actions: K Kill process  S Search  Q/Ctrl+C Quit
```

After (v2.1):
```
Sort: PID  CPU  Memory  User     Current: MEM↓  (Green highlight on active)
Filter: F User  R Reset               (Yellow headers)
Actions: K Kill  S Search  Q/Ctrl+C Quit  (Red K, Cyan S)
```

### Command Summary

| Command | Color | Function | Status |
|---------|-------|----------|--------|
| P | Bold/Green | Sort by PID | ✅ Working |
| C | Bold/Green | Sort by CPU | ✅ Working |
| M | Bold/Green | Sort by Memory | ✅ Working (default) |
| U | Bold/Green | Sort by User | ✅ Working |
| F | Bold | Filter by user | ✅ Working |
| R | Bold | Reset filter | ✅ Working |
| **K** | **Red** | **Kill process** | ✅ **NEW** |
| **S** | **Cyan** | **Search** | ✅ **NEW** |
| Q | Bold | Quit | ✅ Working |

---

# 🎉 AltTasker v2.0 - Interactive CLI Update (Previous)

## What's New?

AltTasker now includes a **full interactive command-line interface** for real-time process monitoring and management!

### New Features

#### 🔄 Sorting Options
- **P** - Sort by Process ID (ascending)
- **C** - Sort by CPU usage (descending)
- **M** - Sort by Memory usage (descending) - **Default**
- **U** - Sort by Username (alphabetical)

#### 🔍 Filtering
- **F** - Filter processes by username (interactive prompt)
- **R** - Reset filter to show all processes

#### 🎯 Interactive Menu
- Visual command menu always visible at bottom of screen
- Shows current sort mode with indicator (e.g., "Current: MEM↓")
- Displays active filter status

#### ⚡ Real-Time Response
- Commands execute **immediately** when pressed
- Non-blocking keyboard input
- No need to wait for next refresh cycle

#### 🎨 Improved Display
- Reduced to top 20 processes for better readability with command menu
- Enhanced color coding for process states
- Bordered command menu for clarity

### Technical Improvements

#### Non-Blocking Input
```c
// Uses select() for non-blocking keyboard input
char get_keypress() {
    fd_set readfds;
    struct timeval tv;
    tv.tv_usec = 50000;  // 50ms timeout
    select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
    // ...
}
```

#### Comparison Functions
```c
// Sort processes with qsort()
static int compare_by_cpu(const void* a, const void* b);
static int compare_by_mem(const void* a, const void* b);
static int compare_by_pid(const void* a, const void* b);
static int compare_by_user(const void* a, const void* b);
```

#### Filter Implementation
```c
// Filter processes by username
int filter_processes_by_user(const ProcessInfo processes[], int count, 
                              ProcessInfo filtered[], const char* username);
```

### Code Changes

#### Modified Files
- `include/common.h` - Added `SortMode` enum
- `include/display.h` - Added `display_command_menu()` function
- `include/process_monitor.h` - Added sort and filter functions
- `src/main.c` - Complete rewrite with interactive loop
- `src/display.c` - Added command menu display, reduced to 20 processes
- `src/process_monitor.c` - Added sorting and filtering functions
- `src/signal_handler.c` - Updated for new variable names

#### New Files
- `docs/INTERACTIVE-COMMANDS.md` - Detailed command reference
- `USAGE.md` - User guide with examples
- `scripts/create-main.sh` - Helper script for main.c generation
- `scripts/create-signal.sh` - Helper script for signal_handler.c generation
- `scripts/update-main.sh` - Helper script for main.c updates
- `scripts/create-usage.sh` - Helper script for usage doc generation

### Usage Example

```bash
# Start AltTasker
task

# The interface shows:
# - System info at top
# - Top 20 processes in middle (sorted by memory by default)
# - Interactive command menu at bottom

# Try these commands:
C     # Sort by CPU usage
U     # Sort by user
F     # Filter by user (prompts for username)
root  # Enter username to filter
R     # Reset filter
Q     # Quit
```

### Performance Considerations

- **Refresh Rate**: Still 2 seconds (unchanged)
- **CPU Impact**: Minimal - non-blocking input with 50ms timeout
- **Memory Usage**: Slightly increased due to filtered process array
- **Responsiveness**: Instant command execution

### Breaking Changes

**None!** The program remains backward compatible. If you don't use any commands, it works exactly like before.

### Migration Guide

No migration needed! If you have the old version:
```bash
cd /mnt/d/AltTasker
git pull
make clean && make
```

### Future Enhancements

Planned features shown in command menu:
- **K** - Kill process (interactive)
- **S** - Search for process by name
- CPU usage over time (multi-sample average)
- Tree view for parent-child processes

### Documentation

New documentation files:
- `USAGE.md` - Comprehensive usage guide with examples
- `docs/INTERACTIVE-COMMANDS.md` - Detailed command reference
- Updated `README.md` - Quick start and feature list

### Building

```bash
# Clean build
make clean && make

# The build is successful with only minor warnings:
# - usleep() implicit declaration (POSIX function)
# - fscanf format warnings (minor parsing issues)
```

### Testing

To test the new features:
1. Open WSL terminal: `wsl`
2. Run: `task`
3. Try sorting: Press `C`, `M`, `U`, `P`
4. Try filtering: Press `F`, enter a username
5. Reset filter: Press `R`
6. Exit: Press `Q` or `Ctrl+C`

---

## Commit Message

```
feat: Add interactive CLI with sorting and filtering

- Added sort modes: PID, CPU, Memory, User
- Added user filter functionality
- Implemented non-blocking keyboard input
- Display command menu at bottom
- Support P/C/M/U for sorting
- Support F for filter, R for reset
- Support Q for quit
- Reduced display to 20 processes for better visibility
- Interactive mini-CLI for better usability

BREAKING CHANGE: None (backward compatible)
```

---

**Happy Process Monitoring!** 🚀
