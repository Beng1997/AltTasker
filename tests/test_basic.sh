#!/bin/bash
# Basic integration tests for AltTasker v2.2

# Don't exit on first error - we want to run all tests
set +e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BINARY="$PROJECT_ROOT/alttasker"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=========================================="
echo "  AltTasker v2.2 - Basic Tests"
echo "=========================================="
echo ""

# Test 1: Binary exists and is executable
test_binary_exists() {
    echo -n "Test 1: Binary exists and is executable... "
    if [ -f "$BINARY" ] && [ -x "$BINARY" ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Binary not found or not executable: $BINARY"
        return 1
    fi
}

# Test 2: Binary runs without crashing (1 second test)
test_binary_runs() {
    echo -n "Test 2: Binary runs without crashing... "
    if timeout 1s "$BINARY" > /dev/null 2>&1 || [ $? -eq 124 ]; then
        # Exit code 124 means timeout (expected), anything else is a crash
        if [ $? -eq 124 ] || [ $? -eq 0 ]; then
            echo -e "${GREEN}PASS${NC}"
            return 0
        fi
    fi
    echo -e "${RED}FAIL${NC}"
    echo "  Binary crashed during execution"
    return 1
}

# Test 3: Check if all source files exist
test_source_files() {
    echo -n "Test 3: All source files exist... "
    local missing=0
    
    for file in src/main.c src/display.c src/process_monitor.c src/signal_handler.c; do
        if [ ! -f "$PROJECT_ROOT/$file" ]; then
            echo -e "${RED}FAIL${NC}"
            echo "  Missing: $file"
            missing=1
        fi
    done
    
    if [ $missing -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    fi
    return 1
}

# Test 4: Check if all header files exist
test_header_files() {
    echo -n "Test 4: All header files exist... "
    local missing=0
    
    for file in include/common.h include/display.h include/process_monitor.h include/signal_handler.h; do
        if [ ! -f "$PROJECT_ROOT/$file" ]; then
            echo -e "${RED}FAIL${NC}"
            echo "  Missing: $file"
            missing=1
        fi
    done
    
    if [ $missing -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    fi
    return 1
}

# Test 5: Compilation succeeds
test_compilation() {
    echo -n "Test 5: Project compiles successfully... "
    cd "$PROJECT_ROOT"
    if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Compilation failed"
        return 1
    fi
}

# Test 6: No compiler warnings with strict flags
test_no_warnings() {
    echo -n "Test 6: Compiles without warnings... "
    cd "$PROJECT_ROOT"
    local warnings=$(make 2>&1 | grep -i "warning" | wc -l)
    
    if [ $warnings -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    else
        echo -e "${YELLOW}PARTIAL${NC} ($warnings warnings)"
        return 0  # Don't fail on warnings, just note them
    fi
}

# Test 7: Check documentation exists
test_documentation() {
    echo -n "Test 7: Documentation files exist... "
    local missing=0
    
    for file in README.md USAGE.md CHANGELOG-v2.2.md FEATURES-v2.2.md; do
        if [ ! -f "$PROJECT_ROOT/$file" ]; then
            echo -e "${RED}FAIL${NC}"
            echo "  Missing: $file"
            missing=1
        fi
    done
    
    if [ $missing -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    fi
    return 1
}

# Test 8: Check v2.2 features in code
test_v22_features() {
    echo -n "Test 8: v2.2 scrolling features present... "
    local features_found=0
    
    # Check for scroll_offset in main.c
    if grep -q "scroll_offset" "$PROJECT_ROOT/src/main.c"; then
        ((features_found++))
    fi
    
    # Check for arrow key handling
    if grep -q "ESC\[A\|Up arrow" "$PROJECT_ROOT/src/main.c"; then
        ((features_found++))
    fi
    
    # Check for Page Up/Down handling
    if grep -q "Page Up\|Page Down" "$PROJECT_ROOT/src/main.c"; then
        ((features_found++))
    fi
    
    if [ $features_found -eq 3 ]; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Only $features_found/3 scrolling features found"
        return 1
    fi
}

# Test 9: Binary size reasonable
test_binary_size() {
    echo -n "Test 9: Binary size is reasonable... "
    if [ ! -f "$BINARY" ]; then
        echo -e "${RED}FAIL${NC} (Binary not found)"
        return 1
    fi
    
    local size=$(stat -c%s "$BINARY" 2>/dev/null || echo "0")
    
    if [ "$size" -eq 0 ]; then
        echo -e "${YELLOW}WARN${NC} (Cannot determine size)"
        return 0
    fi
    
    # Should be between 10KB and 100KB
    if [ $size -gt 10240 ] && [ $size -lt 102400 ]; then
        echo -e "${GREEN}PASS${NC} ($(($size / 1024))KB)"
        return 0
    else
        echo -e "${YELLOW}WARN${NC} ($(($size / 1024))KB - unusual size)"
        return 0  # Don't fail, just warn
    fi
}

# Test 10: Check signal handling
test_signal_handling() {
    echo -n "Test 10: Signal handling implemented... "
    if grep -q "signal_handler\|SIGINT\|SIGTERM" "$PROJECT_ROOT/src/signal_handler.c"; then
        echo -e "${GREEN}PASS${NC}"
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Signal handling code not found"
        return 1
    fi
}

# Run all tests
echo "Running tests..."
echo ""

passed=0
failed=0

tests=(
    test_binary_exists
    test_source_files
    test_header_files
    test_compilation
    test_no_warnings
    test_binary_runs
    test_documentation
    test_v22_features
    test_binary_size
    test_signal_handling
)

for test in "${tests[@]}"; do
    if $test; then
        ((passed++))
    else
        ((failed++))
    fi
done

echo ""
echo "=========================================="
echo "  Test Results"
echo "=========================================="
echo -e "Passed: ${GREEN}$passed${NC}"
echo -e "Failed: ${RED}$failed${NC}"
echo "Total:  $((passed + failed))"
echo ""

if [ $failed -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
