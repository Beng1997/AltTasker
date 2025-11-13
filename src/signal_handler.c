#define _POSIX_C_SOURCE 200809L

#include "signal_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>   // For signal handling
#include <termios.h>  // For terminal settings
#include <unistd.h>   // For STDIN_FILENO

// This global variable will be defined in main.c and used to control the main loop.
// 'volatile sig_atomic_t' is the only type that is safe to modify in a signal handler.
extern volatile sig_atomic_t running;

// This global variable will hold the original terminal settings.
// It will be initialized in main.c and restored in cleanup().
extern struct termios old_tio;

void setup_signal_handlers(void) {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Set the handler for termination signals
    sa.sa_handler = handle_termination_signal;
    
    // Register the handler for SIGINT (Ctrl+C) and SIGTERM (system shutdown)
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void handle_termination_signal(int signo) {
    // This is the only safe action to perform inside a signal handler.
    // We set a flag, and the main loop will detect it and exit gracefully.
    // Avoid calling functions like printf() or exit() from here.
    running = 0;
}

void cleanup(void) {
    // 1. Clear screen one last time
    printf("\x1b[2J\x1b[H");
    
    // 2. Make the cursor visible again
    printf("\x1b[?25h");
    
    // 3. Exit alternate screen buffer (return to normal terminal)
    printf("\x1b[?1049l");
    
    // 4. Restore original terminal settings (very important!)
    // This brings back canonical mode and character echoing.
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    
    // 5. Print goodbye message
    printf("\n");
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    printf(COLOR_BOLD "  AltTasker terminated successfully\n" COLOR_RESET);
    printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
    printf(COLOR_GREEN "  Thank you for using AltTasker! 👋\n" COLOR_RESET);
    printf("\n");
}
    
