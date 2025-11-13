#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "config.h"
#include "common.h"

// Global configuration instance
Config global_config;

// Get the config file path (~/.alttaskerrc)
const char* config_get_path(void) {
    static char path[512];
    const char *home = getenv("HOME");
    
    if (!home) {
        struct passwd *pw = getpwuid(getuid());
        home = pw->pw_dir;
    }
    
    snprintf(path, sizeof(path), "%s/.alttaskerrc", home);
    return path;
}

// Set default configuration values
void config_set_defaults(void) {
    global_config.refresh_interval = DEFAULT_REFRESH_INTERVAL;
    global_config.visible_processes = DEFAULT_VISIBLE_PROCESSES;
    global_config.default_sort = DEFAULT_SORT_BY;
    global_config.show_tree_view = DEFAULT_SHOW_TREE;
    global_config.theme = THEME_DEFAULT;
    
    // Default custom colors (ANSI codes)
    global_config.custom_colors.running = 32;    // Green
    global_config.custom_colors.sleeping = 37;   // White
    global_config.custom_colors.zombie = 31;     // Red
    global_config.custom_colors.header = 36;     // Cyan
    global_config.custom_colors.border = 36;     // Cyan
}

// Apply color theme
void config_apply_theme(ColorTheme theme) {
    global_config.theme = theme;
    
    switch (theme) {
        case THEME_DARK:
            global_config.custom_colors.running = 32;    // Green
            global_config.custom_colors.sleeping = 37;   // White
            global_config.custom_colors.zombie = 31;     // Red
            global_config.custom_colors.header = 36;     // Cyan
            global_config.custom_colors.border = 34;     // Blue
            break;
            
        case THEME_LIGHT:
            global_config.custom_colors.running = 34;    // Blue
            global_config.custom_colors.sleeping = 30;   // Black
            global_config.custom_colors.zombie = 35;     // Magenta
            global_config.custom_colors.header = 36;     // Cyan
            global_config.custom_colors.border = 33;     // Yellow
            break;
            
        case THEME_COLORBLIND:
            global_config.custom_colors.running = 36;    // Cyan
            global_config.custom_colors.sleeping = 37;   // White
            global_config.custom_colors.zombie = 33;     // Yellow
            global_config.custom_colors.header = 35;     // Magenta
            global_config.custom_colors.border = 34;     // Blue
            break;
            
        case THEME_DEFAULT:
        case THEME_CUSTOM:
        default:
            // Keep existing colors
            break;
    }
}

// Load configuration from file
void config_load(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    // Set defaults first
    config_set_defaults();
    
    if (!file) {
        // Config file doesn't exist, use defaults
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        
        // Parse key=value pairs
        char key[64], value[64];
        if (sscanf(line, "%63[^=]=%63s", key, value) == 2) {
            if (strcmp(key, "refresh_interval") == 0) {
                global_config.refresh_interval = atoi(value);
            } else if (strcmp(key, "visible_processes") == 0) {
                global_config.visible_processes = atoi(value);
            } else if (strcmp(key, "default_sort") == 0) {
                global_config.default_sort = value[0];
            } else if (strcmp(key, "show_tree_view") == 0) {
                global_config.show_tree_view = (strcmp(value, "true") == 0);
            } else if (strcmp(key, "theme") == 0) {
                if (strcmp(value, "dark") == 0) {
                    config_apply_theme(THEME_DARK);
                } else if (strcmp(value, "light") == 0) {
                    config_apply_theme(THEME_LIGHT);
                } else if (strcmp(value, "colorblind") == 0) {
                    config_apply_theme(THEME_COLORBLIND);
                } else if (strcmp(value, "custom") == 0) {
                    global_config.theme = THEME_CUSTOM;
                }
            } else if (strcmp(key, "color_running") == 0) {
                global_config.custom_colors.running = atoi(value);
            } else if (strcmp(key, "color_sleeping") == 0) {
                global_config.custom_colors.sleeping = atoi(value);
            } else if (strcmp(key, "color_zombie") == 0) {
                global_config.custom_colors.zombie = atoi(value);
            } else if (strcmp(key, "color_header") == 0) {
                global_config.custom_colors.header = atoi(value);
            } else if (strcmp(key, "color_border") == 0) {
                global_config.custom_colors.border = atoi(value);
            }
        }
    }
    
    fclose(file);
}

// Save configuration to file
void config_save(const char *filename) {
    FILE *file = fopen(filename, "w");
    
    if (!file) {
        perror("Failed to save config");
        return;
    }
    
    fprintf(file, "# AltTasker Configuration File\n");
    fprintf(file, "# Generated automatically - edit carefully\n\n");
    
    fprintf(file, "# Refresh interval in seconds (1-10)\n");
    fprintf(file, "refresh_interval=%d\n\n", global_config.refresh_interval);
    
    fprintf(file, "# Number of visible processes (10-100)\n");
    fprintf(file, "visible_processes=%d\n\n", global_config.visible_processes);
    
    fprintf(file, "# Default sort key: P (PID), C (CPU), M (Memory), U (User)\n");
    fprintf(file, "default_sort=%c\n\n", global_config.default_sort);
    
    fprintf(file, "# Show process tree view: true or false\n");
    fprintf(file, "show_tree_view=%s\n\n", global_config.show_tree_view ? "true" : "false");
    
    fprintf(file, "# Color theme: default, dark, light, colorblind, custom\n");
    const char *theme_name = "default";
    switch (global_config.theme) {
        case THEME_DARK: theme_name = "dark"; break;
        case THEME_LIGHT: theme_name = "light"; break;
        case THEME_COLORBLIND: theme_name = "colorblind"; break;
        case THEME_CUSTOM: theme_name = "custom"; break;
        default: break;
    }
    fprintf(file, "theme=%s\n\n", theme_name);
    
    if (global_config.theme == THEME_CUSTOM) {
        fprintf(file, "# Custom colors (ANSI color codes: 30-37, 90-97)\n");
        fprintf(file, "color_running=%d\n", global_config.custom_colors.running);
        fprintf(file, "color_sleeping=%d\n", global_config.custom_colors.sleeping);
        fprintf(file, "color_zombie=%d\n", global_config.custom_colors.zombie);
        fprintf(file, "color_header=%d\n", global_config.custom_colors.header);
        fprintf(file, "color_border=%d\n", global_config.custom_colors.border);
    }
    
    fclose(file);
}
