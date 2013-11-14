/**
 * @file ueye_event_thread.hpp
 * @brief uEye camera event management thread class definition.
 */

#ifndef DEF_UEYE_EVENT_THREAD_HPP
#define DEF_UEYE_EVENT_THREAD_HPP

#include <uEye.h>
#include <pthread.h>

class UEye_EventThread {

    public: 
        UEye_EventThread(HIDS camID, int event, void (*callback)(void)); 
        ~UEye_EventThread(); 
        
        void start(void); 
        void stop(void);

    private: 
        HIDS m_camID; 
        int  m_event;
        void (*m_eventCallback)(void);
        pthread_t m_eventThread; 
        bool m_stop; 
        bool m_running; 

        void waitForThreadEnd(void); 
        static void * handler(void *arg);
};

#endif  /* DEF_UEYE_EVENT_THREAD_HPP */

