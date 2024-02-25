//
// Created by chema on 2/21/24.
//

#include "StringUtils.h"

std::string StringUtils::ReplaceAll(std::string in, const std::string& from, const std::string& to) {
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
