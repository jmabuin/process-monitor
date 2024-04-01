//
// Created by chema on 2/15/24.
//

#ifndef PROCESS_MONITOR_ARGS_H
#define PROCESS_MONITOR_ARGS_H

#include <string>

/**
 * @brief Struct storing input arguments.
 */
struct Args {

    /// @brief Path to the JSON configuration file.
    std::string config_file;

    /// @brief Indicates whether to show debugging information or not.
    bool debug_mode = false;

    /// @brief PID of the process being monitored/watched.
    int pid = 0;

    /// @brief Output folder where results will be stored.
    std::string output_folder;
};

#endif //PROCESS_MONITOR_ARGS_H
