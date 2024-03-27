#include <iostream>
#include <getopt.h>
#include <Args.h>
#include <Globals.h>
#include "Config.h"
#include "ProcessInfo.h"
#include "PapiInfo.h"
#include "EnergyInfo.h"
#include "ProcessUtils.h"
#include <cstdio>
#include <filesystem>
#include <wait.h>
#include <cstring>


namespace fs = std::filesystem;

static int usage() {
    fprintf(stderr, "\n");
    fprintf(stderr, "Program: ProcessMonitor (Monitor for a given process)\n");
    fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
    fprintf(stderr, "Contact: José M. Abuín <>\n\n");
    fprintf(stderr, "Usage:   ProcessMonitor [options]\n\n");
    fprintf(stderr, "Common options:\n");
    fprintf(stderr, "\n");

    fprintf(stderr, " -d \tDebug mode. Default: False.\n");
    fprintf(stderr, " -h \tPrint this help.\n");
    fprintf(stderr, " -p \tPID of process we are looking for.\n");
    fprintf(stderr, " -c \tJSON configuration file to be used.\n");
    fprintf(stderr, " -o \tOutput folder to store results.\n");

    fprintf(stderr, "\n");

    return 1;
}

int main(int argc, char *argv[]) {

    int option;
    Args input_args;

    std::unique_ptr<ProcessInfo> process_info;
    std::unique_ptr<PapiInfo> papi_info;
    std::unique_ptr<EnergyInfo> energy_info;

    while ((option = getopt(argc, argv, "hdc:p:o:")) >= 0) {
        switch (option) {
            case 'h':
                usage();
                return EXIT_SUCCESS;
            case 'c' :
                input_args.ConfigFile = optarg;
                break;
            case 'd':
                input_args.DebugMode = true;
                break;
            case 'p':
                input_args.Pid = std::stoi(optarg);
                break;
            case 'o':
                input_args.OutputFolder = optarg;
                break;
            default:
                break;
        }
    }

    if (input_args.OutputFolder.empty()) {
        std::cout << "Output folder is empty. Using PID: " << input_args.Pid << std::endl;
        input_args.OutputFolder = std::to_string(input_args.Pid);
    }

    if (!fs::is_directory(input_args.OutputFolder) ||
        !fs::exists(input_args.OutputFolder)) { // Check if src folder exists
        fs::create_directory(input_args.OutputFolder); // create src folder
    }

    if (input_args.DebugMode) {
        std::cout << "PID: " << input_args.Pid << std::endl;
        std::cout << "Config file: " << input_args.ConfigFile << std::endl;
        std::cout << "Output folder: " << input_args.OutputFolder << std::endl;
    }

    PapiInfo::print_device_info();

    // No PID nor config file. Exit
    if (input_args.ConfigFile.empty() && input_args.Pid == 0) {
        std::cerr << "Can not run without PID nor configuration file!!" << std::endl;
        exit(EXIT_FAILURE);
    } else if (input_args.ConfigFile.empty() && input_args.Pid != 0) {
        std::cout << "Attaching to PID " << input_args.Pid << " with default configuration." << std::endl;
        Config config;
        config.print_config();

        if (config.MeasureCpu && config.MeasureMemory) {
            process_info = std::make_unique<ProcessInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                         input_args.DebugMode);
        }

        if (config.MeasurePapi) {
            papi_info = std::make_unique<PapiInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                   input_args.DebugMode);
        }

        if (config.MeasureEnergy) {
            energy_info = std::make_unique<EnergyInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                       input_args.DebugMode);
        }

        if (process_info != nullptr) {
            process_info->run_thread();
        }

        if (papi_info != nullptr) {
            papi_info->run_thread();
        }

        if (energy_info != nullptr) {
            energy_info->run_thread();
        }

        if (process_info != nullptr) {
            process_info->running_thread.join();
        }

        if (papi_info != nullptr) {
            papi_info->running_thread.join();
        }

        if (energy_info != nullptr) {
            energy_info->running_thread.join();
        }
    } else if (input_args.Pid == 0) {
        std::cout << "Configuration file is " << input_args.ConfigFile << " and should contain a command for running"
                  << std::endl;
        auto config = Config::from_json_file(input_args.ConfigFile);
        config.print_config();

        if (config.Command.empty()) {
            std::cerr << " Configuration must contain a command if no PID is provided!!" << std::endl;
            exit(EXIT_FAILURE);
        }
        pid_t new_pid = fork();

        if (new_pid < 0) {
            std::cerr << " Could not create external process!" << std::endl;
            exit(EXIT_FAILURE);
        } else if (new_pid == 0) { // Child process. Run command.
            execl("/bin/sh", "/bin/sh", "-c", config.Command.c_str(), NULL);
        } else {
            std::cout << "Theoretical children PID is: " << new_pid << std::endl;

            sleep(1); // Let's give some time to children to start
            auto subprocess_pid = ProcessUtils::FindChildrenDirect(new_pid, config.Command);

            if (subprocess_pid != -1) {
                std::cout << "Real children PID is: " << subprocess_pid << std::endl;
                input_args.Pid = subprocess_pid;
            } else {
                input_args.Pid = new_pid;
            }

            if (config.MeasureCpu && config.MeasureMemory) {
                process_info = std::make_unique<ProcessInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                             input_args.DebugMode);
            }

            if (config.MeasurePapi) {
                papi_info = std::make_unique<PapiInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                       input_args.DebugMode);
            }

            if (config.MeasureEnergy) {
                energy_info = std::make_unique<EnergyInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                           input_args.DebugMode);
            }

            if (process_info != nullptr) {
                process_info->run_thread();
            }

            if (papi_info != nullptr) {
                papi_info->run_thread();
            }

            if (energy_info != nullptr) {
                energy_info->run_thread();
            }

            int status;
            while (true) {
                if ((waitpid(new_pid, &status, 0)) == -1) {
                    if (WIFEXITED(status)) {
                        std::cout << "Children with PID " << new_pid << " finished." << std::endl;
                        break;
                    }
                }
            }

            if (process_info != nullptr) {
                process_info->running_thread.join();
            }

            if (papi_info != nullptr) {
                papi_info->running_thread.join();
            }

            if (energy_info != nullptr) {
                energy_info->running_thread.join();
            }
        }
    } else {
        std::cout << "Configuration file is " << input_args.ConfigFile << " and PID is " << input_args.Pid << std::endl;
        auto config = Config::from_json_file(input_args.ConfigFile);
        config.print_config();

        if (!config.Command.empty()) {
            std::cout << "PID takes precedence over configured command! " << config.Command << std::endl;
        }

        if (config.MeasureCpu && config.MeasureMemory) {
            process_info = std::make_unique<ProcessInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                         input_args.DebugMode);
        }

        if (config.MeasurePapi) {
            papi_info = std::make_unique<PapiInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                   input_args.DebugMode);
        }

        if (config.MeasureEnergy) {
            energy_info = std::make_unique<EnergyInfo>(input_args.Pid, &config, &input_args.OutputFolder,
                                                       input_args.DebugMode);
        }

        if (process_info != nullptr) {
            process_info->run_thread();
        }

        if (papi_info != nullptr) {
            papi_info->run_thread();
        }

        if (energy_info != nullptr) {
            energy_info->run_thread();
        }

        if (process_info != nullptr) {
            process_info->running_thread.join();
        }

        if (papi_info != nullptr) {
            papi_info->running_thread.join();
        }

        if (energy_info != nullptr) {
            energy_info->running_thread.join();
        }
    }

    return EXIT_SUCCESS;
}
