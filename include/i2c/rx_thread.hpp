/**
 * @file rx_thread.hpp
 * @brief I2C message reception handler thread. 
 */

#ifndef DEF_RX_THREAD_HPP
#define DEF_RX_THREAD_HPP

#include "i2c/i2c_bus.hpp"

class RXThread {
    public: 
        RXThread(I2CBus bus); 
        ~RXThread();

    private I2CBus m_bus; 
};

#endif  /* DEF_RX_THREAD_HPP */

