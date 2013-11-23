/**
 * @file rx_pipe.hpp
 * @brief Receiver pipe class definition.
 */

#ifndef DEF_RX_PIPE_HPP
#define DEF_RX_PIPE_HPP

#include <pthread.h>
#include <fstream>
#include <string>

class RXPipe {
    public: 
        RXPipe(const std::string &pipefile, void (*rxCallback)(char *, int), int dataFrameSize = 0);
        ~RXPipe(); 
        
        void start(void); 
        void stop(void); 

    protected:
        const char *m_fifo; 
        int m_fd; 
        void (*m_callback)(char *, int); 
        int m_dataFrameSize;
        char *m_rxBuffer; 
        pthread_t m_rxThread;
        bool m_running; 
        bool m_stop;
        
        static void * thread(void *arg);
        void waitForThreadEnd(void); 
};

#endif  /* DEF_RX_PIPE_HPP */

