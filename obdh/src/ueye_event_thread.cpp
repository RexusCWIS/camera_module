/**
 * @file ueye_event_thread.cpp
 * @brief uEye camera event management thread class implementation.
 */

#include "ueye_event_thread.h"

#define EVENT_TIMEOUT 1

ueye_event_thread::ueye_event_thread(ueye_camera* const camera, int event, void (*callback)(ueye_camera* const)) :
	m_camera(camera), m_event(event), m_event_callback(callback) {

    this->m_stop = false; 
    this->m_running = false; 
}


ueye_event_thread::~ueye_event_thread() {
   
    if(this->m_running) { 
        this->stop(); 
    }
}

bool ueye_event_thread::is_running(void) const {
    
    return this->m_running; 
}

void ueye_event_thread::start(void) {

    if(!this->m_running) {
        
        int status = is_EnableEvent(this->m_camera->get_camera_id(), this->m_event);
        
        if(status == IS_SUCCESS) {
            this->m_stop = false;
            pthread_create(&m_event_thread, NULL, handler, this); 
        }
    }
}

void ueye_event_thread::stop(void) {

    this->m_stop = true;
    this->wait_for_thread_end();
    is_DisableEvent(this->m_camera->get_camera_id(), this->m_event); 

}

void ueye_event_thread::wait_for_thread_end(void) {

    pthread_join(m_event_thread, NULL); 
}

void* ueye_event_thread::handler(void* arg) {

    ueye_event_thread* thread = reinterpret_cast<ueye_event_thread *>(arg);

    while(!thread->m_stop) {
        
        if(is_WaitEvent(thread->m_camera->get_camera_id(), thread->m_event, EVENT_TIMEOUT) == IS_SUCCESS) {
            thread->m_event_callback(thread->m_camera);
        }
    }

    thread->m_running = false;
    return NULL; 
}

