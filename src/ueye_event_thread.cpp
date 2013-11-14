/**
 * @file ueye_event_thread.cpp
 * @brief uEye camera event management thread class implementation.
 */

#include "ueye_event_thread.hpp"

UEye_EventThread::UEye_EventThread(HIDS camID, int event, void (*callback)(void)) :
    : m_camID(camID), m_event(event), m_eventCallback(callback) {

    this->m_stop = false; 
    this->m_running = false; 
}


UEye_EventThread::~UEye_EventThread() {
   
    if(this->m_running) { 
        this->stop(); 
    }
}

void UEye_EventThread::start(void) {

    if(!this->m_running) {
        
        this->m_stop = false;
        pthread_create(&m_eventThread, NULL, handler, this); 
    }
}

void UEye_EventThread::stop(void) {

    this->m_stop = true;
    this->waitForThreadEnd();

}

void UEye_EventThread::waitForThreadEnd(void) {

    pthread_join(m_eventThread, NULL); 
}

void UEye_EventThread::handler(void * arg) {

    UEye_EventThread thread = reinterpret_cast<UEye_EventThread *>(arg);

    while(!thread->m_stop) {
        
    }

    thread->m_running = false;
    return NULL; 
}

