//
// Created by chema on 3/29/24.
//

#ifndef PROCESS_MONITOR_RUNNER_H
#define PROCESS_MONITOR_RUNNER_H


#include "Args.h"
#include "ProcessInfo.h"
#include "PapiInfo.h"
#include "EnergyInfo.h"

class Runner {
public:
    Args input_args;

    std::unique_ptr<ProcessInfo> process_info;
    std::unique_ptr<PapiInfo> papi_info;
    std::unique_ptr<EnergyInfo> energy_info;

    explicit Runner(Args input_args);

    void PreRun() const;
    int RunForPid();
    int RunForCommandConfig();
    int RunForPidAndConfig();
    int Run();

private:
    void RunThreads(Config *configuration);
    void WaitForThreads() const;

    void CreateOutputFolder();
};


#endif //PROCESS_MONITOR_RUNNER_H
