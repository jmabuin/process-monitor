//
// Created by chema on 2/16/24.
//

#ifndef PROCESS_MONITOR_ENERGYINFO_H
#define PROCESS_MONITOR_ENERGYINFO_H

#include <string>
#include <thread>
#include <unordered_map>
#include "Config.h"
#include "EnergyMeasure.h"
#include "ISourceInfo.h"

/// @brief Max number of RAPL events.
constexpr size_t MAX_RAPL_EVENTS = 64;

/**
 * @brief Class containing all measures and configuration about Energy measures.
 */
class EnergyInfo : public ISourceInfo {
public:

    /// @brief This class name. Used for logging.
    const std::string class_name = "EnergyInfo";

    /// @brief PID of the process being monitored.
    int pid;

    /// @brief Indicates whether we show debug info or not.
    bool debug_mode = false;

    /// @brief Output folder where to store the output files.
    std::string *output_folder;

    /// @brief The given configuration.
    Config *configuration;

    /// @brief Map containing all Energy related measures.
    std::unordered_map<std::string, std::vector<EnergyMeasure>> results;

    /**
     * @brief Builder for class EnergyInfo.
     * @param pid PID of process being monitored.
     * @param config The given configuration.
     * @param output_folder Output folder where to store the output files.
     * @param debug_mode Boolean variable indicating whether we show debug info or not.
     */
    explicit EnergyInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);

    /// @brief Function to run the measurement code in a different thread.
    void run_thread() final;

    /// @brief Function with the real code to perform measures from a different thread.
    void run() final;

    /**
     * @brief Writes results to output files.
     * @param event_names_vector Vector with the names of the events being used to measure energy.
     */
    void write_results_to_file(const std::vector<std::string>& event_names_vector);
};


#endif //PROCESS_MONITOR_ENERGYINFO_H
