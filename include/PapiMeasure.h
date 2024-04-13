/**
 * @file PapiMeasure.h
 * @brief Header file for a single PAPI measure.
 * @author Jose M. Abuin
 */

#ifndef PROCESS_MONITOR_PAPIMEASURE_H
#define PROCESS_MONITOR_PAPIMEASURE_H

/**
 * @brief Struct representing a single Papi measure.
 */
struct PapiMeasure {
    /// @brief Time of the measure in seconds since start.
    unsigned long long time_seconds;

    /// @brief Quantity for this Papi event measure..
    long long int quantity;
};

#endif //PROCESS_MONITOR_PAPIMEASURE_H
