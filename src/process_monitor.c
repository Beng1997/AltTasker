#include "process_monitor.h"


int scan_processes(ProcessInfo processes[], int max_processes, unsigned long total_mem) {
    // Validate input parameters
    if (!processes || max_processes <= 0) {
        return -1;
    }
    
    // Open /proc directory
    DIR *proc_dir = opendir(PROC_DIR);
    if (!proc_dir) {
        perror("opendir /proc");
        return -1;
    }
    
    struct dirent *entry; 
    int count = 0;
    
    while ((entry = readdir(proc_dir)) != NULL && count < max_processes) {
        if (is_pid(entry->d_name)) {
            pid_t pid = (pid_t)atoi(entry->d_name);
            // Pass total_mem to get_process_info for efficient memory % calculation
            if (get_process_info(pid, &processes[count], total_mem) == 0) {
                count++;
            }
        }
    }
    
    closedir(proc_dir);
    return count;  // Return number of processes scanned
}




int get_process_info(pid_t pid, ProcessInfo *pinfo, unsigned long total_mem) {
    if (!pinfo) return -1;
    
    char path[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    FILE *fp;
    
    // Initialize the structure
    memset(pinfo, 0, sizeof(ProcessInfo));
    pinfo->pid = pid;
    
    // ========================================================================
    // 1. Read from /proc/[pid]/stat - contains most process information
    // ========================================================================
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if (!fp) {
        return -1;  // Process might have terminated
    }
    
    // Parse /proc/[pid]/stat
    // Format: pid (comm) state ppid pgrp session tty_nr tpgid flags minflt cminflt majflt cmajflt utime stime cutime cstime ...
    unsigned long utime, stime;
    unsigned long long starttime;
    long rss_pages;
    
    if (fscanf(fp, "%*d (%255[^)]) %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*d %*d %*d %*d %llu %lu %ld",
               pinfo->name,      // Process name (between parentheses)
               &pinfo->state,    // Process state (R, S, D, Z, T, etc.)
               &utime,           // CPU time in user mode (jiffies)
               &stime,           // CPU time in kernel mode (jiffies)
               &starttime,       // Start time since boot (jiffies)
               &pinfo->vsize,    // Virtual memory size (bytes)
               &rss_pages        // Resident Set Size (pages)
    ) < 7) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    
    // Convert RSS from pages to bytes (typically 4096 bytes per page)
    pinfo->rss = rss_pages * sysconf(_SC_PAGESIZE);
    
    // Store CPU time for later tracking/calculation
    pinfo->utime = utime;
    pinfo->stime = stime;
    
    // Store start time (in seconds since boot)
    pinfo->starttime = starttime / sysconf(_SC_CLK_TCK);
    
    // Calculate CPU % based on process lifetime
    // Get system uptime
    FILE *uptime_fp = fopen("/proc/uptime", "r");
    float system_uptime = 0;
    if (uptime_fp) {
        if (fscanf(uptime_fp, "%f", &system_uptime) != 1) {
            system_uptime = 0;
        }
        fclose(uptime_fp);
    }
    
    // Calculate process uptime
    float process_uptime = system_uptime - pinfo->starttime;
    if (process_uptime > 0) {
        // CPU usage = (total CPU time / process uptime) * 100
        float total_cpu_seconds = (float)(utime + stime) / sysconf(_SC_CLK_TCK);
        pinfo->cpu_usage = (total_cpu_seconds / process_uptime) * 100.0f;
        // Cap at reasonable value (can exceed 100% on multi-core)
        if (pinfo->cpu_usage > 999.9f) pinfo->cpu_usage = 999.9f;
    } else {
        pinfo->cpu_usage = 0.0f;
    }
    
    // ========================================================================
    // 2. Read from /proc/[pid]/status - contains UID and other details
    // ========================================================================
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    fp = fopen(path, "r");
    if (!fp) {
        return -1;  // Process might have terminated
    }
    
    // Parse status file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        // Look for Uid line: "Uid: <real> <effective> <saved> <filesystem>"
        if (sscanf(buffer, "Uid:\t%u", &pinfo->uid) == 1) {
            break;
        }
    }
    fclose(fp);
    
    // Convert UID to username
    struct passwd *pw = getpwuid(pinfo->uid);
    if (pw) {
        strncpy(pinfo->user, pw->pw_name, MAX_NAME_LEN - 1);
        pinfo->user[MAX_NAME_LEN - 1] = '\0';
    } else {
        // If username not found, use UID as string
        snprintf(pinfo->user, MAX_NAME_LEN, "%u", pinfo->uid);
    }
    
    // ========================================================================
    // 3. Read from /proc/[pid]/cmdline - contains full command line
    // ========================================================================
    get_cmdline(pid, pinfo->cmdline, MAX_CMDLINE_LEN);
    if (pinfo->cmdline[0] == '\0') {
        // If cmdline is empty, use the process name in brackets (kernel threads)
        snprintf(pinfo->cmdline, MAX_CMDLINE_LEN, "[%s]", pinfo->name);
    }
    
    // ========================================================================
    // 4. Calculate memory usage percentage
    // ========================================================================
    if (total_mem > 0) {
        pinfo->mem_usage = (float)pinfo->rss / (float)total_mem * 100.0f;
    } else {
        pinfo->mem_usage = 0.0f;
    }
    
    return 0;
}

int is_pid(const char* str) {
    if (!str || *str == '\0') return 0;  // Check for NULL or empty string
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}


int get_uid(const char* username, uid_t* uid) {
    if (!username || !uid) return -1;
    struct passwd *pw = getpwnam(username);
    if (!pw) {
        return -1; // User not found
    }
    *uid = pw->pw_uid;
    return 0;
}

