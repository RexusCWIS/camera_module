/**
 * @file main.cpp 
 * @brief Basic pipe communication test. 
 */

#include "pipes/rx_pipe.hpp"
#include "pipes/tx_pipe.hpp"

#include <string>
#include <iostream>

static void rxCallback(char *data, int dataSize) {

    std::cout << data << std::endl; 
}

int main(int argc, char **argv) {
    
    TXPipe tx("my_pipe"); 
    RXPipe rx("my_pipe", &rxCallback, 0);

    std::string msg = "Through pipes and valleys I shall hunt you"; 

    rx.start();

    tx.send(msg.c_str(), msg.size());

    for(unsigned int incr = 0; incr < 100000000; incr++)
        ;

    rx.stop();
}

