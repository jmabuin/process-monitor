/**
 * @file PapiInfo.h
 * @brief Header file for the Process Utils.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_PROCESSUTILS_H
#define PROCESS_MONITOR_PROCESSUTILS_H


#include <string>
#include <vector>

/**
 * @brief Class containing different utility functions for processes.
 */
class ProcessUtils {

public:
    /**
     * @brief Finds the PID of a process with the given parent PID and cmd line.
     * @param parent PID of parent process.
     * @param cmdline The cmdline of the process we are looking for.
     * @return PID of the process, or -1 if it is not found.
     */
    static pid_t FindChildrenDirect(pid_t parent, std::string_view cmdline);

    /**
     * @brief Converts a vector of string in a single string concatenating spaces.
     * @param items Items being concatenated.
     * @return The formed string.
     */
    static std::string CmdToString(const std::vector<std::string>& items);
};


#endif //PROCESS_MONITOR_PROCESSUTILS_H
