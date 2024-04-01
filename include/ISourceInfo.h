//
// Created by chema on 3/29/24.
//

#ifndef PROCESS_MONITOR_ISOURCEINFO_H
#define PROCESS_MONITOR_ISOURCEINFO_H

class ISourceInfo {

public:
    std::thread running_thread;

    virtual ~ISourceInfo() = default;
    virtual void run_thread() = 0;
    virtual void run() = 0;
};


#endif //PROCESS_MONITOR_ISOURCEINFO_H
