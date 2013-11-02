/**
 * @file utilities.cpp
 * @brief Helper functions implementation. 
 */

#include "utilities.hpp"
#include <stdio.h>

void string_appendInt(std::string &str, int x) {

    char c_string[21]; 
    sprintf(c_string, "%d", x); 
    
    str += c_string;

    return; 
}

