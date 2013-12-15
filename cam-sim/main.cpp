/**
 * @file main.cpp
 * @brief I2C communication process.
 */

#include <iostream>
#include <cstdlib>

#include "pipes/rx_pipe.hpp"

static void order_processing(char orders[], int size); 

int main(int argc, char *argv[]) {

    RXPipe camera_rx("/tmp/camera_pipe.p", &order_processing, 1); 
    camera_rx.start(); 

    while(true) {

    }
	exit(EXIT_SUCCESS); 
}

static void order_processing(char orders[], int size) {

    switch(orders[size-1]) {
        
        case 'G': 
            std::cout << "The experiment has STARTED." << std::endl; 
            break; 
        case 'S':
            std::cout << "The experiment is OVER." << std::endl; 
            break; 
        default: 
            std::cout << "Unknown order." << std::endl; 
    }
}

