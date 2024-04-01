//
// Created by chema on 2/21/24.
//

#ifndef PROCESS_MONITOR_STRINGUTILS_H
#define PROCESS_MONITOR_STRINGUTILS_H


#include <string>

/// @brief Class with auxiliary functions.
class StringUtils {
public:
    /**
     * @brief Function to replace all instances of a substring in a given string.
     * @param in The input string.
     * @param from The substring to be replaced.
     * @param to The substring replacing the old one.
     * @return The resulting string.
     */
    static std::string ReplaceAll( std::string in, std::string_view from, std::string_view to );
};


#endif //PROCESS_MONITOR_STRINGUTILS_H
