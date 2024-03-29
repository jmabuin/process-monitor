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
        new_config.measure_interval = data["interval"];
    }
    if (data.contains("cpu")){
        new_config.measure_cpu = data["cpu"]["measure"];
    }

    if (data.contains("memory")){
        new_config.measure_memory = data["memory"]["measure"];
        new_config.measure_memory_units = data["memory"]["units"];
    }

    if (data.contains("io")){
        new_config.measure_io = data["io"]["measure"];
        new_config.accumulate_io = data["io"]["accumulate"];
    }

    if (data.contains("papi")) {
        new_config.measure_papi = data["papi"]["measure"];
        new_config.accumulate_papi = data["papi"]["accumulate"];
        for(const auto& current_event : data["papi"]["events"]) {
            new_config.papi_events.push_back(current_event);
        }
    }

    if (data.contains("energy")) {
        new_config.measure_energy = data["energy"]["measure"];
        new_config.accumulate_energy = data["energy"]["accumulate"];
    }

    if (data.contains("command")) {
        new_config.command = data["command"];
    }

    return new_config;
}

void Config::print_config() const {

    std::cout<< "====Configuration====" << std::endl;

    std::cout<< "  ====CPU====" << std::endl;
    std::cout << "    Measure: " << this->measure_cpu << std::endl;
    std::cout << "    Interval (s): " << this->measure_interval << std::endl;

    std::cout<< "  ====Memory====" << std::endl;
    std::cout << "    Measure: " << this->measure_memory << std::endl;
    std::cout << "    Interval (s): " << this->measure_interval << std::endl;
    std::cout << "    Units: " << this->measure_memory_units << std::endl;

    std::cout<< "  ====PAPI====" << std::endl;
    std::cout << "    Measure: " << this->measure_papi << std::endl;
    std::cout<< "    Events: " << std::endl;
    std::cout << "    Accumulate: " << this->accumulate_papi << std::endl;
    for(const auto& current_event : this->papi_events) {
        std::cout << "      " << current_event << std::endl;
    }

    std::cout<< "  ====Energy====" << std::endl;
    std::cout << "    Measure: " << this->measure_energy << std::endl;
    std::cout << "    Accumulate: " << this->accumulate_energy << std::endl;

    if (!this->command.empty()) {
        std::cout << "Command to run is: " << this->command << std::endl;
    }


}
