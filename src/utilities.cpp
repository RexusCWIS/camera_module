/**
 * @file utilities.cpp
 * @brief Helper functions implementation. 
 */

#include "utilities.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void createDirectory(const std::string &dirname) {
    
    errno = 0; 

    if(!mkdir(dirname.c_str(), 0666)) {
        perror(strerror(errno));
        exit(EXIT_FAILURE); 
    }
}

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

