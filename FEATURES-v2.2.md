# 🎯 AltTasker v2.2 - Process List Scrolling

## ✨ What's New

### Navigation Through Large Process Lists

When monitoring systems with 50+ processes, you can now:

- **Scroll line by line** with arrow keys (↑/↓)
- **Jump pages** with Page Up/Page Down (20 lines at a time)
- **Go to extremes** with Home (top) and End (bottom)
- **See your position** with status: "Showing 1-20 of 150 processes"
- **Get visual cues** with indicators: ▲ More above / ▼ More below

## 🎮 Navigation Keys

| Key | Action | Example |
|-----|--------|---------|
| `↑` | Scroll up 1 line | From line 10 → line 9 |
| `↓` | Scroll down 1 line | From line 10 → line 11 |
| `Page Up` | Jump up 20 lines | From lines 41-60 → 21-40 |
| `Page Down` | Jump down 20 lines | From lines 21-40 → 41-60 |
| `Home` | Go to first process | Jump to lines 1-20 |
| `End` | Go to last process | Jump to last 20 processes |

## 🔧 Smart Behavior

### Auto-Reset
Scroll position automatically resets to top when you:
- Change sort mode (P/C/M/U)
- Apply a filter (F)
- Reset filter (R)

This ensures you always see the "top" results of your new view.

### Bounds Protection
- Can't scroll above first process
- Can't scroll below last process
- Scroll range adjusts when filter reduces process count

## 📊 Example Usage

### Scenario 1: Finding High-Memory Process in 100 Processes

```bash
$ task
# Shows processes 1-20, sorted by memory

# Want to see #50?
Press Page Down    # Now showing 21-40
Press Page Down    # Now showing 41-60
Press ↓ ten times  # Now showing processes around #50

# Found the culprit!
Press K, enter PID to kill it
```

### Scenario 2: Checking All User Processes

```bash
$ task
Press F, type "www-data"  # Filter shows 75 processes

# Status: "Showing 1-20 of 75 processes"
Press Page Down   # See 21-40
Press Page Down   # See 41-60
Press Page Down   # See 61-75

# Want to go back?
Press Home        # Back to 1-20
```

### Scenario 3: Quick Bottom Check

```bash
$ task
# Shows top 20 by memory

# Want to see the lowest memory processes?
Press End         # Jump to last 20 immediately

# Back to top?
Press Home        # Back to first 20
```

## 🎨 Visual Indicators

### When Scrolling is Available (>20 processes)

The command menu shows:
```
║ Navigate: ↑/↓ Line  PgUp/PgDn Page  Home/End Top/Bottom    ║
```

### Position Display

At the bottom of the process list:
```
Showing 21-40 of 150 processes ▲ More above ▼ More below
```

### When All Processes Fit (≤20 processes)

Navigation line doesn't appear:
```
Showing all 15 processes
```

## 🔍 Technical Implementation

### Modified Functions

**`display_processes(processes[], count, scroll_offset)`**
- Added `scroll_offset` parameter
- Calculates visible window: `[offset, offset+20]`
- Shows position and direction indicators

**`display_command_menu(sort, filter, offset, total)`**
- Added `scroll_offset` and `total_processes` parameters
- Shows navigation line only when needed (>20 processes)

**`get_keypress()`**
- Enhanced to parse ANSI escape sequences
- Maps ESC sequences to internal codes:
  - `ESC[A` → Up arrow
  - `ESC[B` → Down arrow
  - `ESC[5~` → Page Up
  - `ESC[6~` → Page Down
  - `ESC[H` → Home
  - `ESC[F` → End

### State Management

New variables in `main()`:
```c
int scroll_offset = 0;      // Current scroll position
int display_count = 0;      // Total processes after filtering
```

Scroll validation on every refresh:
```c
if (scroll_offset > display_count - 20) {
    scroll_offset = display_count - 20;
}
if (scroll_offset < 0) scroll_offset = 0;
```

## 📈 Performance

- **Memory**: No additional allocation (still shows 20 processes)
- **CPU**: Negligible overhead for escape sequence parsing (<1ms)
- **Binary Size**: 31KB (unchanged from v2.1)
- **Latency**: No perceptible delay in key response

## 🐛 Known Limitations

1. **Terminal Dependency**: Requires ANSI escape sequence support
   - ✅ Linux, WSL, macOS terminals
   - ✅ Modern Windows Terminal, iTerm2, Alacritty
   - ❌ Very old terminals (pre-1990s)

2. **Fixed Page Size**: Always 20 lines per page
   - Not configurable (yet)
   - Future enhancement planned

3. **No Mouse Support**: Keyboard navigation only
   - Scroll wheel not supported (yet)
   - Future enhancement planned

## 🚀 Upgrade from v2.1

### Backward Compatible
All v2.1 features work identically:
- ✅ Kill process (K)
- ✅ Search (S)
- ✅ Sorting (P/C/M/U)
- ✅ Filtering (F/R)
- ✅ Enhanced colors

### What's Added
- ✅ Arrow key navigation
- ✅ Page Up/Down support
- ✅ Home/End keys
- ✅ Scroll indicators
- ✅ Position display

### No Breaking Changes
- Same command line usage: `task`
- Same configuration (none required)
- Same keybindings for existing features

## 📝 Files Changed

| File | Changes | Lines Added |
|------|---------|-------------|
| `src/main.c` | Scroll state, key handling | +65 |
| `src/display.c` | Scroll offset support | +15 |
| `include/display.h` | Updated signatures | +5 |
| `USAGE.md` | Navigation docs | +35 |
| `CHANGELOG-v2.2.md` | Version notes | +250 |

**Total**: ~370 lines of code and documentation

## ✅ Testing Checklist

- [x] Arrow keys scroll one line at a time
- [x] Page Up/Down jump 20 lines
- [x] Home goes to first process
- [x] End goes to last process
- [x] Indicators show correctly (▲/▼)
- [x] Position display accurate
- [x] Scroll resets on sort change
- [x] Scroll resets on filter change
- [x] Bounds checking works (no over-scroll)
- [x] Works with filtered lists
- [x] Clean compilation (zero warnings)

## 🎯 Try It Now!

```bash
# In WSL or Linux terminal
$ task

# If you have >20 processes:
1. Try pressing ↓ several times
2. Press Page Down
3. Press End to see the last processes
4. Press Home to return to top
5. Sort by user (U) - notice scroll resets
6. Filter by user (F) - scroll resets again
```

## 🔮 Future Enhancements

Planned for v2.3+:

- [ ] Configurable page size (10, 20, 30, etc.)
- [ ] Mouse wheel scrolling
- [ ] Smooth scroll animation
- [ ] Vim-style navigation (j/k/gg/G)
- [ ] Scroll position indicator bar on right edge
- [ ] Jump to specific line number
- [ ] Persistent scroll across refreshes

---

**Version**: 2.2.0  
**Release Date**: November 13, 2025  
**Build Status**: ✅ Clean compilation, zero warnings  
**Binary Size**: 31KB  
**Platform**: Linux, WSL, Unix-like systems
