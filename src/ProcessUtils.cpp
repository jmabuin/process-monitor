/**
 * @file ProcessUtils.cpp
 * @brief Implementation file for the ProcessUtils class.
 * @author Jose M. Abuin
 */

#include <iostream>
#include <fstream>
#include "ProcessUtils.h"
#include "pfs/procfs.hpp"

pid_t ProcessUtils::FindChildrenDirect(pid_t parent, std::string_view cmdline) {

    auto task = pfs::procfs().get_task(parent);

    auto pfs = pfs::procfs();
    for (const auto& process : pfs.get_processes()) {
        if ((process.get_stat().ppid == parent) && (CmdToString(process.get_cmdline()) == cmdline)) {
            return process.id();
        }

    }

    return -1;
}

std::string ProcessUtils::CmdToString(const std::vector<std::string>& items) {
    std::string result;

    for (auto const& item : items) {
        result += item + " ";
    }

    result.erase(result.find_last_not_of(' ')+1);
    return result;
}
