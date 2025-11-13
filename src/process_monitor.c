#include "process_monitor.h"


int scan_processes(ProcessInfo processes[], int max_processes) {
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
            if (get_process_info(pid, &processes[count]) == 0) {
                count++;
            }
        }
    }
    
    closedir(proc_dir);
    return count;  // Return number of processes scanned, not 0!
}




int get_process_info(pid_t pid, ProcessInfo *pinfo) {
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
    
    // Calculate CPU usage (simplified - actual calculation requires tracking over time)
    // For now, store total CPU time in jiffies
    pinfo->cpu_usage = (float)(utime + stime) / sysconf(_SC_CLK_TCK);
    
    // Store start time
    pinfo->starttime = starttime / sysconf(_SC_CLK_TCK);
    
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
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    fp = fopen(path, "r");
    if (fp) {
        size_t bytes_read = fread(pinfo->cmdline, 1, MAX_CMDLINE_LEN - 1, fp);
        fclose(fp);
        
        if (bytes_read > 0) {
            // cmdline has null-separated arguments, replace nulls with spaces
            for (size_t i = 0; i < bytes_read - 1; i++) {
                if (pinfo->cmdline[i] == '\0') {
                    pinfo->cmdline[i] = ' ';
                }
            }
            pinfo->cmdline[bytes_read] = '\0';
            
            // Remove trailing spaces
            while (bytes_read > 0 && pinfo->cmdline[bytes_read - 1] == ' ') {
                pinfo->cmdline[--bytes_read] = '\0';
            }
        } else {
            // If cmdline is empty, use the process name in brackets (kernel threads)
            snprintf(pinfo->cmdline, MAX_CMDLINE_LEN, "[%s]", pinfo->name);
        }
    } else {
        // If cmdline cannot be read, use process name
        snprintf(pinfo->cmdline, MAX_CMDLINE_LEN, "[%s]", pinfo->name);
    }
    
    // ========================================================================
    // 4. Calculate memory usage percentage
    // ========================================================================
    // Read total memory from /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        unsigned long total_mem_kb = 0;
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (sscanf(buffer, "MemTotal: %lu kB", &total_mem_kb) == 1) {
                break;
            }
        }
        fclose(fp);
        
        if (total_mem_kb > 0) {
            // Convert RSS from bytes to KB and calculate percentage
            unsigned long rss_kb = pinfo->rss / 1024;
            pinfo->mem_usage = (float)rss_kb / (float)total_mem_kb * 100.0f;
        }
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