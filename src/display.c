#include "display.h"


void display_system_info(const sysinfo_t* sysinfo) {
    if (!sysinfo) return;

    char total_mem_str[32]; 
    char used_mem_str[32]; 
    char free_mem_str[32];
    char uptime_str[64];
    char mem_colored[64];
    char cpu_colored[64];

    format_memory(sysinfo->total_mem, total_mem_str, sizeof(total_mem_str));
    format_memory(sysinfo->used_mem, used_mem_str, sizeof(used_mem_str));
    format_memory(sysinfo->free_mem, free_mem_str, sizeof(free_mem_str));
    format_uptime(sysinfo->uptime, uptime_str, sizeof(uptime_str));
    apply_color(sysinfo->mem_usage_percent, mem_colored, sizeof(mem_colored));
    apply_color(sysinfo->cpu_usage_percent, cpu_colored, sizeof(cpu_colored));

    printf(COLOR_BOLD "System Information:\n" COLOR_RESET);
    printf("Uptime: %s\n", uptime_str);
    printf("Total Memory: %s\n", total_mem_str);
    printf("Used Memory: %s\n", used_mem_str);
    printf("Free Memory: %s\n", free_mem_str);
    printf("Memory Usage: %s\n", mem_colored);
    printf("CPU Usage: %s\n", cpu_colored);
    printf("Total Processes: %u\n", sysinfo->total_processes);
    printf("\n");
}


void display_processes(const ProcessInfo processes[], int count) {
    if (!processes || count <= 0) return;

    printf(COLOR_BOLD "PID\tUSER\tCPU%%\tMEM%%\tVSZ\tRSS\tSTATE\tCOMMAND\n" COLOR_RESET);
    for (int i = 0; i < count; i++) {
        char vsize_str[32];
        char rss_str[32];
        char cmdline_short[64];

        format_memory(processes[i].vsize, vsize_str, sizeof(vsize_str));
        format_memory(processes[i].rss, rss_str, sizeof(rss_str));
        
        // Truncate cmdline if too long
        size_t cmdline_len = strlen(processes[i].cmdline);
        if (cmdline_len > 60) {
            strncpy(cmdline_short, processes[i].cmdline, 57);
            cmdline_short[57] = '.';
            cmdline_short[58] = '.';
            cmdline_short[59] = '.';
            cmdline_short[60] = '\0';
        } else {
            strncpy(cmdline_short, processes[i].cmdline, sizeof(cmdline_short) - 1);
            cmdline_short[sizeof(cmdline_short) - 1] = '\0';
        }

        printf("%d\t%s\t%.2f\t%.2f\t%s\t%s\t%s%c%s\t%s\n",
               processes[i].pid,
               processes[i].user,
               processes[i].cpu_usage,
               processes[i].mem_usage,
               vsize_str,
               rss_str,
               get_state_color(processes[i].state),
               processes[i].state,
               COLOR_RESET,
               cmdline_short);
    }
    printf("\n");
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