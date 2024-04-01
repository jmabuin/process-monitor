//
// Created by chema on 2/15/24.
//

#ifndef PROCESS_MONITOR_CPUMEASURE_H
#define PROCESS_MONITOR_CPUMEASURE_H

/**
 * @brief Struct representing a single measure of the CPU.
 */
struct CpuMeasure {
    /// @brief Time of the measure in seconds since start.
    unsigned long long time_seconds;

    /// @brief CPU percentage of this measure.
    double percentage;
};

#endif //PROCESS_MONITOR_CPUMEASURE_H
