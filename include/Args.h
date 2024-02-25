//
// Created by chema on 2/15/24.
//

#ifndef PROCESS_MONITOR_ARGS_H
#define PROCESS_MONITOR_ARGS_H

#include <string>

struct Args {
    std::string ConfigFile;
    bool DebugMode = false;
    int Pid = 0;
    std::string OutputFolder;
};

#endif //PROCESS_MONITOR_ARGS_H
