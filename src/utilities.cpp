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

int createDirectory(const std::string &dirname) {
    
    errno = 0; 

    if(mkdir(dirname.c_str(), 0666) != 0) {
        switch(errno) {
            /* If the file exists, check that it is a directory */
            case EEXIST:
                struct stat info; 
                stat(dirname.c_str(), &info);
                return (S_ISDIR(info.st_mode)); 
                break;
            default: 
                perror(strerror(errno));
                return 0;
                break; 
        }
    }

    return 1; 
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

