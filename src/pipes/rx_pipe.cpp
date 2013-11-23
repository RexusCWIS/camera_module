/**
 * @file rx_pipe.hpp 
 * @brief Receiver pipe class implementation. 
 */

#include "pipes/rx_pipe.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

/** @todo Implement error handling (open and close operations, ...) */
RXPipe::RXPipe(const std::string &pipefile, void (*rxCallback)(char *, int), int dataFrameSize) :
        m_callback(rxCallback) {

    this->m_running = false; 
    this->m_stop = false;

    this->m_dataFrameSize = (dataFrameSize > 0) ? dataFrameSize : BUFFER_SIZE;
    
    this->m_fifo = pipefile.c_str();
    //mkfifo(this->m_fifo, 0666); 
    this->m_fd = open(this->m_fifo, O_RDONLY); 

}

RXPipe::~RXPipe() {

    if(this->m_running) {
        this->stop(); 
    }

    close(this->m_fd); 
}

void RXPipe::start(void) {

    if(this->m_running) {
        this->stop(); 
    }

    this->m_running = true;
    this->m_stop = false;

    pthread_create(&(this->m_rxThread), NULL, &thread, this); 
    
}

void RXPipe::stop(void) {

    this->m_stop = true; 
    this->waitForThreadEnd();
    this->m_running = false; 
}

void * RXPipe::thread(void *arg) {

    RXPipe *rxPipe = reinterpret_cast<RXPipe *>(arg);

    while(!rxPipe->m_stop) {
       
        read(rxPipe->m_fd, rxPipe->m_rxBuffer, rxPipe->m_dataFrameSize); 
        rxPipe->m_callback(rxPipe->m_rxBuffer, rxPipe->m_dataFrameSize); 
    }

    return NULL;
}

void RXPipe::waitForThreadEnd(void) {

    pthread_join(this->m_rxThread, NULL); 
}

