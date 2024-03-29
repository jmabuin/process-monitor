//
// Created by chema on 2/20/24.
//

#ifndef PROCESS_MONITOR_PROCESSUTILS_H
#define PROCESS_MONITOR_PROCESSUTILS_H


#include <string>
#include <vector>

class ProcessUtils {

public:
    static pid_t FindChildrenDirect(pid_t parent, std::string_view cmdline);
    static std::string CmdToString(const std::vector<std::string>& items);
};


#endif //PROCESS_MONITOR_PROCESSUTILS_H
