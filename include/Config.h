//
// Created by Jose M. Abuin on 2/15/24.
//

#ifndef PROCESS_MONITOR_CONFIG_H
#define PROCESS_MONITOR_CONFIG_H

#include <string>
#include <vector>

class Config {

public:
    bool MeasureCpu = true;
    unsigned int MeasureInterval = 5;
    bool MeasureMemory = true;

    std::string MeasureMemoryUnits = "MB";

    bool MeasureIo = true;
    bool AccumulateIo = false;

    bool MeasurePapi = false;
    bool AccumulatePapi = false;
    std::vector<std::string> PapiEvents;

    bool MeasureEnergy = false;
    bool AccumulateEnergy = false;

    std::string Command;

    static Config from_json_file(const std::string& config_json_file);
    void print_config() const;
};

#endif //PROCESS_MONITOR_CONFIG_H
