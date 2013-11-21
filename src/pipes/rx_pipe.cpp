/**
 * @file rx_pipe.hpp 
 * @brief Receiver pipe class implementation. 
 */

#include "pipes/rx_pipe.hpp"

RXPipe::RXPipe(const std::string &pipefile, void (*rxCallback)(void)) :
        m_callback(rxCallback) {

    this->m_pipe = new std::filebuf;
    this->m_pipe->open(pipefile.c_str(), std::ios_base::in); 
}

RXPipe::~RXPipe() {

    delete this->m_pipe; 
}

