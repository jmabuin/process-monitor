//
// Created by chema on 3/29/24.
//

#include "Runner.h"
#include <iostream>
#include "ProcessUtils.h"
#include <filesystem>
#include <wait.h>
#include <cstring>
#include <utility>

#ifdef __cplusplus
extern "C"
{
#endif
#include <unistd.h>
#ifdef __cplusplus
}
#endif

namespace fs = std::filesystem;

Runner::Runner(Args input_args) : input_args(std::move(input_args)) {

}

void Runner::PreRun() const {
        if (this->input_args.debug_mode) {
        std::cout << "PID: " << this->input_args.pid << std::endl;
        std::cout << "Config file: " << this->input_args.config_file << std::endl;
        std::cout << "Output folder: " << this->input_args.output_folder << std::endl;
    }

    PapiInfo::print_device_info();
}

int Runner::RunForPid() {
    std::cout << "Attaching to PID " << this->input_args.pid << " with default configuration." << std::endl;
    Config config;
    config.print_config();

    this->RunThreads(&config);

    this->WaitForThreads();

    return EXIT_SUCCESS;
}

int Runner::RunForCommandConfig() {
    std::cout << "Configuration file is " << this->input_args.config_file << " and should contain a command for running"
              << std::endl;
    auto config = Config::from_json_file(this->input_args.config_file);
    config.print_config();

    if (config.command.empty()) {
        std::cerr << " Configuration must contain a command if no PID is provided!!" << std::endl;
        return EXIT_FAILURE;
    }

    if (pid_t new_pid = fork(); new_pid < 0) {
        std::cerr << " Could not create external process!" << std::endl;
        return EXIT_FAILURE;
    } else if (new_pid == 0) { // Child process. Run command.
        execl("/bin/sh", "/bin/sh", "-c", config.command.c_str(), NULL);
    } else {
        std::cout << "Theoretical children PID is: " << new_pid << std::endl;

        sleep(1); // Let's give some time to children to start

        if (auto subprocess_pid = ProcessUtils::FindChildrenDirect(new_pid, config.command); subprocess_pid != -1) {
            std::cout << "Real children PID is: " << subprocess_pid << std::endl;
            this->input_args.pid = subprocess_pid;
        } else {
            this->input_args.pid = new_pid;
        }

        this->RunThreads(&config);

        int status;
        while (true) {
            if (((waitpid(new_pid, &status, 0)) == -1) && (WIFEXITED(status))) {
                std::cout << "Children with PID " << new_pid << " finished." << std::endl;
                break;
            }
        }

        this->WaitForThreads();
    }

    return EXIT_SUCCESS;
}

int Runner::RunForPidAndConfig() {
    std::cout << "Configuration file is " << this->input_args.config_file << " and PID is " << this->input_args.pid << std::endl;
    auto config = Config::from_json_file(this->input_args.config_file);
    config.print_config();

    if (!config.command.empty()) {
        std::cout << "PID takes precedence over configured command! " << config.command << std::endl;
    }

    this->RunThreads(&config);
    this->WaitForThreads();

    return EXIT_SUCCESS;
}

int Runner::Run() {

    this->PreRun();

    // No PID nor config file. Exit
    if (this->input_args.config_file.empty() && this->input_args.pid == 0) {
        std::cerr << "Can not run without PID nor configuration file!!" << std::endl;
        return EXIT_FAILURE;
    } else if (this->input_args.config_file.empty() && this->input_args.pid != 0) { // PID only
        return this->RunForPid();
    } else if (this->input_args.pid == 0) { // Config only
        return this->RunForCommandConfig();
    } else { // PID and config
        return this->RunForPidAndConfig();
    }
}

void Runner::RunThreads(Config *configuration) {

    this->CreateOutputFolder();

    if (configuration->measure_cpu && configuration->measure_memory) {
        this->process_info = std::make_unique<ProcessInfo>(this->input_args.pid, configuration, &this->input_args.output_folder,
                                                           this->input_args.debug_mode);
    }

    if (configuration->measure_papi) {
        this->papi_info = std::make_unique<PapiInfo>(this->input_args.pid, configuration, &this->input_args.output_folder,
                                                     this->input_args.debug_mode);
    }

    if (configuration->measure_energy) {
        this->energy_info = std::make_unique<EnergyInfo>(this->input_args.pid, configuration, &this->input_args.output_folder,
                                                         this->input_args.debug_mode);
    }

    if (this->process_info != nullptr) {
        this->process_info->run_thread();
    }

    if (this->papi_info != nullptr) {
        this->papi_info->run_thread();
    }

    if (this->energy_info != nullptr) {
        this->energy_info->run_thread();
    }
}

void Runner::WaitForThreads() const {
    if (this->process_info != nullptr) {
        this->process_info->running_thread.join();
    }

    if (this->papi_info != nullptr) {
        this->papi_info->running_thread.join();
    }

    if (this->energy_info != nullptr) {
        this->energy_info->running_thread.join();
    }
}

void Runner::CreateOutputFolder() {
    if (this->input_args.output_folder.empty()) {
        std::cout << "Output folder is empty. Using PID: " << this->input_args.pid << std::endl;
        this->input_args.output_folder = std::to_string(this->input_args.pid);
    }

    if (!fs::is_directory(this->input_args.output_folder) ||
        !fs::exists(this->input_args.output_folder)) { // Check if src folder exists
        fs::create_directory(this->input_args.output_folder); // create src folder
    }
}






