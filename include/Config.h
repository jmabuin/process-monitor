/**
 * @file Config.h
 * @brief Header file to handle configuration options.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_CONFIG_H
#define PROCESS_MONITOR_CONFIG_H

#include <string>
#include <vector>

/**
 * @brief Class representing a configuration to run the application.
 */
class Config {

public:

    /// @brief Interval used to takes measures (in seconds).
    unsigned int measure_interval = 5;

    /// @brief Indicates whether to measure CPU percentage or not.
    bool measure_cpu = true;

    /// @brief Indicates whether to measure memory usage or not.
    bool measure_memory = true;

    /// @brief units to store memory results.
    std::string measure_memory_units = "MB";

    /// @brief Indicates whether to measure IO or not.
    bool measure_io = true;

    /// @brief Indicates if the IO measure is accumulative or not.
    bool accumulate_io = false;

    /// @brief Indicates whether to measure PAPI counters or not.
    bool measure_papi = false;

    /// @brief Indicates if the PAPI measure is accumulative or not.
    bool accumulate_papi = false;

    /// @brief Vector of strings containing the PAPI events to be measured.
    std::vector<std::string> papi_events;

    /// @brief Indicates whether to measure energy using RAPL or not.
    bool measure_energy = false;

    /// @brief Indicates if the energy measure is accumulative or not.
    bool accumulate_energy = false;

    /// @brief Command to run. If no PID is provided., this command is run as a different process, and this is the process being watched.
    std::string command;

    /**
     * @brief Function to create a configuration from a JSON file.
     * @param config_json_file Path to the JSON file containing the configuration.
     * @return The configuration instance created from the provided JSON file.
     */
    static Config from_json_file(const std::string& config_json_file);

    /**
     * Function to print out the configuration.
     */
    void print_config() const;
};

#endif //PROCESS_MONITOR_CONFIG_H
