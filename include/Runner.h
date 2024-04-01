//
// Created by chema on 3/29/24.
//

#ifndef PROCESS_MONITOR_RUNNER_H
#define PROCESS_MONITOR_RUNNER_H


#include <memory>
#include "Args.h"
#include "ProcessInfo.h"
#include "PapiInfo.h"
#include "EnergyInfo.h"

/// @brief Class that runs the main code of the application.
class Runner {
public:

    /// @brief Input args for the application.
    Args input_args;

    /// @brief Vector of threads which run to gather the required information.
    std::vector<std::unique_ptr<ISourceInfo>> info_threads;

    /**
     * @brief Builder for runner class.
     * @param input_args The input arguments.
     */
    explicit Runner(Args input_args);

    /// @brief Function running at the beginning of the execution. It only shows some information.
    void PreRun() const;

    /// @brief RUns for a given PID with default configuration.
    int RunForPid();

    /// @brief Runs for given configuration and with command provided in configuration.
    int RunForCommandConfig();

    /// @brief Runs for given configuration for provided PID. Ignores command provided in configuration.
    int RunForPidAndConfig();

    /// @brief Main run function.
    int Run();

private:
    /**
     * @brief STart the information threads from the given configuration.
     * @param configuration The given configuration.
     */
    void RunThreads(Config *configuration);

    /// @brief Wait for all the information threads to finish.
    void WaitForThreads() const;

    void CreateOutputFolder();
};


#endif //PROCESS_MONITOR_RUNNER_H
