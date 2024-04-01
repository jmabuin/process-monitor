//
// Created by chema on 2/19/24.
//

#ifndef PROCESS_MONITOR_ENERGYMEASURE_H
#define PROCESS_MONITOR_ENERGYMEASURE_H

/**
 * @brief Struct representing a single energy measure.
 */
struct EnergyMeasure {
    /// @brief Time of the measure in seconds since start.
    double time_seconds;

    /// @brief Amount of energy of this measure, typically in Jules.
    double quantity;
};

#endif //PROCESS_MONITOR_ENERGYMEASURE_H
