//
// Created by chema on 2/16/24.
//

#ifndef PROCESS_MONITOR_PAPIINFO_H
#define PROCESS_MONITOR_PAPIINFO_H

#include <string>
#include <thread>
#include <unordered_map>
#include "Config.h"
#include "PapiMeasure.h"
#include "ISourceInfo.h"

/**
 * @brief Class containing all measures and configuration about Papi measures.
 */
class PapiInfo : public ISourceInfo {
public:

    /// @brief This class name. Used for logging.
    const std::string class_name = "PapiInfo";

    /// @brief PID of the process being monitored.
    int pid;

    /// @brief Indicates whether we show debug info or not.
    bool debug_mode = false;

    /// @brief Output folder where to store the output files.
    std::string *output_folder;

    /// @brief The given configuration.
    Config *configuration;

    /// @brief Map containing all Papi related measures.
    std::unordered_map<std::string, std::vector<PapiMeasure>> results;

    /**
     * @brief Builder for class PapiInfo.
     * @param pid PID of process being monitored.
     * @param config The given configuration.
     * @param output_folder Output folder where to store the output files.
     * @param debug_mode Boolean variable indicating whether we show debug info or not.
     */
    explicit PapiInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);

    /// @brief Prints out the CPU info.
    static void print_device_info();

    /// @brief Function to run the measurement code in a different thread.
    void run_thread() final;

    /// @brief Function with the real code to perform measures from a different thread.
    void run() final;

    /// @brief @brief Writes results to output files.
    void write_results_to_file();
};


#endif //PROCESS_MONITOR_PAPIINFO_H
