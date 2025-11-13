#include "display.h"
#include "config.h"


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

    // Create colored bars for memory usage with warning colors
    const char* mem_color = (sysinfo->mem_usage_percent < 50.0f) ? COLOR_GREEN :
                            (sysinfo->mem_usage_percent < 75.0f) ? COLOR_YELLOW : COLOR_RED;
    
    // Header with system name - Yellow border for visibility
    printf(COLOR_BOLD COLOR_YELLOW "╔══════════════════════════════════════════════════════════════════════════════╗\n" COLOR_RESET);
    printf(COLOR_BOLD COLOR_YELLOW "║" COLOR_RESET COLOR_BOLD "%s                            AltTasker - System Monitor                        " COLOR_YELLOW "║\n" COLOR_RESET, config_get_header_color());
    printf(COLOR_BOLD COLOR_YELLOW "╚══════════════════════════════════════════════════════════════════════════════╝\n" COLOR_RESET);
    printf("\n");
    
    // System info in a nice format with icons and colors
    printf(COLOR_BOLD "%s  ⏱️  Uptime: " COLOR_RESET "%s" COLOR_BOLD "%s    |    📊 Processes: " COLOR_RESET COLOR_BOLD "%u\n" COLOR_RESET, 
           config_get_header_color(), uptime_str, config_get_header_color(), sysinfo->total_processes);
    printf("\n");
    
    // Memory bar with color-coded percentage
    printf(COLOR_BOLD "%s  💾 Memory Usage: " COLOR_RESET, config_get_header_color());
    printf("%s%.1f%%" COLOR_RESET " [%s / %s]\n", 
           mem_color, sysinfo->mem_usage_percent, used_mem_str, total_mem_str);
    
    // Visual memory bar with gradient colors
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


