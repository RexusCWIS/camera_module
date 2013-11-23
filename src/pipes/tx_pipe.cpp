/**
 * @file tx_pipe.cpp
 * @brief Transmitter pipe class definition. 
 */

#include "pipes/tx_pipe.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

TXPipe::TXPipe(const std::string &pipefile) {

    this->m_fifo = pipefile.c_str(); 
    mkfifo(this->m_fifo, 0666); 
    this->fd = open(this->m_fifo, O_WRONLY); 
}

TXPipe::~TXPipe() {

    close(this->fd); 
}

void TXPipe::send(const char *data, int dataSize) {
    
    write(this->fd, data, dataSize); 
}

