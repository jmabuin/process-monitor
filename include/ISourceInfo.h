//
// Created by chema on 3/29/24.
//

#ifndef PROCESS_MONITOR_ISOURCEINFO_H
#define PROCESS_MONITOR_ISOURCEINFO_H

/// @brief Abstract class for different sources of measures.
class ISourceInfo {

public:
    /// @brief A different thread to run the code where measures are being taken.
    std::thread running_thread;

    /// @brief Default virtual builder.
    virtual ~ISourceInfo() = default;

    /// @brief Function to run the measurement code in a different thread.
    virtual void run_thread() = 0;

    /// @brief Function with the real code to perform measures from a different thread.
    virtual void run() = 0;
};


#endif //PROCESS_MONITOR_ISOURCEINFO_H
