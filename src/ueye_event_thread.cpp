/**
 * @file ueye_event_thread.cpp
 * @brief uEye camera event management thread class implementation.
 */

#include "ueye_event_thread.hpp"

#define EVENT_TIMEOUT 1

UEye_EventThread::UEye_EventThread(const UEye_Camera *camera, int event, void (*callback)(const UEye_Camera *)) :
	m_camera(camera), m_event(event), m_eventCallback(callback) {

    this->m_stop = false; 
    this->m_running = false; 
}


UEye_EventThread::~UEye_EventThread() {
   
    if(this->m_running) { 
        this->stop(); 
    }
}

bool UEye_EventThread::isRunning(void) const {
    
    return this->m_running; 
}

void UEye_EventThread::start(void) {

    if(!this->m_running) {
        
        int status = is_EnableEvent(this->m_camera->getCameraID(), this->m_event);
        
        if(status == IS_SUCCESS) {
            this->m_stop = false;
            pthread_create(&m_eventThread, NULL, handler, this); 
        }
    }
}

void UEye_EventThread::stop(void) {

    this->m_stop = true;
    this->waitForThreadEnd();
    is_DisableEvent(this->m_camera->getCameraID(), this->m_event); 

}

void UEye_EventThread::waitForThreadEnd(void) {

    pthread_join(m_eventThread, NULL); 
}

void * UEye_EventThread::handler(void * arg) {

    UEye_EventThread * thread = reinterpret_cast<UEye_EventThread *>(arg);

    while(!thread->m_stop) {
        
        if(is_WaitEvent(thread->m_camera->getCameraID(), thread->m_event, EVENT_TIMEOUT) == IS_SUCCESS) {
            thread->m_eventCallback(thread->m_camera);  
        }
    }

    thread->m_running = false;
    return NULL; 
}

