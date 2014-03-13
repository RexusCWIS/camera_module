/**
 * @file main.cpp 
 * @brief Basic pipe communication test. 
 */

#include "pipes/rx_pipe.hpp"
#include "pipes/tx_pipe.hpp"

#include <string>
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {
    
    TXPipe tx("/tmp/camera_pipe.p"); 

    sleep(5); 
    tx.send("G", 1);

    sleep(140);

    tx.send("S", 1);  
}

