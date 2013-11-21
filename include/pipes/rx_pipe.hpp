/**
 * @file rx_pipe.hpp
 * @brief Receiver pipe class implementation.
 */

#ifndef DEF_RX_PIPE_HPP
#define DEF_RX_PIPE_HPP

#include <fstream>
#include <string>

class RXPipe {
    public: 
        RXPipe(const std::string &pipefile, void (*rxCallback)(void));
        ~RXPipe(); 

    protected:
        std::filebuf *m_pipe;
        void (*m_callback)(void); 
};

#endif  /* DEF_RX_PIPE_HPP */

