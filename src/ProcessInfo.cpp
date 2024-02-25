//
// Created by chema on 2/15/24.
//

#include "ProcessInfo.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <sys/sysinfo.h>
#ifdef __cplusplus
}
#endif

#include <iostream>
#include <fstream>


long get_uptime() {
    struct sysinfo s_info{};
    int error = sysinfo(&s_info);
    if (error != 0) {
        printf("code error = %d\n", error);
    }
    return s_info.uptime;
}

ProcessInfo::ProcessInfo(int pid, Config *config, std::string *output_folder, bool debug_mode) {
    this->Pid = pid;
    this->configuration = config;
    this->output_folder = output_folder;
    this->debug_mode = debug_mode;
}

void ProcessInfo::run_thread() {
    this->running_thread = std::thread(&ProcessInfo::run, this);
}

void ProcessInfo::run() {

    bool stop_run = false;

    //Variables to iterate over the processes in this node
    PROCTAB *proc_tab;

    //Variables to calculate pcpu -> % CPU
    unsigned long long seconds;
    unsigned pcpu;
    unsigned long long used_jiffies;

    unsigned long old_cpu = 0, new_cpu, seconds_since_boot;
    long Hertz = procps_hertz_get();
    double current_cpu;


    std::cout << "[" << this->class_name << "] Started reading processes table. Looking for PID: " << this->Pid << std::endl;
    while (!stop_run) {
        seconds_since_boot = get_uptime();

        proc_tab = openproc(
                PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLENV | PROC_FILLARG);

        auto *proc_info = (proc_t *) calloc(1, sizeof(proc_t));

        bool found = false;

        while ((proc_info = readproc(proc_tab, proc_info)) != nullptr) {
            if (proc_info->tid == this->Pid) {
                pcpu = 0;
                used_jiffies = proc_info->utime + proc_info->stime;
                seconds = seconds_since_boot - proc_info->start_time / Hertz;

                if (this->debug_mode) {
                    std::cout << "[" << this->class_name << "] Jiffies: " << used_jiffies << ". Seconds: " << seconds << ". Seconds since boot: "
                              << seconds_since_boot << ". Hertz: " << Hertz << std::endl;
                }

                if (seconds) {
                    pcpu = (used_jiffies * 1000ULL / Hertz) / seconds;
                }

                if (old_cpu == 0) {
                    current_cpu = pcpu;
                    old_cpu = used_jiffies * 1000ULL / Hertz;
                } else {
                    new_cpu = used_jiffies * 1000ULL / Hertz;
                    double cpu2 = (double) ((double) new_cpu / 10U) + (double) (new_cpu % 10U) / 10.0;
                    double cpu1 = (double) ((double) old_cpu / 10U) + (double) (old_cpu % 10U) / 10.0;

                    current_cpu = (cpu2 - cpu1) / this->configuration->MeasureInterval;
                    old_cpu = new_cpu;
                }

                if (this->debug_mode) {
                    std::cout << "[" << this->class_name << "] PID: " << proc_info->tid << ", command: " << proc_info->cmd << ", Memory: "
                              << proc_info->vm_rss << " Kb. Start time: " << proc_info->start_time << " s. CPU:"
                              << current_cpu << "%" << std::endl;
                }

                this->cpu_measures.push_back(CpuMeasure{seconds, current_cpu});
                this->memory_measures.push_back(MemoryMeasure{seconds, proc_info->vm_rss});

                found = true;
            }
        }

        free(proc_info);
        closeproc(proc_tab);

        if (!found) {
            stop_run = true;
        } else {
            sleep(this->configuration->MeasureInterval);
        }
    }

    if (!this->cpu_measures.empty() || !this->memory_measures.empty()) {
        this->write_results_to_file();
    }
    else {
        std::cout << "[" << this->class_name << "] No values found for given PID " <<  this->Pid << std::endl;
    }

}

void ProcessInfo::write_results_to_file() {

    std::string cpu_file_name = std::to_string(this->Pid) + "_cpu.csv";
    std::string memory_file_name = std::to_string(this->Pid) + "_memory.csv";

    if (this->debug_mode) {
        std::cout << "[" << ProcessInfo::class_name << "] Writing Process Results at: " << *this->output_folder << std::endl;
        std::cout << "[" << ProcessInfo::class_name << "] CPU file: " << *this->output_folder + "/" + cpu_file_name << std::endl;
        std::cout << "[" << ProcessInfo::class_name << "] Memory file: " << *this->output_folder + "/" + memory_file_name << std::endl;
    }

    // Write CPU measures
    std::ofstream cpu_file;
    cpu_file.open(*this->output_folder + "/" + cpu_file_name);

    for(const CpuMeasure& current_cpu_measure : this->cpu_measures) {
        cpu_file << current_cpu_measure.time_seconds << ";" << current_cpu_measure.percentage << std::endl;
    }

    cpu_file.close();

    // Write Memory measures
    std::ofstream memory_file;
    memory_file.open(*this->output_folder + "/" + memory_file_name);

    for(const MemoryMeasure& current_memory_measure : this->memory_measures) {
        memory_file << current_memory_measure.time_seconds << ";" << current_memory_measure.quantity << std::endl;
    }

    memory_file.close();
}
