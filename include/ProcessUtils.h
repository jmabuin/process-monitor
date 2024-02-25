//
// Created by chema on 2/20/24.
//

#ifndef PROCESS_MONITOR_PROCESSUTILS_H
#define PROCESS_MONITOR_PROCESSUTILS_H


#include <string>

class ProcessUtils {

public:
    static pid_t FindChildren(pid_t parent, const std::string& cmdline);
    static pid_t FindChildrenDirect(pid_t parent, const std::string& cmdline);
};


#endif //PROCESS_MONITOR_PROCESSUTILS_H
