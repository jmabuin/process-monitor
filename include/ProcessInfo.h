//
// Created by chema on 2/15/24.
//

#ifndef PROCESS_MONITOR_PROCESSINFO_H
#define PROCESS_MONITOR_PROCESSINFO_H


#ifdef __cplusplus
extern "C"
{
#endif
#include <third-party/procps/readproc.h>
#ifdef __cplusplus
}
#endif

#include <vector>
#include <thread>
#include "CpuMeasure.h"
#include "MemoryMeasure.h"
#include "IoMeasure.h"
#include "Config.h"
#include "third-party/procps/readproc.h"

class ProcessInfo {

public:

    const std::string class_name = "ProcessInfo";
    int Pid;
    bool debug_mode = false;
    std::string *output_folder;
    Config *configuration;
    std::vector<CpuMeasure> cpu_measures;
    std::vector<MemoryMeasure> memory_measures;
    std::vector<IoMeasure> num_io_read_operations;
    std::vector<IoMeasure> num_io_write_operations;

    std::thread running_thread;

    explicit ProcessInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);
    void run_thread();
    void run();
    void write_results_to_file();

};


#endif //PROCESS_MONITOR_PROCESSINFO_H
