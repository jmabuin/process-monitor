//
// Created by chema on 2/21/24.
//

#ifndef PROCESS_MONITOR_STRINGUTILS_H
#define PROCESS_MONITOR_STRINGUTILS_H


#include <string>

class StringUtils {
public:
    static std::string ReplaceAll( std::string in, std::string_view from, std::string_view to );
};


#endif //PROCESS_MONITOR_STRINGUTILS_H
