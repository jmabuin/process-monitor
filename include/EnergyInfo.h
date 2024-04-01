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

constexpr size_t MAX_RAPL_EVENTS = 64;

class EnergyInfo : public ISourceInfo {
public:

    const std::string class_name = "EnergyInfo";
    int Pid;
    bool debug_mode = false;
    std::string *output_folder;
    Config *configuration;

    std::unordered_map<std::string, std::vector<EnergyMeasure>> results;

    explicit EnergyInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);
    void run_thread() final;
    void run() final;
    void write_results_to_file(const std::vector<std::string>& event_names_vector);
};


#endif //PROCESS_MONITOR_ENERGYINFO_H
