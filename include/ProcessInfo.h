/**
 * @file PapiInfo.h
 * @brief Header file for the Process information.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_PROCESSINFO_H
#define PROCESS_MONITOR_PROCESSINFO_H

#include <vector>
#include <thread>
#include "CpuMeasure.h"
#include "MemoryMeasure.h"
#include "IoMeasure.h"
#include "Config.h"
#include "ISourceInfo.h"

/**
 * @brief Class containing all measures and configuration about Process measures (CPU, Memory and IO).
 */
class ProcessInfo : public ISourceInfo {

public:

    /// @brief This class name. Used for logging.
    const std::string class_name = "ProcessInfo";

    /// @brief PID of the process being monitored.
    int pid;

    /// @brief Indicates whether we show debug info or not.
    bool debug_mode = false;

    /// @brief Output folder where to store the output files.
    std::string *output_folder;

    /// @brief The given configuration.
    Config *configuration;

    /// @brief Vector containing all CPU percentage measures.
    std::vector<CpuMeasure> cpu_measures;

    /// @brief Vector containing all Memory measures.
    std::vector<MemoryMeasure> memory_measures;

    /// @brief Vector containing all IO read measures.
    std::vector<IoMeasure> num_io_read_operations;

    /// @brief Vector containing all IO write measures.
    std::vector<IoMeasure> num_io_write_operations;

    /**
     * @brief Builder for class ProcessInfo.
     * @param pid PID of process being monitored.
     * @param config The given configuration.
     * @param output_folder Output folder where to store the output files.
     * @param debug_mode Boolean variable indicating whether we show debug info or not.
     */
    explicit ProcessInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);

    /// @brief Function to run the measurement code in a different thread.
    void run_thread() final;

    /// @brief Function with the real code to perform measures from a different thread.
    void run() final;

    /// @brief Writes results to output files.
    void write_results_to_file() const;

    /**
     * @brief Transforms the given value according to the memory units specified in the configuration.
     * @param num The amount of memory in bytes.
     * @return The amount of memory in the units provided in the configuration.
     */
    [[nodiscard]] double get_mem_for_units(size_t num) const;

    /// @brief Memory units for bytes.
    static const std::string MemoryUnitsB;
    /// @brief Memory units for Kilobytes.
    static const std::string MemoryUnitsKB;
    /// @brief Memory units for Megabytes.
    static const std::string MemoryUnitsMB;
    /// @brief Memory units for Gigabytes.
    static const std::string MemoryUnitsGB;
    /// @brief Memory units for Terabytes.
    static const std::string MemoryUnitsTB;
};


#endif //PROCESS_MONITOR_PROCESSINFO_H
