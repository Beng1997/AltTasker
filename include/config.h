#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Default configuration values
#define DEFAULT_REFRESH_INTERVAL 2
#define DEFAULT_VISIBLE_PROCESSES 20
#define DEFAULT_SORT_BY 'M'  // Memory
#define DEFAULT_SHOW_TREE false

// Color theme types
typedef enum {
    THEME_DEFAULT = 0,
    THEME_DARK,
    THEME_LIGHT,
    THEME_COLORBLIND,
    THEME_CUSTOM
} ColorTheme;

// Configuration structure
typedef struct {
    int refresh_interval;       // Refresh rate in seconds
    int visible_processes;      // Number of processes per page
    char default_sort;          // Default sort key (P/C/M/U)
    bool show_tree_view;        // Enable process tree
    ColorTheme theme;           // Color theme
    
    // Custom colors (if THEME_CUSTOM)
    struct {
        int running;            // Color for running processes
        int sleeping;           // Color for sleeping processes
        int zombie;             // Color for zombie processes
        int header;             // Color for headers
        int border;             // Color for borders
    } custom_colors;
} Config;

// Global config instance
extern Config global_config;

// Function declarations
void config_load(const char *filename);
void config_save(const char *filename);
void config_set_defaults(void);
const char* config_get_path(void);
void config_apply_theme(ColorTheme theme);

// Dynamic color getters
const char* config_get_header_color(void);
const char* config_get_border_color(void);
const char* config_get_running_color(void);
const char* config_get_sleeping_color(void);
const char* config_get_zombie_color(void);

#endif // CONFIG_H
