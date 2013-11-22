/**
 * @file main.cpp 
 * @brief Basic pipe communication test. 
 */

#include "pipes/rx_pipe.hpp"
#include "pipes/tx_pipe.hpp"

#include <string>
#include <iostream>
#include <unistd.h>

static void rxCallback(char *data, int dataSize) {

    std::cout << data << std::endl; 
}

int main(int argc, char **argv) {
    
    TXPipe tx("my_pipe"); 
    RXPipe rx("my_pipe", &rxCallback, 0);

    std::string msg = "Through pipes and valleys I shall hunt you"; 

    rx.start();

    tx.send(msg.c_str(), msg.size());

    sleep(1); 

    rx.stop();
}

