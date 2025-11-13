#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

/**
 * @brief Displays system information in a formatted manner.
 * 
 * @param sysinfo Pointer to a sysinfo_t structure containing system information.
 */
void display_system_info(const sysinfo_t* sysinfo);
/**
 * @brief Displays the list of processes in a formatted table.
 * 
 * @param processes Array of ProcessInfo structures containing process information.
 * @param count Number of processes in the array.
 */
void display_processes(const ProcessInfo processes[], int count);


/**
 * @brief Formats a memory size in bytes into a human-readable string (e.g., KB, MB, GB).
 * 
 * @param size Memory size in bytes.
 * @param buffer Pointer to a buffer where the formatted string will be stored.
 * @param buffer_size Size of the buffer.
 */
void format_memory(unsigned long size, char* buffer, size_t buffer_size);

/**
 * @brief Applies color coding to a load value (CPU/Memory) based on its percentage.
 * 
 * Color scheme:
 * - Green:  < 50%
 * - Yellow: 50-75%
 * - Red:    > 75%
 * 
 * @param load Load percentage value (0-100).
 * @param buffer Pointer to a buffer where the colored string will be stored.
 * @param buffer_size Size of the buffer.
 */
void apply_color(float load, char* buffer, size_t buffer_size);

/**
 * @brief Formats uptime in seconds to a human-readable string (days, hours, minutes, seconds).
 * 
 * @param seconds Uptime in seconds.
 * @param buffer Pointer to a buffer where the formatted string will be stored.
 * @param buffer_size Size of the buffer.
 */
void format_uptime(unsigned long seconds, char* buffer, size_t buffer_size);

/**
 * @brief Returns the ANSI color code for a process state.
 * 
 * @param state Process state character (R, S, D, Z, T, etc.).
 * @return const char* ANSI color code string.
 */
const char* get_state_color(char state);

#endif // DISPLAY_H