#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H


#include "common.h"

/**
 * @brief Scans the /proc directory and fills the processes array with information about each process.      
 * 
 * @param processes 
 * @param max_processes 
 * @return int 
 */
int scan_processes(ProcessInfo processes[], int max_processes, unsigned long total_mem);

/** 
 * @brief Retrieves information about a specific process given its PID.
 * 
 * @param pid The process ID.
 * @param pinfo Pointer to a ProcessInfo structure to store the process information.
 * @param total_mem Total system memory in bytes, used to calculate memory percentage.
 * @return int Returns 0 on success, or a negative value on failure.
 */
int get_process_info(pid_t pid, ProcessInfo *pinfo, unsigned long total_mem);

/**
 * @brief Checks if a given string represents a valid process ID (PID).
 * 
 * @param str The string to check.
 * @return int Returns 1 if the string is a valid PID, 0 otherwise.
 */
int is_pid(const char* str);


/** 
 * @brief Retrieves the user ID (UID) for a given username.
 * 
 * @param username The username to look up.
 * @param uid Pointer to a uid_t variable to store the retrieved UID.
 * @return int Returns 0 on success, or a negative value on failure.
 */
int get_uid(const char* username, uid_t* uid);


/**
 * @brief Retrieves the command line of a process given its PID.
 * 
 * This function reads the command line arguments from /proc/[pid]/cmdline
 * and formats them into a single string with spaces between arguments.
 * 
 * @param pid The process ID.
 * @param buffer Pointer to a buffer where the command line will be stored.
 * @param size Size of the buffer.
 * @return int Returns the number of bytes written on success, 0 if cmdline is empty, 
 *         or a negative value on failure.
 */
int get_cmdline(pid_t pid, char *buffer, size_t size);

/**
 * @brief Gathers general system information like memory, uptime, and CPU usage.
 * 
 * Reads from /proc/meminfo and /proc/uptime to fill the sysinfo_t struct.
 * 
 * @param sysinfo Pointer to a sysinfo_t struct to be filled.
 */
void get_system_info(sysinfo_t *sysinfo);

/**
 * @brief Sorts the processes array according to the specified sort mode.
 * 
 * @param processes Array of process information to sort.
 * @param count Number of processes in the array.
 * @param mode Sorting mode (SORT_BY_PID, SORT_BY_CPU, SORT_BY_MEM, SORT_BY_USER).
 */
void sort_processes(ProcessInfo processes[], int count, SortMode mode);

/**
 * @brief Filters processes by username.
 * 
 * @param processes Source array of all processes.
 * @param count Number of processes in source array.
 * @param filtered Destination array for filtered processes.
 * @param username Username to filter by (NULL for no filter).
 * @return int Number of filtered processes.
 */
int filter_processes_by_user(const ProcessInfo processes[], int count, 
                              ProcessInfo filtered[], const char* username);

/**
 * @brief Advanced process filtering with multiple criteria.
 * 
 * @param processes Source array of all processes.
 * @param count Number of processes in source array.
 * @param filtered Destination array for filtered processes.
 * @param username Username to filter by (NULL to skip).
 * @param name_filter Process name substring to match (NULL to skip).
 * @param state_filter Process state to match (0 to skip: R/S/D/Z/T).
 * @param mem_threshold_mb Minimum memory in MB (0 to skip).
 * @return int Number of filtered processes.
 */
int filter_processes_advanced(const ProcessInfo processes[], int count,
                               ProcessInfo filtered[], const char* username,
                               const char* name_filter, char state_filter,
                               float mem_threshold_mb);

/**
 * @brief Builds process tree by calculating tree depth for each process.
 * 
 * @param processes Array of processes to build tree for.
 * @param count Number of processes.
 */
void build_process_tree(ProcessInfo processes[], int count);

#endif // PROCESS_MONITOR_H