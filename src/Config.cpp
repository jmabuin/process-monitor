//
// Created by Jose M. Abuin on 2/15/24.
//


#include <Config.h>
#include <third-party/nlohmann/json.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Config Config::from_json_file(const std::string& config_json_file) {

    Config new_config;

    std::ifstream f(config_json_file);
    json data = json::parse(f);

    if (data.contains("interval")){
        new_config.MeasureInterval = data["interval"];
    }
    if (data.contains("cpu")){
        new_config.MeasureCpu = data["cpu"]["measure"];
    }

    if (data.contains("memory")){
        new_config.MeasureMemory = data["memory"]["measure"];
        new_config.MeasureMemoryUnits = data["memory"]["units"];
    }

    if (data.contains("io")){
        new_config.MeasureIo = data["io"]["measure"];
        new_config.AccumulateIo = data["io"]["accumulate"];
    }

    if (data.contains("papi")) {
        new_config.MeasurePapi = data["papi"]["measure"];
        new_config.AccumulatePapi = data["papi"]["accumulate"];
        for(const auto& current_event : data["papi"]["events"]) {
            new_config.PapiEvents.push_back(current_event);
        }
    }

    if (data.contains("energy")) {
        new_config.MeasureEnergy = data["energy"]["measure"];
        new_config.AccumulateEnergy = data["energy"]["accumulate"];
    }

    if (data.contains("command")) {
        /*for(const auto& current_command_item : data["command"]) {
            new_config.Command.push_back(current_command_item);
        }*/
        new_config.Command = data["command"];
    }

    return new_config;
}

void Config::print_config() const {

    std::cout<< "====Configuration====" << std::endl;

    std::cout<< "  ====CPU====" << std::endl;
    std::cout<< "    Measure: " << this->MeasureCpu << std::endl;
    std::cout<< "    Interval (s): " << this->MeasureInterval << std::endl;

    std::cout<< "  ====Memory====" << std::endl;
    std::cout<< "    Measure: " << this->MeasureMemory << std::endl;
    std::cout<< "    Interval (s): " << this->MeasureInterval << std::endl;
    std::cout<< "    Units: " << this->MeasureMemoryUnits << std::endl;

    std::cout<< "  ====PAPI====" << std::endl;
    std::cout<< "    Measure: " << this->MeasurePapi << std::endl;
    std::cout<< "    Events: " << std::endl;
    std::cout<< "    Accumulate: " << this->AccumulatePapi << std::endl;
    for(const auto& current_event : this->PapiEvents) {
        std::cout << "      " << current_event << std::endl;
    }

    std::cout<< "  ====Energy====" << std::endl;
    std::cout<< "    Measure: " << this->MeasureEnergy << std::endl;
    std::cout<< "    Accumulate: " << this->AccumulateEnergy << std::endl;

    if (!this->Command.empty()) {
        std::cout << "Command to run is: " << this->Command << std::endl;
    }


}
