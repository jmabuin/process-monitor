/**
 * @file StringUtils.cpp
 * @brief Implementation file for the StringUtils class.
 * @author Jose M. Abuin
 */

#include "StringUtils.h"

std::string StringUtils::ReplaceAll(std::string in, std::string_view from, std::string_view to) {
    std::string tmp = in;

    if ( from.empty())
    {
        return in;
    }

    size_t start_pos = 0;

    // tmp.find() fails to match on "\0"
    while (( start_pos = tmp.find( from, start_pos )) != std::string::npos )
    {
        tmp.replace( start_pos, from.length(), to );
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }

    return tmp;
}
