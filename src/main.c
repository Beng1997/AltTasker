#include "common.h"
#include "process_monitor.h"
#include "display.h"
#include "signal_handler.h"
#include <termios.h> // For terminal settings

// ============================================================================
// Global Variables
// ============================================================================

// This flag controls the main loop. It is modified by the signal handler.
// 'volatile sig_atomic_t' is required for safe signal handling.
volatile sig_atomic_t running = 1;

// This struct will store the original terminal settings to restore them on exit.
struct termios old_tio;

// ============================================================================
// Forward Declarations for local functions
// ============================================================================
void setup_terminal(void);

// ============================================================================
// Main Function
// ============================================================================
int main() {
    // 1. Initial Setup
    // ----------------
    // Register signal handlers for graceful shutdown (Ctrl+C)
    setup_signal_handlers();
    
    // Set up the terminal for TUI mode (non-canonical, no echo, hide cursor)
    setup_terminal();

    // Data structures to hold information
    ProcessInfo processes[MAX_PROCESS];
    sysinfo_t sysinfo;

    // 2. Main Refresh Loop
    // --------------------
    // The loop continues as long as the 'running' flag is 1.
    // The signal handler will set it to 0 to terminate the loop.
    while (running) {
        // Clear entire screen and move to top
        printf("\x1b[2J");  // Clear entire screen
        printf("\x1b[H");   // Move cursor to home (1,1)

        // Get current system and process information
        get_system_info(&sysinfo);
        int process_count = scan_processes(processes, MAX_PROCESS, sysinfo.total_mem);

        // Display the information
        display_system_info(&sysinfo);
        if (process_count > 0) {
            display_processes(processes, process_count);
        }

        // Flush output to ensure everything is printed
        fflush(stdout);

        // Wait for a short period before refreshing (2 seconds for better readability)
        sleep(2);
    }

    // 3. Graceful Exit
    // ----------------
    // The cleanup function is called to restore the terminal and clean up resources.
    cleanup();
    
    return 0;
}

// ============================================================================
// Local Helper Functions
// ============================================================================

/**
 * @brief Configures the terminal for TUI mode.
 * 
 * This function saves the original terminal settings and then configures the
 * terminal to be in non-canonical mode with echo disabled. It also hides the cursor.
 * This allows for reading single key presses without waiting for Enter and
 * prevents user input from being displayed on the screen.
 */
void setup_terminal(void) {
    // Save the original terminal settings
    tcgetattr(STDIN_FILENO, &old_tio);
    
    // Create a new terminal settings struct based on the original
    struct termios new_tio = old_tio;
    
    // Disable canonical mode (line buffering) and echo
    new_tio.c_lflag &= ~(ICANON | ECHO);
    
    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    
    // Enter alternate screen buffer (like htop does)
    printf("\x1b[?1049h");
    
    // Hide the cursor
    printf("\x1b[?25l");
    
    // Clear screen
    printf("\x1b[2J\x1b[H");
    
    fflush(stdout);
}