int get_cmdline(pid_t pid, char *buffer, size_t size) {
    // Validate input parameters
    if (!buffer || size == 0) {
        return -1;
    }
    
    // Build path to /proc/[pid]/cmdline
    char path[BUFFER_SIZE];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    
    // Open cmdline file
    FILE *fp = fopen(path, "r");
    if (!fp) {
        return -1; // Could not open cmdline (process may have terminated or no permission)
    }
    
    // Read cmdline into buffer
    size_t bytes_read = fread(buffer, 1, size - 1, fp);
    fclose(fp);
    
    // Handle empty cmdline (kernel threads)
    if (bytes_read == 0) {
        buffer[0] = '\0';
        return 0;
    }
    
    // ========================================================================
    // Process the cmdline: replace null bytes with spaces
    // ========================================================================
    // The cmdline file contains arguments separated by null bytes:
    // Example: "/usr/bin/vim\0project.c\0" -> "/usr/bin/vim project.c"
    
    for (size_t i = 0; i < bytes_read - 1; i++) {
        if (buffer[i] == '\0') {
            buffer[i] = ' ';  // Replace null with space
        }
    }
    
    // Ensure the string is null-terminated
    buffer[bytes_read] = '\0';
    
    // Remove trailing spaces (if any)
    while (bytes_read > 0 && buffer[bytes_read - 1] == ' ') {
        buffer[--bytes_read] = '\0';
    }
    
    return (int)bytes_read;  // Return number of bytes written
}

void get_system_info(sysinfo_t *sysinfo) {
    if (!sysinfo) return;
    
    FILE *fp;
    char buffer[BUFFER_SIZE];
    
    // Initialize
    memset(sysinfo, 0, sizeof(sysinfo_t));
    
    // Read memory info from /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        unsigned long total_mem = 0, free_mem = 0, buffers = 0, cached = 0;
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (sscanf(buffer, "MemTotal: %lu kB", &total_mem) == 1) sysinfo->total_mem = total_mem * 1024;
            if (sscanf(buffer, "MemFree: %lu kB", &free_mem) == 1) sysinfo->free_mem = free_mem * 1024;
            if (sscanf(buffer, "Buffers: %lu kB", &buffers) == 1) {} // Part of free
            if (sscanf(buffer, "Cached: %lu kB", &cached) == 1) {}   // Part of free
        }
        fclose(fp);
        
        sysinfo->used_mem = sysinfo->total_mem - sysinfo->free_mem;
        if (sysinfo->total_mem > 0) {
            sysinfo->mem_usage_percent = (float)sysinfo->used_mem / sysinfo->total_mem * 100.0f;
        }
    }
    
    // Read uptime from /proc/uptime
    fp = fopen("/proc/uptime", "r");
    if (fp) {
        double uptime_seconds;
        if (fscanf(fp, "%lf", &uptime_seconds) == 1) {
            sysinfo->uptime = (unsigned long)uptime_seconds;
        }
        fclose(fp);
    }
    
    // Count total processes by scanning /proc directory
    DIR *proc_dir = opendir(PROC_DIR);
    if (proc_dir) {
        struct dirent *entry;
        while ((entry = readdir(proc_dir)) != NULL) {
            if (is_pid(entry->d_name)) {
                sysinfo->total_processes++;
            }
        }
        closedir(proc_dir);
    }
}

// Comparison functions for qsort
static int compare_by_pid(const void* a, const void* b) {
    const ProcessInfo* pa = (const ProcessInfo*)a;
    const ProcessInfo* pb = (const ProcessInfo*)b;
    return pa->pid - pb->pid;
}

static int compare_by_cpu(const void* a, const void* b) {
    const ProcessInfo* pa = (const ProcessInfo*)a;
    const ProcessInfo* pb = (const ProcessInfo*)b;
    // Descending order (highest CPU first)
    if (pb->cpu_usage > pa->cpu_usage) return 1;
    if (pb->cpu_usage < pa->cpu_usage) return -1;
    return 0;
}

static int compare_by_mem(const void* a, const void* b) {
    const ProcessInfo* pa = (const ProcessInfo*)a;
    const ProcessInfo* pb = (const ProcessInfo*)b;
    // Descending order (highest memory first)
    if (pb->mem_usage > pa->mem_usage) return 1;
    if (pb->mem_usage < pa->mem_usage) return -1;
    return 0;
}

static int compare_by_user(const void* a, const void* b) {
    const ProcessInfo* pa = (const ProcessInfo*)a;
    const ProcessInfo* pb = (const ProcessInfo*)b;
    return strcmp(pa->user, pb->user);
}

void sort_processes(ProcessInfo processes[], int count, SortMode mode) {
    if (!processes || count <= 0) return;
    
    switch (mode) {
        case SORT_BY_PID:
            qsort(processes, count, sizeof(ProcessInfo), compare_by_pid);
            break;
        case SORT_BY_CPU:
            qsort(processes, count, sizeof(ProcessInfo), compare_by_cpu);
            break;
        case SORT_BY_MEM:
            qsort(processes, count, sizeof(ProcessInfo), compare_by_mem);
            break;
        case SORT_BY_USER:
            qsort(processes, count, sizeof(ProcessInfo), compare_by_user);
            break;
    }
}

int filter_processes_by_user(const ProcessInfo processes[], int count, 
                              ProcessInfo filtered[], const char* username) {
    if (!processes || !filtered || count <= 0) return 0;
    
    // If no username provided, copy all processes
    if (!username || strlen(username) == 0) {
        memcpy(filtered, processes, count * sizeof(ProcessInfo));
        return count;
    }
    
    int filtered_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(processes[i].user, username) == 0) {
            filtered[filtered_count++] = processes[i];
        }
    }
    
    return filtered_count;
}