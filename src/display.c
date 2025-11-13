#include "display.h"


void display_system_info(const sysinfo_t* sysinfo) {
    if (!sysinfo) return;

    char total_mem_str[32]; 
    char used_mem_str[32]; 
    char free_mem_str[32];
    char uptime_str[64];

    format_memory(sysinfo->total_mem, total_mem_str, sizeof(total_mem_str));
    format_memory(sysinfo->used_mem, used_mem_str, sizeof(used_mem_str));
    format_memory(sysinfo->free_mem, free_mem_str, sizeof(free_mem_str));
    format_uptime(sysinfo->uptime, uptime_str, sizeof(uptime_str));

    // Create colored bars for memory usage
    const char* mem_color = (sysinfo->mem_usage_percent < 50.0f) ? COLOR_GREEN :
                            (sysinfo->mem_usage_percent < 75.0f) ? COLOR_YELLOW : COLOR_RED;
    
    // Header with system name
    printf(COLOR_BOLD COLOR_CYAN "╔══════════════════════════════════════════════════════════════════════════════╗\n" COLOR_RESET);
    printf(COLOR_BOLD COLOR_CYAN "║" COLOR_RESET COLOR_BOLD "                            AltTasker - System Monitor                        " COLOR_CYAN "║\n" COLOR_RESET);
    printf(COLOR_BOLD COLOR_CYAN "╚══════════════════════════════════════════════════════════════════════════════╝\n" COLOR_RESET);
    printf("\n");
    
    // System info in a nice format
    printf(COLOR_BOLD "  Uptime: " COLOR_RESET "%s" COLOR_BOLD "    |    " COLOR_RESET "Processes: " COLOR_BOLD "%u\n" COLOR_RESET, 
           uptime_str, sysinfo->total_processes);
    printf("\n");
    
    // Memory bar
    printf(COLOR_BOLD "  Memory Usage: " COLOR_RESET);
    printf("%s%.1f%%" COLOR_RESET " [%s / %s]\n", 
           mem_color, sysinfo->mem_usage_percent, used_mem_str, total_mem_str);
    
    // Visual memory bar
    int bar_width = 60;
    int filled = (int)((sysinfo->mem_usage_percent / 100.0f) * bar_width);
    printf("  [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("%s█" COLOR_RESET, mem_color);
        } else {
            printf("░");
        }
    }
    printf("]\n\n");
}


void display_processes(const ProcessInfo processes[], int count) {
    if (!processes || count <= 0) return;

    // Table header with better formatting and colors
    printf(COLOR_BOLD COLOR_CYAN "  %-6s %-10s %6s %6s %10s %10s %5s  %-40s\n" COLOR_RESET,
           "PID", "USER", "CPU%", "MEM%", "VIRT", "RES", "S", "COMMAND");
    printf(COLOR_BOLD COLOR_CYAN "  ────── ────────── ────── ────── ────────── ────────── ─────  ────────────────────────────────────────\n" COLOR_RESET);
    
    // Show top processes (limit to 30 for readability)
    int display_count = (count > 30) ? 30 : count;
    
    for (int i = 0; i < display_count; i++) {
        char vsize_str[16];
        char rss_str[16];
        char cmdline_short[50];
        char user_short[11]; // 10 chars + null terminator

        format_memory(processes[i].vsize, vsize_str, sizeof(vsize_str));
        format_memory(processes[i].rss, rss_str, sizeof(rss_str));
        
        // Truncate username if too long
        if (strlen(processes[i].user) > 10) {
            strncpy(user_short, processes[i].user, 9);
            user_short[9] = '+';
            user_short[10] = '\0';
        } else {
            strncpy(user_short, processes[i].user, sizeof(user_short));
        }
        
        // Truncate cmdline if too long
        size_t cmdline_len = strlen(processes[i].cmdline);
        if (cmdline_len > 47) {
            strncpy(cmdline_short, processes[i].cmdline, 44);
            cmdline_short[44] = '.';
            cmdline_short[45] = '.';
            cmdline_short[46] = '.';
            cmdline_short[47] = '\0';
        } else {
            strncpy(cmdline_short, processes[i].cmdline, sizeof(cmdline_short) - 1);
            cmdline_short[sizeof(cmdline_short) - 1] = '\0';
        }

        // Color code based on memory usage
        const char* row_color = "";
        if (processes[i].mem_usage > 5.0f) {
            row_color = COLOR_RED;
        } else if (processes[i].mem_usage > 2.0f) {
            row_color = COLOR_YELLOW;
        }

        printf("%s  %-6d %-10s %6.1f %6.2f %10s %10s %s%c%s     %-40s%s\n",
               row_color,
               processes[i].pid,
               user_short,
               processes[i].cpu_usage,
               processes[i].mem_usage,
               vsize_str,
               rss_str,
               get_state_color(processes[i].state),
               processes[i].state,
               COLOR_RESET,
               cmdline_short,
               COLOR_RESET);
    }
    
    if (count > display_count) {
        printf(COLOR_BOLD "\n  ... and %d more processes\n" COLOR_RESET, count - display_count);
    }
    
    printf("\n");
    printf(COLOR_BOLD "  Press " COLOR_GREEN "Ctrl+C" COLOR_RESET COLOR_BOLD " to exit\n" COLOR_RESET);
}

