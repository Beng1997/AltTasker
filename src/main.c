#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>
#include <sys/types.h>
#include "../include/common.h"
#include "../include/process_monitor.h"
#include "../include/display.h"
#include "../include/signal_handler.h"

extern volatile sig_atomic_t keep_running;

struct termios orig_termios;  // Defined here, used in main and signal_handler

void setup_terminal() {
    printf("\x1b[?1049h");
    printf("\x1b[?25l");
    fflush(stdout);
    
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    printf("\x1b[?25h");
    printf("\x1b[?1049l");
    fflush(stdout);
}

char get_keypress() {
    char c = 0;
    fd_set readfds;
    struct timeval tv;
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    tv.tv_sec = 0;
    tv.tv_usec = 50000;
    
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0) {
        ssize_t n = read(STDIN_FILENO, &c, 1);
        (void)n;  // Suppress unused warning
        
        // Handle escape sequences for arrow keys
        if (c == 27) {  // ESC
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1) {
                if (seq[0] == '[') {
                    if (read(STDIN_FILENO, &seq[1], 1) == 1) {
                        if (seq[1] == 'A') return 'w';      // Up arrow -> 'w'
                        if (seq[1] == 'B') return 'x';      // Down arrow -> 'x'
                        if (seq[1] == '5') {                // Page Up
                            char tmp;
                            ssize_t n = read(STDIN_FILENO, &tmp, 1); // consume '~'
                            (void)n;
                            return 'W';
                        }
                        if (seq[1] == '6') {                // Page Down
                            char tmp;
                            ssize_t n = read(STDIN_FILENO, &tmp, 1); // consume '~'
                            (void)n;
                            return 'X';
                        }
                        if (seq[1] == 'H') return 'h';      // Home
                        if (seq[1] == 'F') return 'e';      // End
                    }
                }
            }
        }
    }
    
    return c;
}

