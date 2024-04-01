//
// Created by chema on 3/3/24.
//

#ifndef PROCESS_MONITOR_IOMEASURE_H
#define PROCESS_MONITOR_IOMEASURE_H

/**
 * @brief Struct representing a single IO measure.
 */
struct IoMeasure {
    /// @brief Time of the measure in seconds since start.
    unsigned long long time_seconds;

    /// @brief Amount of IO operations of this measure.
    unsigned long quantity;
};

#endif //PROCESS_MONITOR_IOMEASURE_H
