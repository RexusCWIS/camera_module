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

std::string getFileExtension(std::string &filename) {
    
    size_t index = filename.rfind('.', filename.length()); 
        
    /* If rfind does not find the pattern, it returns npos */
    if(index != std::string::npos) {
        return (filename.substr(index + 1, filename.length() - index)); 
    }

    return ""; 
}