int main() {
    setup_signal_handler();
    setup_terminal();
    
    ProcessInfo processes[MAX_PROCESS];
    ProcessInfo filtered_processes[MAX_PROCESS];
    sysinfo_t sysinfo;
    
    SortMode current_sort = SORT_BY_MEM;
    char filter_user[MAX_NAME_LEN] = "";
    int refresh_counter = 0;
    const int REFRESH_INTERVAL = 20;
    int process_count = 0;  // Store for search feature
    int scroll_offset = 0;  // Current scroll position
    int display_count = 0;  // Number of processes after filtering
    
    while (keep_running) {
        char key = get_keypress();
        
        if (key != 0) {
            switch (key) {
                case 'w':  // Up arrow
                    if (scroll_offset > 0) {
                        scroll_offset--;
                        refresh_counter = REFRESH_INTERVAL;
                    }
                    break;
                case 'x':  // Down arrow
                    if (scroll_offset < display_count - 20 && display_count > 20) {
                        scroll_offset++;
                        refresh_counter = REFRESH_INTERVAL;
                    }
                    break;
                case 'W':  // Page Up
                    scroll_offset -= 20;
                    if (scroll_offset < 0) scroll_offset = 0;
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'X':  // Page Down
                    scroll_offset += 20;
                    if (scroll_offset > display_count - 20) {
                        scroll_offset = (display_count > 20) ? display_count - 20 : 0;
                    }
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'h':  // Home
                    scroll_offset = 0;
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'e':  // End
                    scroll_offset = (display_count > 20) ? display_count - 20 : 0;
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'p':
                case 'P':
                    current_sort = SORT_BY_PID;
                    scroll_offset = 0;  // Reset scroll on sort change
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'c':
                case 'C':
                    current_sort = SORT_BY_CPU;
                    scroll_offset = 0;  // Reset scroll on sort change
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'm':
                case 'M':
                    current_sort = SORT_BY_MEM;
                    scroll_offset = 0;  // Reset scroll on sort change
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'u':
                case 'U':
                    current_sort = SORT_BY_USER;
                    scroll_offset = 0;  // Reset scroll on sort change
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'f':
                case 'F':
                    printf("\x1b[2J\x1b[H");
                    printf(COLOR_CYAN "Enter username to filter (or press Enter for all): " COLOR_RESET);
                    printf("\x1b[?25h");
                    fflush(stdout);
                    
                    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
                    
                    if (fgets(filter_user, sizeof(filter_user), stdin)) {
                        size_t len = strlen(filter_user);
                        if (len > 0 && filter_user[len-1] == '\n') {
                            filter_user[len-1] = '\0';
                        }
                    }
                    
                    struct termios new_termios = orig_termios;
                    new_termios.c_lflag &= ~(ICANON | ECHO);
                    new_termios.c_cc[VMIN] = 0;
                    new_termios.c_cc[VTIME] = 0;
                    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
                    printf("\x1b[?25l");
                    
                    scroll_offset = 0;  // Reset scroll on filter change
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'r':
                case 'R':
                    filter_user[0] = '\0';
                    scroll_offset = 0;  // Reset scroll on filter reset
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'k':
                case 'K':
                    // Kill process - prompt for PID
                    printf("\x1b[2J\x1b[H");
                    printf(COLOR_RED "⚠️  Kill Process\n" COLOR_RESET);
                    printf(COLOR_YELLOW "Enter PID to kill (or 0 to cancel): " COLOR_RESET);
                    printf("\x1b[?25h");
                    fflush(stdout);
                    
                    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
                    
                    char pid_input[32];
                    if (fgets(pid_input, sizeof(pid_input), stdin)) {
                        int pid_to_kill = atoi(pid_input);
                        if (pid_to_kill > 0) {
                            if (kill(pid_to_kill, SIGTERM) == 0) {
                                printf(COLOR_GREEN "\n✅ Signal sent to process %d\n" COLOR_RESET, pid_to_kill);
                            } else {
                                printf(COLOR_RED "\n❌ Failed to kill process %d (Permission denied?)\n" COLOR_RESET, pid_to_kill);
                            }
                            printf(COLOR_CYAN "Press Enter to continue..." COLOR_RESET);
                            getchar();
                        }
                    }
                    
                    struct termios new_termios_k = orig_termios;
                    new_termios_k.c_lflag &= ~(ICANON | ECHO);
                    new_termios_k.c_cc[VMIN] = 0;
                    new_termios_k.c_cc[VTIME] = 0;
                    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios_k);
                    printf("\x1b[?25l");
                    
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 's':
                case 'S':
                    // Search for process
                    printf("\x1b[2J\x1b[H");
                    printf(COLOR_CYAN "🔍 Search Process\n" COLOR_RESET);
                    printf(COLOR_YELLOW "Enter process name to search: " COLOR_RESET);
                    printf("\x1b[?25h");
                    fflush(stdout);
                    
                    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
                    
                    char search_term[MAX_NAME_LEN];
                    if (fgets(search_term, sizeof(search_term), stdin)) {
                        size_t slen = strlen(search_term);
                        if (slen > 0 && search_term[slen-1] == '\n') {
                            search_term[slen-1] = '\0';
                        }
                        
                        if (strlen(search_term) > 0) {
                            // Search in current processes
                            printf(COLOR_CYAN "\nSearching for: '%s'\n" COLOR_RESET, search_term);
                            printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
                            
                            int found = 0;
                            for (int i = 0; i < process_count && i < MAX_PROCESS; i++) {
                                if (strstr(processes[i].name, search_term) != NULL || 
                                    strstr(processes[i].cmdline, search_term) != NULL) {
                                    printf(COLOR_GREEN "PID: %-6d" COLOR_RESET " User: %-10s Mem: %5.2f%% Cmd: %s\n",
                                           processes[i].pid, processes[i].user, 
                                           processes[i].mem_usage, processes[i].cmdline);
                                    found++;
                                }
                            }
                            
                            if (found == 0) {
                                printf(COLOR_YELLOW "No processes found matching '%s'\n" COLOR_RESET, search_term);
                            } else {
                                printf(COLOR_CYAN "═══════════════════════════════════════\n" COLOR_RESET);
                                printf(COLOR_GREEN "Found %d process(es)\n" COLOR_RESET, found);
                            }
                            
                            printf(COLOR_CYAN "\nPress Enter to continue..." COLOR_RESET);
                            getchar();
                        }
                    }
                    
                    struct termios new_termios_s = orig_termios;
                    new_termios_s.c_lflag &= ~(ICANON | ECHO);
                    new_termios_s.c_cc[VMIN] = 0;
                    new_termios_s.c_cc[VTIME] = 0;
                    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios_s);
                    printf("\x1b[?25l");
                    
                    refresh_counter = REFRESH_INTERVAL;
                    break;
                case 'q':
                case 'Q':
                    keep_running = 0;
                    break;
            }
        }
        
        if (++refresh_counter >= REFRESH_INTERVAL) {
            refresh_counter = 0;
            
            printf("\x1b[2J\x1b[H");
            
            get_system_info(&sysinfo);
            
            process_count = scan_processes(processes, MAX_PROCESS, sysinfo.total_mem);
            
            ProcessInfo* display_processes_ptr;
            if (strlen(filter_user) > 0) {
                display_count = filter_processes_by_user(processes, process_count, 
                                                         filtered_processes, filter_user);
                display_processes_ptr = filtered_processes;
            } else {
                display_count = process_count;
                display_processes_ptr = processes;
            }
            
            sort_processes(display_processes_ptr, display_count, current_sort);
            
            // Adjust scroll offset if out of bounds after refresh
            if (scroll_offset > display_count - 20 && display_count > 20) {
                scroll_offset = display_count - 20;
            }
            if (scroll_offset < 0) scroll_offset = 0;
            
            display_system_info(&sysinfo);
            display_processes(display_processes_ptr, display_count, scroll_offset);
            display_command_menu(current_sort, strlen(filter_user) > 0 ? filter_user : NULL, 
                               scroll_offset, display_count);
            
            fflush(stdout);
        }
        
        usleep(100000);
    }
    
    cleanup();
    
    return 0;
}
