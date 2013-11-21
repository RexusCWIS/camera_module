/**
 * @file rx_pipe.hpp 
 * @brief Receiver pipe class implementation. 
 */

#include "pipes/rx_pipe.hpp"

/** @todo Implement error handling (open and close operations, ...) */
RXPipe::RXPipe(const std::string &pipefile, void (*rxCallback)(void)) :
        m_callback(rxCallback) {

    this->m_running = false; 
    this->m_stop = false;

    this->m_pipe = new std::filebuf;
    this->m_pipe->open(pipefile.c_str(), std::ios_base::in); 
}

RXPipe::~RXPipe() {

    this->m_pipe->close(); 
    delete this->m_pipe; 
}

void RXPipe::start(void) {

    this->m_running = true;
    pthread_create(&(this->m_rxThread), NULL, &thread, this); 
    
}

void RXPipe::stop(void) {

    this->m_stop = true; 
    this->waitForThreadEnd(); 
}

void * RXPipe::thread(void *arg) {

    RXPipe *rxPipe = reinterpret_cast<RXPipe *>(arg);

    while(!rxPipe->m_stop) {

    }

    return NULL;
}

void RXPipe::waitForThreadEnd(void) {

    pthread_join(this->m_rxThread, NULL); 
}