void format_memory(unsigned long size, char* buffer, size_t buffer_size) {
    // Validate input parameters
    if (!buffer || buffer_size < 16) {
        return;  // Need at least 16 bytes for "999.99 TB" + null
    }
    
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double display_size = (double)size;

    while (display_size >= 1024 && unit_index < 4) {
        display_size /= 1024;
        unit_index++;
    }

    snprintf(buffer, buffer_size, "%.2f %s", display_size, units[unit_index]);
}


void apply_color(float load, char* buffer, size_t buffer_size) {
    // Validate input parameters
    if (!buffer || buffer_size < 32) {
        return;  // Need enough space for color codes + value
    }
    
    // Apply color based on load percentage
    if (load < 50.0f) {
        snprintf(buffer, buffer_size, COLOR_GREEN "%.2f%%" COLOR_RESET, load);
    } else if (load < 75.0f) {
        snprintf(buffer, buffer_size, COLOR_YELLOW "%.2f%%" COLOR_RESET, load);
    } else {
        snprintf(buffer, buffer_size, COLOR_RED "%.2f%%" COLOR_RESET, load);
    }
}

void format_uptime(unsigned long seconds, char* buffer, size_t buffer_size) {
    // Validate input parameters
    if (!buffer || buffer_size < 32) {
        return;
    }
    
    unsigned long days = seconds / 86400;
    unsigned long hours = (seconds % 86400) / 3600;
    unsigned long mins = (seconds % 3600) / 60;
    unsigned long secs = seconds % 60;
    
    if (days > 0) {
        snprintf(buffer, buffer_size, "%lu days, %02lu:%02lu:%02lu", 
                 days, hours, mins, secs);
    } else {
        snprintf(buffer, buffer_size, "%02lu:%02lu:%02lu", hours, mins, secs);
    }
}

const char* get_state_color(char state) {
    switch (state) {
        case 'R': return COLOR_GREEN;   // Running - green
        case 'S': return "";            // Sleeping - default
        case 'D': return COLOR_YELLOW;  // Disk wait - yellow
        case 'Z': return COLOR_RED;     // Zombie - red
        case 'T': return COLOR_CYAN;    // Stopped - cyan
        case 't': return COLOR_CYAN;    // Tracing stop - cyan
        case 'X': return COLOR_RED;     // Dead - red
        case 'I': return COLOR_BLUE;    // Idle - blue
        default:  return "";            // Unknown - default
    }
}