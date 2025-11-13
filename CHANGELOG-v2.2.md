# Changelog - Version 2.2

## [2.2.0] - November 13, 2025

### 🎯 Major Feature: Process List Scrolling

Added comprehensive scrolling capability to navigate through large process lists (50+ processes).

### ✨ New Features

#### Navigation Controls
- **Arrow Keys (↑/↓)**: Line-by-line scrolling through process list
- **Page Up/Page Down**: Fast navigation (jumps 20 lines at a time)
- **Home Key**: Instantly jump to the first process
- **End Key**: Instantly jump to the last process

#### Visual Enhancements
- **Scroll Position Indicator**: Shows "Showing 1-20 of 150 processes"
- **Direction Arrows**: Visual cues (▲/▼) indicate when more content is above/below
- **Navigation Menu**: Appears automatically when >20 processes detected

#### Smart Behavior
- **Auto-Reset Scrolling**: Scroll position resets to top when:
  - Sorting mode changes (P/C/M/U)
  - Filter is applied or reset (F/R)
- **Bounds Protection**: Prevents scrolling beyond list limits
- **Dynamic Adjustment**: Scroll range adjusts when filter reduces process count

### 🔧 Technical Implementation

#### Modified Functions
- `display_processes()`: Added `scroll_offset` parameter for window positioning
- `display_command_menu()`: Added `scroll_offset` and `total_processes` parameters
- `get_keypress()`: Enhanced to parse ANSI escape sequences for special keys

#### Key Mappings (Internal)
```c
ESC[A  → 'w' (Up arrow)
ESC[B  → 'x' (Down arrow)  
ESC[5~ → 'W' (Page Up)
ESC[6~ → 'X' (Page Down)
ESC[H  → 'h' (Home)
ESC[F  → 'e' (End)
```

#### State Management
- New variable: `scroll_offset` tracks current viewing position
- Scroll validation on every refresh cycle
- Coordinated with `display_count` after filtering/sorting

### 📊 Performance Notes
- Zero performance impact - scrolling is UI-only
- No additional memory allocation (displays same 20 processes)
- Escape sequence parsing adds negligible latency (<1ms)

### 🐛 Bug Fixes
- Fixed unused parameter warnings in display functions
- Added proper `read()` return value handling for escape sequences
- Added `_DEFAULT_SOURCE` define for `usleep()` compatibility

### 📝 Documentation Updates
- Updated `USAGE.md` with navigation section
- Added keyboard command table with arrow keys
- Included scroll indicator explanations

### 🎨 UI Improvements
- Navigation commands only shown when applicable (>20 processes)
- Consistent color scheme for navigation hints
- Clear visual feedback for scroll boundaries

---

## Version Comparison

| Feature | v2.1 | v2.2 |
|---------|------|------|
| Kill Process (K) | ✅ | ✅ |
| Search (S) | ✅ | ✅ |
| Enhanced Colors | ✅ | ✅ |
| Arrow Key Navigation | ❌ | ✅ |
| Page Up/Down | ❌ | ✅ |
| Home/End Keys | ❌ | ✅ |
| Scroll Indicators | ❌ | ✅ |
| Position Display | ❌ | ✅ |

---

## Upgrade Notes

### From v2.1 to v2.2
- **Backward Compatible**: All v2.1 features work identically
- **New Keybindings**: Arrow keys now functional (previously did nothing)
- **No Configuration Needed**: Scrolling is automatic when >20 processes exist

### User Experience Changes
- With **≤20 processes**: Interface identical to v2.1
- With **>20 processes**: Navigation menu appears, arrow keys active

---

## Usage Examples

### Example 1: Scrolling Through 100 Processes
```bash
$ task
# Shows processes 1-20
# Press Page Down → Shows 21-40
# Press Page Down → Shows 41-60
# Press End → Shows 81-100
# Press Home → Back to 1-20
```

### Example 2: Precise Navigation
```bash
# Find a specific process in a long list
1. Press M to sort by memory
2. Press ↓ three times to see next 3 processes
3. Found it at position #23
4. Press K, enter PID to kill
```

### Example 3: Filter + Scroll
```bash
# User 'www-data' has 50 processes
1. Press F, type "www-data"
2. Shows 50 filtered processes
3. Press Page Down to see processes 21-40
4. Navigation works on filtered list
```

---

## Technical Details

### Escape Sequence Handling
```c
// Before v2.2: Arrow keys ignored
char c = getchar();

// After v2.2: Full escape sequence parsing
if (c == 27) {  // ESC
    char seq[2];
    read(STDIN_FILENO, &seq[0], 1);
    if (seq[0] == '[') {
        read(STDIN_FILENO, &seq[1], 1);
        if (seq[1] == 'A') return 'w';  // Up
        if (seq[1] == 'B') return 'x';  // Down
        // ... etc
    }
}
```

### Scroll Offset Management
```c
// Calculate visible window
int start_index = scroll_offset;
int end_index = min(start_index + 20, total_count);

// Display only visible processes
for (int i = start_index; i < end_index; i++) {
    display_process(processes[i]);
}
```

---

## Known Limitations

1. **Terminal Compatibility**: Requires ANSI-compatible terminal (Linux, WSL, most modern terminals)
2. **Fixed Window Size**: Always shows 20 processes per page (not configurable yet)
3. **No Mouse Support**: Navigation is keyboard-only

---

## Future Enhancements (Roadmap)

- [ ] Configurable page size (allow 10, 20, 30, etc.)
- [ ] Mouse wheel scrolling support
- [ ] Smooth scrolling animation
- [ ] Scroll position persistence across refreshes
- [ ] Vim-style navigation (j/k keys)

---

**Contributors**: Beng1997  
**Build**: v2.2.0 (November 2025)  
**Lines of Code**: 1,200+ (added ~90 lines for scrolling)  
**Compilation**: Clean, zero warnings ✅
