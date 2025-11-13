# AltTasker Testing Suite

This directory contains test scripts for AltTasker.

## Test Files

### Shell Scripts (Tracked in Git)
- `test_basic.sh` - Basic integration tests (binary existence, compilation, etc.)

### Python Scripts (Local Only - Not Tracked)
- `test_features.py` - Automated feature testing for all interactive commands
- `test_performance.py` - Performance benchmarking (CPU, memory, response times)

## Running Tests

### In WSL/Linux:

```bash
# Run basic shell tests
./test_basic.sh

# Run Python feature tests
python3 test_features.py

# Run performance tests
python3 test_performance.py
```

### From Windows PowerShell:

```powershell
# Run basic tests
wsl -- bash -c "cd /mnt/d/AltTasker/tests && ./test_basic.sh"

# Run feature tests
wsl -- python3 /mnt/d/AltTasker/tests/test_features.py

# Run performance tests
wsl -- python3 /mnt/d/AltTasker/tests/test_performance.py
```

## Python Test Requirements

The Python tests have the following dependencies:

### Required for Performance Tests:
```bash
# Install psutil for performance monitoring
pip3 install psutil

# Or install all requirements at once
pip3 install -r tests/requirements.txt
```

### Optional Dependencies:
The feature tests (`test_features.py`) work with standard library only - no dependencies needed!

**Note:** If `psutil` is not installed, performance tests will run with limited functionality (startup and scrolling tests only).

## Test Coverage

### test_features.py Tests:
1. ✅ Binary exists and is executable
2. ✅ Binary starts and exits cleanly
3. ✅ SIGINT/SIGTERM handling (Ctrl+C)
4. ✅ Sort commands (P/C/M/U)
5. ✅ Scrolling (Arrow keys, Page Up/Down, Home/End)
6. ✅ Filter by user (F) and reset (R)
7. ✅ Search command (S)
8. ✅ Kill command with cancel (K)
9. ✅ Multiple operations in sequence
10. ✅ Rapid key presses (stress test)

### test_performance.py Tests:
1. ⚡ Memory usage footprint
2. ⚡ CPU usage during operation
3. ⚡ Startup time
4. ⚡ Scrolling performance
5. ⚡ Refresh cycle overhead

### test_basic.sh Tests:
1. 🔧 Binary compilation
2. 🔧 Source files present
3. 🔧 Header files present
4. 🔧 Documentation complete
5. 🔧 v2.2 features in code
6. 🔧 Signal handling implemented

## Notes

- Python test scripts are **NOT tracked in git** (listed in `.gitignore`)
- Tests are designed to be non-destructive (no actual processes killed)
- Performance benchmarks should be run on a idle system for accurate results
- All tests can be run in parallel if needed
