/**
 * @file CpuMeasure.h
 * @brief Header file for a single Memory measure.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_MEMORYMEASURE_H
#define PROCESS_MONITOR_MEMORYMEASURE_H

/**
 * @brief Struct representing a single measure of the memory.
 */
struct MemoryMeasure {
    /// @brief Time of the measure in seconds since start.
    double time_seconds;

    /// @brief Amount of memory of this measure. In the units provided in the configuration.
    double quantity;
};

#endif //PROCESS_MONITOR_MEMORYMEASURE_H