void display_processes(const ProcessInfo processes[], int count, int scroll_offset, int visible_processes) {
    if (!processes || count <= 0) return;

    // Table header with better formatting and colors
    printf(COLOR_BOLD "%s  %-6s %-10s %6s %6s %10s %10s %-5s  %-45s\n" COLOR_RESET,
           config_get_header_color(), "PID", "USER", "CPU%", "MEM%", "VIRT", "RES", "STATE", "COMMAND");
    printf("%s  ────── ────────── ────── ────── ────────── ────────── ─────  ─────────────────────────────────────────────\n" COLOR_RESET, config_get_border_color());
    
    // Show processes with scrolling support
    int start_index = scroll_offset;
    int end_index = (start_index + visible_processes > count) ? count : start_index + visible_processes;
    int display_count = end_index - start_index;
    
    for (int i = 0; i < display_count; i++) {
        int proc_index = start_index + i;
        char vsize_str[16];
        char rss_str[16];
        char cmdline_short[48];
        char user_short[11]; // 10 chars + null terminator

        format_memory(processes[proc_index].vsize, vsize_str, sizeof(vsize_str));
        format_memory(processes[proc_index].rss, rss_str, sizeof(rss_str));
        
        // Truncate username if too long
        if (strlen(processes[proc_index].user) > 10) {
            strncpy(user_short, processes[proc_index].user, 9);
            user_short[9] = '+';
            user_short[10] = '\0';
        } else {
            strncpy(user_short, processes[proc_index].user, sizeof(user_short));
        }
        
        // Truncate cmdline if too long (account for tree indentation)
        int tree_indent = processes[proc_index].tree_depth * 2;  // 2 chars per level
        int max_cmd_len = 44 - tree_indent;
        if (max_cmd_len < 10) max_cmd_len = 10;  // Minimum space for command
        
        // Build tree prefix
        char tree_prefix[64] = "";
        if (global_config.show_tree_view && processes[proc_index].tree_depth > 0) {
            for (int d = 0; d < processes[proc_index].tree_depth && d < 20; d++) {
                if (d == processes[proc_index].tree_depth - 1) {
                    strcat(tree_prefix, "└─");
                } else {
                    strcat(tree_prefix, "  ");
                }
            }
        }
        
        // Format command with tree prefix
        char cmdline_with_tree[128];
        snprintf(cmdline_with_tree, sizeof(cmdline_with_tree), "%s%s", 
                 tree_prefix, processes[proc_index].cmdline);
        
        size_t cmdline_len = strlen(cmdline_with_tree);
        if (cmdline_len > 44) {
            strncpy(cmdline_short, cmdline_with_tree, 41);
            cmdline_short[41] = '.';
            cmdline_short[42] = '.';
            cmdline_short[43] = '.';
            cmdline_short[44] = '\0';
        } else {
            strncpy(cmdline_short, cmdline_with_tree, sizeof(cmdline_short) - 1);
            cmdline_short[sizeof(cmdline_short) - 1] = '\0';
        }

        // Color code based on memory usage
        const char* row_color = "";
        if (processes[proc_index].mem_usage > 5.0f) {
            row_color = COLOR_RED;
        } else if (processes[proc_index].mem_usage > 2.0f) {
            row_color = COLOR_YELLOW;
        }
        
        // Get state description
        const char* state_desc;
        switch (processes[proc_index].state) {
            case 'R': state_desc = COLOR_GREEN "RUN  " COLOR_RESET; break;
            case 'S': state_desc = "SLEEP"; break;
            case 'D': state_desc = COLOR_YELLOW "DISK " COLOR_RESET; break;
            case 'Z': state_desc = COLOR_RED "ZOMBI" COLOR_RESET; break;
            case 'T': state_desc = COLOR_CYAN "STOP " COLOR_RESET; break;
            case 'I': state_desc = COLOR_BLUE "IDLE " COLOR_RESET; break;
            default:  state_desc = "?????"; break;
        }

        printf("%s  %-6d %-10s %6.1f %6.2f %10s %10s %-5s  %-45s%s\n",
               row_color,
               processes[proc_index].pid,
               user_short,
               processes[proc_index].cpu_usage,
               processes[proc_index].mem_usage,
               vsize_str,
               rss_str,
               state_desc,
               cmdline_short,
               COLOR_RESET);
    }
    
    // Show scroll position info
    if (count > visible_processes) {
        printf("\n");
        printf(COLOR_BOLD "  Showing %d-%d of %d processes" COLOR_RESET, 
               start_index + 1, end_index, count);
        
        // Add scroll indicators
        if (scroll_offset > 0) {
            printf(COLOR_GREEN " ▲ More above" COLOR_RESET);
        }
        if (end_index < count) {
            printf(COLOR_GREEN " ▼ More below" COLOR_RESET);
        }
        printf("\n");
    } else if (count > 0) {
        printf("\n");
        printf(COLOR_BOLD "  Showing all %d processes\n" COLOR_RESET, count);
    }
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

void display_command_menu(SortMode current_sort, const char* filter_user, int scroll_offset, int total_processes) {
    (void)scroll_offset;  // For future use if needed
    
    printf("\n");
    printf("%s  ╔══════════════════════════════════════════════════════════════════════════════════════╗\n" COLOR_RESET, config_get_border_color());
    // Commands (8 chars + 78 spaces = 86)
    printf("%s  ║ " COLOR_RESET COLOR_BOLD COLOR_YELLOW "Commands" COLOR_RESET "                                                                              %s║\n" COLOR_RESET, config_get_border_color(), config_get_border_color());
    printf("%s  ╠══════════════════════════════════════════════════════════════════════════════════════╣\n" COLOR_RESET, config_get_border_color());
    
    // Sort options with highlighted current mode
    const char* sort_p = (current_sort == SORT_BY_PID) ? COLOR_GREEN "P" COLOR_RESET : COLOR_BOLD "P" COLOR_RESET;
    const char* sort_c = (current_sort == SORT_BY_CPU) ? COLOR_GREEN "C" COLOR_RESET : COLOR_BOLD "C" COLOR_RESET;
    const char* sort_m = (current_sort == SORT_BY_MEM) ? COLOR_GREEN "M" COLOR_RESET : COLOR_BOLD "M" COLOR_RESET;
    const char* sort_u = (current_sort == SORT_BY_USER) ? COLOR_GREEN "U" COLOR_RESET : COLOR_BOLD "U" COLOR_RESET;
    
    // Current sort indicator
    const char* sort_indicator;
    switch (current_sort) {
        case SORT_BY_CPU: sort_indicator = COLOR_GREEN "CPU↓" COLOR_RESET; break;
        case SORT_BY_MEM: sort_indicator = COLOR_GREEN "MEM↓" COLOR_RESET; break;
        case SORT_BY_USER: sort_indicator = COLOR_GREEN "USER↓" COLOR_RESET; break;
        case SORT_BY_PID: 
        default: sort_indicator = COLOR_GREEN "PID↓" COLOR_RESET; break;
    }
    
    // Sort: PID  CPU  Memory  User     Current: MEM↓ (46 chars + 40 spaces = 86)
    printf("%s  ║ " COLOR_RESET COLOR_YELLOW "Sort:" COLOR_RESET " %sID  %sPU  %semory  %sser     Current: %s                                        %s║\n" COLOR_RESET, 
           config_get_border_color(), sort_p, sort_c, sort_m, sort_u, sort_indicator, config_get_border_color());
    
    // Filter: F User  R Reset (23 chars + 63 spaces = 86)
    if (filter_user && strlen(filter_user) > 0) {
        // Filter: Active: username
        char filter_line[128];
        int remaining = 86 - 16 - (int)strlen(filter_user);  // 86 total - "Filter: Active: " - username length
        if (remaining < 0) remaining = 0;
        snprintf(filter_line, sizeof(filter_line), "%*s", remaining, "");
        printf("%s  ║ " COLOR_RESET COLOR_YELLOW "Filter:" COLOR_RESET " " COLOR_GREEN "Active: %s" COLOR_RESET "%s%s║\n" COLOR_RESET, 
               config_get_border_color(), filter_user, filter_line, config_get_border_color());
    } else {
        printf("%s  ║ " COLOR_RESET COLOR_YELLOW "Filter:" COLOR_RESET " " COLOR_BOLD "F" COLOR_RESET " User  " COLOR_BOLD "R" COLOR_RESET " Reset                                                               %s║\n" COLOR_RESET, 
               config_get_border_color(), config_get_border_color());
    }
    
    // Navigate: ↑/↓ Line  PgUp/PgDn Page  Home/End Top/Bottom (55 chars + 31 spaces = 86)
    if (total_processes > 20) {
        printf("%s  ║ " COLOR_RESET COLOR_YELLOW "Navigate:" COLOR_RESET " " COLOR_BOLD "↑" COLOR_RESET "/" COLOR_BOLD "↓" COLOR_RESET " Line  " COLOR_BOLD "PgUp" COLOR_RESET "/" COLOR_BOLD "PgDn" COLOR_RESET " Page  " COLOR_BOLD "Home" COLOR_RESET "/" COLOR_BOLD "End" COLOR_RESET " Top/Bottom                               %s║\n" COLOR_RESET, 
               config_get_border_color(), config_get_border_color());
    }
    
    // Actions: K Kill  S Search  Q/Ctrl+C Quit (40 chars + 46 spaces = 86)
    printf("%s  ║ " COLOR_RESET COLOR_YELLOW "Actions:" COLOR_RESET " " COLOR_RED "K" COLOR_RESET " Kill  " COLOR_CYAN "S" COLOR_RESET " Search  " COLOR_BOLD "Q" COLOR_RESET "/" COLOR_BOLD "Ctrl+C" COLOR_RESET " Quit                                              %s║\n" COLOR_RESET, 
           config_get_border_color(), config_get_border_color());
    printf("%s  ╚══════════════════════════════════════════════════════════════════════════════════════╝\n" COLOR_RESET, config_get_border_color());
    printf(COLOR_BOLD "  Auto-refresh: 2s" COLOR_RESET "  |  Press any key above to execute\n");
}