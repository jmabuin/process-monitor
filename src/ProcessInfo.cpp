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
#include <csignal>
#include "pfs/task.hpp"
#include "pfs/procfs.hpp"


const std::string ProcessInfo::MemoryUnitsB = "B";
const std::string ProcessInfo::MemoryUnitsKB = "KB";
const std::string ProcessInfo::MemoryUnitsMB = "MB";
const std::string ProcessInfo::MemoryUnitsGB = "GB";
const std::string ProcessInfo::MemoryUnitsTB = "TB";

long get_uptime() {
    struct sysinfo s_info{};
    if (int error = sysinfo(&s_info); error != 0) {
        printf("code error = %d\n", error);
    }
    return s_info.uptime;
}

ProcessInfo::ProcessInfo(int pid, Config *config, std::string *output_folder, bool debug_mode) : Pid(pid),
                                                                                                 debug_mode(debug_mode),
                                                                                                 output_folder(output_folder),
                                                                                                 configuration(config) {
}

void ProcessInfo::run_thread() {
    this->running_thread = std::thread(&ProcessInfo::run, this);
}

void ProcessInfo::run() {

    if (kill(this->Pid,0) != 0) {
        std::cerr << "No process with PID " << this->Pid << std::endl;
        return;
    }

    auto task = pfs::procfs().get_task(this->Pid);

    // Variables to calculate CPU
    auto tic = sysconf (_SC_CLK_TCK);
    unsigned long old_time = 0;
    unsigned long long seconds = 0;

    // Variables to calculate IO reads and writes
    unsigned long old_reads = 0;
    unsigned long old_writes = 0;

    while(kill(this->Pid,0) == 0) {
        // CPU and Mem
        auto stats = task.get_stat();
        auto stats_mem = task.get_statm();

        auto new_time = stats.utime + stats.stime;

        if (old_time != 0) {
            auto used_time_seconds = double(new_time - old_time) / double(tic);
            auto process_cpu_percentage = used_time_seconds * 100.0 / this->configuration->measure_interval;

            if (this->debug_mode) {
                std::cout << "[" << this->class_name << "] PID: " << this->Pid << ", command: " << task.get_comm() << ", Memory: "
                          << get_mem_for_units(stats_mem.resident) << this->configuration->measure_memory_units << ". CPU: " << process_cpu_percentage << "%." << std::endl;
            }

            this->cpu_measures.push_back(CpuMeasure{seconds, process_cpu_percentage});

        }

        double m = get_mem_for_units(stats_mem.resident);
        this->memory_measures.push_back(MemoryMeasure{seconds, m});

        old_time = new_time;

        // IO
        if (this->configuration->measure_io) {
            auto stats_io = task.get_io();
            auto current_reads = stats_io.syscr;
            auto current_writes = stats_io.syscw;

            auto io_read_measure = IoMeasure{seconds, current_reads};
            auto io_write_measure = IoMeasure{seconds, current_writes};

            if (!this->configuration->accumulate_io) {
                auto num_reads = current_reads - old_reads;
                auto num_writes = current_writes - old_writes;

                io_read_measure.quantity = num_reads;
                io_write_measure.quantity = num_writes;

                old_reads = current_reads;
                old_writes = current_writes;
            }

            this->num_io_read_operations.push_back(io_read_measure);
            this->num_io_write_operations.push_back(io_write_measure);

        }

        seconds += this->configuration->measure_interval;
        sleep(this->configuration->measure_interval);

    }

    if (!this->cpu_measures.empty() || !this->memory_measures.empty()) {
        this->write_results_to_file();
    }
    else {
        std::cout << "[" << this->class_name << "] No values found for given PID " <<  this->Pid << std::endl;
    }

}

void ProcessInfo::write_results_to_file() const {

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

    // Write IO measures if needed
    std::string io_reads_file_name = std::to_string(this->Pid) + "_io_reads.csv";
    std::string io_writes_file_name = std::to_string(this->Pid) + "_io_writes.csv";

    if (!this->num_io_read_operations.empty()) {
        std::ofstream io_reads_file;
        io_reads_file.open(*this->output_folder + "/" + io_reads_file_name);

        for(const IoMeasure& current_io_read_measure : this->num_io_read_operations) {
            io_reads_file << current_io_read_measure.time_seconds << ";" << current_io_read_measure.quantity << std::endl;
        }

        io_reads_file.close();
    }

    if (!this->num_io_write_operations.empty()) {
        std::ofstream io_writes_file;
        io_writes_file.open(*this->output_folder + "/" + io_writes_file_name);

        for(const IoMeasure& current_io_write_measure : this->num_io_write_operations) {
            io_writes_file << current_io_write_measure.time_seconds << ";" << current_io_write_measure.quantity << std::endl;
        }

        io_writes_file.close();
    }
}

double ProcessInfo::get_mem_for_units(size_t num) const {

    auto page_size = sysconf(_SC_PAGE_SIZE);

    auto in_bytes = double(num * page_size);
    if (this->configuration->measure_memory_units == MemoryUnitsB) {
        return in_bytes;
    }
    else if (this->configuration->measure_memory_units == MemoryUnitsKB) {
        return in_bytes / 1024.0;
    }
    else if (this->configuration->measure_memory_units == MemoryUnitsMB) {
        return in_bytes / 1024.0 / 1024.0;
    }
    else if (this->configuration->measure_memory_units == MemoryUnitsGB) {
        return in_bytes / 1024.0 / 1024.0 / 1024.0;
    }
    else if (this->configuration->measure_memory_units == MemoryUnitsTB) {
        return in_bytes / 1024.0 / 1024.0 / 1024.0 / 1024.0;
    }

    return 0.0;
}
