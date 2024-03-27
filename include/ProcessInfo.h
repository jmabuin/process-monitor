//
// Created by chema on 2/15/24.
//

#ifndef PROCESS_MONITOR_PROCESSINFO_H
#define PROCESS_MONITOR_PROCESSINFO_H

#include <vector>
#include <thread>
#include "CpuMeasure.h"
#include "MemoryMeasure.h"
#include "IoMeasure.h"
#include "Config.h"

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
    void write_results_to_file() const;
    [[nodiscard]] double get_mem_for_units(size_t num) const;

    static const std::string MemoryUnitsB;
    static const std::string MemoryUnitsKB;
    static const std::string MemoryUnitsMB;
    static const std::string MemoryUnitsGB;
    static const std::string MemoryUnitsTB;
};


#endif //PROCESS_MONITOR_PROCESSINFO_H
