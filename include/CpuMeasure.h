/**
 * @file CpuMeasure.h
 * @brief Header file for a single CPU measure.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_CPUMEASURE_H
#define PROCESS_MONITOR_CPUMEASURE_H

/**
 * @brief Struct representing a single measure of the CPU.
 */
struct CpuMeasure {
    /// @brief Time of the measure in seconds since start.
    double time_seconds;

    /// @brief CPU percentage of this measure.
    double percentage;
};

#endif //PROCESS_MONITOR_CPUMEASURE_H
