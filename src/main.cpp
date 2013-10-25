/**
 * @file main.c
 * @author Olivier Desenfans
 * @brief CWIS camera acquisition software. 
 */

#include <iostream>
#include "unistd.h"
using namespace std; 

int main(int argc, char *argv[]) {

    int opt = 0;
    char *outputFile = NULL;

    /* Command-line arguments parsing */
    while( (opt = getopt(argc, argv, "o:")) != -1) {

        switch (opt) {
            /* Output file specification */
            case 'o':
                outputFile = optarg; 
                break; 

            /* Missing argument to an option */
            case ':':
                return 1;
                break; 

            /* Unknown argument, not specified in the optstring */
            case '?':
                break;
        }
    }

    return 0; 
}

