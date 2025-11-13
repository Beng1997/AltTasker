#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>

#define MAX_NAME_LEN 256
#define MAX_CMDLINE_LEN 512
#define MAX_PROCESS 1024
#define BUFFER_SIZE 512
#define PROC_DIR "/proc/"

// Sorting options
typedef enum {
    SORT_BY_PID,
    SORT_BY_CPU,
    SORT_BY_MEM,
    SORT_BY_USER
} SortMode;


// ANSI Color codes
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"

typedef struct {
    pid_t pid;
    pid_t ppid;  // Parent process ID
    char name[MAX_NAME_LEN];
    char cmdline[MAX_CMDLINE_LEN];
    char state; // Process state
    uid_t uid;
    char user[MAX_NAME_LEN];
    unsigned long vsize; // Virtual memory size
    unsigned long rss; // Resident Set Size
    time_t starttime;
    float cpu_usage;
    float mem_usage;
    unsigned long utime;  // CPU time in user mode (for tracking)
    unsigned long stime;  // CPU time in kernel mode (for tracking)
    int tree_depth;  // Depth in process tree (0 = root)
} ProcessInfo; // Process information structure

typedef struct {
    unsigned long total_mem;
    unsigned long free_mem;
    unsigned long used_mem;
    float mem_usage_percent;
    float cpu_usage_percent;
    unsigned int total_processes;
    unsigned long uptime; // System uptime in seconds
}sysinfo_t; // System information structure


#endif // COMMON_H