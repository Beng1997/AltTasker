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
int scan_processes(ProcessInfo processes[], int max_processes);

/** 
 * @brief Retrieves information about a specific process given its PID.
 * 
 * @param pid The process ID.
 * @param pinfo Pointer to a ProcessInfo structure to store the process information.
 * @return int Returns 0 on success, or a negative value on failure.
 */
int get_process_info(pid_t pid, ProcessInfo *pinfo);

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
 * @param pid The process ID.
 * @return int Returns 0 on success, or a negative value on failure.
 */
int get_cmdline(pid_t pid);

#endif // PROCESS_MONITOR_H