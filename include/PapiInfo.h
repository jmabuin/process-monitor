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

class PapiInfo {
public:

    const std::string class_name = "PapiInfo";
    int Pid;
    bool debug_mode = false;
    std::string *output_folder;
    Config *configuration;

    std::thread running_thread;

    std::unordered_map<std::string, std::vector<PapiMeasure>> results;

    explicit PapiInfo(int pid, Config *config, std::string *output_folder, bool debug_mode);
    static void print_device_info();
    void run_thread();
    void run();
    void write_results_to_file();
};


#endif //PROCESS_MONITOR_PAPIINFO_H
