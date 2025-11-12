# ============================================================================
# AltTasker Makefile
# ============================================================================
# Description: Build automation for AltTasker - Linux task manager
# Author: AltTasker Team
# License: MIT
# ============================================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O2 -std=c11 
INCLUDES = -I./include 
LDFLAGS = 

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
SCRIPTS_DIR = scripts
TESTS_DIR = tests

# Target executable
TARGET = alttasker

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Colors for output (optional, makes output prettier)
COLOR_RESET = \033[0m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_CYAN = \033[36m
COLOR_RED = \033[31m

# ============================================================================
# Default target: build the project
# ============================================================================
.PHONY: all
all: $(TARGET)
	@echo "$(COLOR_GREEN)✅ Build successful: ./$(TARGET)$(COLOR_RESET)"

# ============================================================================
# Link object files into final executable
# ============================================================================
$(TARGET): $(OBJECTS)
	@echo "$(COLOR_CYAN)🔗 Linking $(TARGET)...$(COLOR_RESET)"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# ============================================================================
# Compile source files to object files
# ============================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "$(COLOR_YELLOW)⚙️  Compiling $<...$(COLOR_RESET)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================================
# Clean build artifacts
# ============================================================================
.PHONY: clean
clean:
	@echo "$(COLOR_RED)🧹 Cleaning build artifacts...$(COLOR_RESET)"
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "$(COLOR_GREEN)✅ Clean complete$(COLOR_RESET)"

# ============================================================================
# Install the program (requires root/sudo)
# ============================================================================
.PHONY: install
install: $(TARGET)
	@echo "$(COLOR_CYAN)📦 Installing $(TARGET)...$(COLOR_RESET)"
	@if [ -f $(SCRIPTS_DIR)/install.sh ]; then \
		chmod +x $(SCRIPTS_DIR)/install.sh; \
		sudo $(SCRIPTS_DIR)/install.sh; \
	else \
		echo "$(COLOR_YELLOW)⚠️  install.sh not found, performing basic install...$(COLOR_RESET)"; \
		sudo install -m 755 $(TARGET) $(BINDIR)/$(TARGET); \
		echo "$(COLOR_GREEN)✅ Installed to $(BINDIR)/$(TARGET)$(COLOR_RESET)"; \
	fi

# ============================================================================
# Uninstall the program (requires root/sudo)
# ============================================================================
.PHONY: uninstall
uninstall:
	@echo "$(COLOR_RED)🗑️  Uninstalling $(TARGET)...$(COLOR_RESET)"
	@if [ -f $(SCRIPTS_DIR)/uninstall.sh ]; then \
		chmod +x $(SCRIPTS_DIR)/uninstall.sh; \
		sudo $(SCRIPTS_DIR)/uninstall.sh; \
	else \
		echo "$(COLOR_YELLOW)⚠️  uninstall.sh not found, performing basic uninstall...$(COLOR_RESET)"; \
		sudo rm -f $(BINDIR)/$(TARGET); \
		echo "$(COLOR_GREEN)✅ Uninstalled from $(BINDIR)/$(TARGET)$(COLOR_RESET)"; \
	fi

# ============================================================================
# Run tests
# ============================================================================
.PHONY: test
test: $(TARGET)
	@echo "$(COLOR_CYAN)🧪 Running tests...$(COLOR_RESET)"
	@if [ -f $(TESTS_DIR)/test_basic.sh ]; then \
		chmod +x $(TESTS_DIR)/test_basic.sh; \
		$(TESTS_DIR)/test_basic.sh; \
	else \
		echo "$(COLOR_YELLOW)⚠️  No tests found$(COLOR_RESET)"; \
	fi

# ============================================================================
# Rebuild: clean and build
# ============================================================================
.PHONY: rebuild
rebuild: clean all

# ============================================================================
# Run the program (for development/testing)
# ============================================================================
.PHONY: run
run: $(TARGET)
	@echo "$(COLOR_CYAN)🚀 Running $(TARGET)...$(COLOR_RESET)"
	./$(TARGET)

# ============================================================================
# Show help
# ============================================================================
.PHONY: help
help:
	@echo "$(COLOR_CYAN)════════════════════════════════════════════════════════════$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)  AltTasker Makefile - Available Commands$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)════════════════════════════════════════════════════════════$(COLOR_RESET)"
	@echo ""
	@echo "  $(COLOR_YELLOW)make$(COLOR_RESET) or $(COLOR_YELLOW)make all$(COLOR_RESET)      - Build the project"
	@echo "  $(COLOR_YELLOW)make clean$(COLOR_RESET)            - Remove build artifacts"
	@echo "  $(COLOR_YELLOW)make rebuild$(COLOR_RESET)          - Clean and rebuild"
	@echo "  $(COLOR_YELLOW)make install$(COLOR_RESET)          - Install to system (requires sudo)"
	@echo "  $(COLOR_YELLOW)make uninstall$(COLOR_RESET)        - Remove from system (requires sudo)"
	@echo "  $(COLOR_YELLOW)make test$(COLOR_RESET)             - Run test suite"
	@echo "  $(COLOR_YELLOW)make run$(COLOR_RESET)              - Build and run the program"
	@echo "  $(COLOR_YELLOW)make help$(COLOR_RESET)             - Show this help message"
	@echo ""
	@echo "$(COLOR_CYAN)════════════════════════════════════════════════════════════$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)Example usage:$(COLOR_RESET)"
	@echo "  make                    # Build the project"
	@echo "  sudo make install       # Install system-wide"
	@echo "  make test               # Run tests"
	@echo "  sudo make uninstall     # Remove installation"
	@echo "$(COLOR_CYAN)════════════════════════════════════════════════════════════$(COLOR_RESET)"

# ============================================================================
# Debug: show Makefile variables
# ============================================================================
.PHONY: debug
debug:
	@echo "$(COLOR_CYAN)🐛 Debug Information:$(COLOR_RESET)"
	@echo "CC        = $(CC)"
	@echo "CFLAGS    = $(CFLAGS)"
	@echo "INCLUDES  = $(INCLUDES)"
	@echo "SOURCES   = $(SOURCES)"
	@echo "OBJECTS   = $(OBJECTS)"
	@echo "TARGET    = $(TARGET)"
	@echo "BINDIR    = $(BINDIR)"
