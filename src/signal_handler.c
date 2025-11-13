#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "../include/signal_handler.h"
#include "../include/common.h"

volatile sig_atomic_t keep_running = 1;

extern struct termios orig_termios;

void handle_termination_signal(int signo) {
    (void)signo;
    keep_running = 0;
}

void setup_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = handle_termination_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void cleanup(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    
    printf("\x1b[?25h");
    printf("\x1b[?1049l");
    printf("\x1b[2J\x1b[H");
    
    printf(COLOR_CYAN "\n👋 Goodbye! Thanks for using AltTasker!\n\n" COLOR_RESET);
    
    fflush(stdout);
}
