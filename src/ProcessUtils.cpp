//
// Created by chema on 2/20/24.
//

#ifdef __cplusplus
extern "C"
{
#endif
#include <third-party/procps/readproc.h>
#ifdef __cplusplus
}
#endif

#include <iostream>
#include <fstream>
#include "ProcessUtils.h"

pid_t ProcessUtils::FindChildren(pid_t parent, const std::string &cmdline) {

    PROCTAB *proc_tab;
    proc_tab = openproc(
            PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLENV | PROC_FILLARG);

    auto *proc_info = (proc_t *) calloc(1, sizeof(proc_t));
    std::string processNameStr;

    while ((proc_info = readproc(proc_tab, proc_info)) != nullptr) {
        if (proc_info->ppid == parent) {
            if (proc_info->cmdline == nullptr) {
                std::cout << "Cmdline is null!!!!!!!!!!!!!!1" << std::endl;
            }
            std::cout << "Found parent " << parent  << " Cmdline:" << *proc_info->cmdline << std::endl;
            if (proc_info->cmdline) {
                std::cout << "Found cmdline " << parent << std::endl;
                processNameStr.clear();

                char *tmpCmd = reinterpret_cast<char *>(proc_info->cmdline[0]);

                int j = 0;

                while (tmpCmd) {

                    if (j == 0) {
                        processNameStr = std::string(reinterpret_cast<const char *>(proc_info->cmdline[j])) + " ";

                    } else {
                        processNameStr += std::string(reinterpret_cast<const char *>(proc_info->cmdline[j])) + " ";

                    }

                    j++;
                    tmpCmd = reinterpret_cast<char *>(proc_info->cmdline[j]);
                }
            } else {
                std::cout << "NOT Found cmdline " << parent << std::endl;
                processNameStr = std::string(proc_info->cmd);
            }

            std::cout << "Process name is " << processNameStr << std::endl;
            if (processNameStr.find(cmdline) == 0) {
                return proc_info->tid;
            }
        }
    }
    return 0;
}

pid_t ProcessUtils::FindChildrenDirect(pid_t parent, const std::string &cmdline) {
    PROCTAB *proc_tab;
    proc_tab = openproc(
            PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR | PROC_FILLENV | PROC_FILLARG);

    auto *proc_info = (proc_t *) calloc(1, sizeof(proc_t));
    std::string processNameStr;

    while ((proc_info = readproc(proc_tab, proc_info)) != nullptr) {

        if (proc_info->ppid == parent) {
            //Open cmdline file
            std::string cmdline_file = "/proc/" + std::to_string(proc_info->tid) + "/cmdline";
            std::string line;
            std::ifstream f(cmdline_file.c_str());

            if (f.good()) {
                std::string content;

                while ( getline (f,line) )
                {
                    content.append(line);
                }

                std::string alternative_start = "sh -c ";

                if (content.find(cmdline) == 0) {
                    f.close();
                    return proc_info->tid;
                }
                /*else { // Use this else only if using Popen to create the process
                    // popen creates the process starting with sh -c, and contains nulls instead of spaces
                    auto alternative = StringUtils::ReplaceAll(std::string(content), {'\0'}, " ");

                    if (alternative.find(alternative_start + cmdline) == 0) {
                        f.close();
                        //return proc_info->tid;
                        return ProcessUtils::FindChildrenDirect(proc_info->tid, cmdline);
                    }
                }*/

                f.close();
            } else {
                std::cout << "BAD================================" << std::endl;
            }
        }
    }
    return 0;
}
