/**
 * @file rx_pipe.hpp
 * @brief Receiver pipe class implementation.
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
        std::filebuf *m_pipe;
        void (*m_callback)(char *, int); 
        int m_dataFrameSize; 
        pthread_t m_rxThread;
        bool m_running; 
        bool m_stop;
        
        static void * thread(void *arg);
        void waitForThreadEnd(void); 
};

#endif  /* DEF_RX_PIPE_HPP */

