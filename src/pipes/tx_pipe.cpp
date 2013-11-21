/**
 * @file tx_pipe.cpp
 * @brief Transmitter pipe class definition. 
 */

#include "pipes/tx_pipe.hpp"

TXPipe::TXPipe(const std::string &pipefile) {

    this->m_pipe = new std::filebuf; 
    this->m_pipe->open(pipefile.c_str(), std::ios_base::out); 
}

TXPipe::~TXPipe() {

    this->m_pipe->close(); 
    delete this->m_pipe; 
}

void TXPipe::send(char *data, int dataSize) {
    
    this->m_pipe->sputn(data, dataSize);
    this->m_pipe->pubsync(); 
}

