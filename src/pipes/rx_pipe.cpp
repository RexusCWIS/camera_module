/**
 * @file rx_pipe.hpp 
 * @brief Receiver pipe class implementation. 
 */

#include "pipes/rx_pipe.hpp"

/** @todo Implement error handling (open and close operations, ...) */
RXPipe::RXPipe(const std::string &pipefile, void (*rxCallback)(char *, int)) :
        m_callback(rxCallback) {

    this->m_running = false; 
    this->m_stop = false;

    this->m_pipe = new std::filebuf;
    this->m_pipe->open(pipefile.c_str(), std::ios_base::in); 
}

RXPipe::~RXPipe() {

    if(this->m_running) {
        this->stop(); 
    }

    this->m_pipe->close(); 
    delete this->m_pipe; 
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
        if(rxPipe->m_pipe->is_open()) {
            
            std::streamsize nbOfData = rxPipe->m_pipe->in_avail(); 
            if(nbOfData > 0) {
    
                char *data = new char[nbOfData]; 
                rxPipe->m_pipe->sgetn(data, nbOfData);
                rxPipe->m_pipe->pubsync(); 

                rxPipe->m_callback(data, nbOfData); 

                delete [] data; 
            }
        }
    }

    return NULL;
}

void RXPipe::waitForThreadEnd(void) {

    pthread_join(this->m_rxThread, NULL); 
}

