#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "common.h"
#include <signal.h>

/**
 * @brief Sets up signal handlers for the application.
 * 
 * This function registers handlers for SIGINT and SIGTERM to ensure graceful shutdown.
 * It should be called once at the beginning of main().
 */
void setup_signal_handlers(void);

/**
 * @brief Handler function for termination signals (SIGINT, SIGTERM).
 * 
 * This function is signal-safe. It sets a global flag to terminate the main loop,
 * allowing the application to clean up and exit gracefully.
 * 
 * @param signo The signal number that was received.
 */
void handle_termination_signal(int signo);

/**
 * @brief Cleanup function to restore terminal settings and release resources.
 * 
 * This function should be called before the application exits to:
 * - Restore original terminal settings (canonical mode, echo).
 * - Make the cursor visible again.
 * - Clear the screen.
 * - Free any allocated resources.
 */
void cleanup(void);

#endif // SIGNAL_HANDLER_H