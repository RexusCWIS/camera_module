/** 
 * @file rx_thread.cpp
 * @brief I2C message reception handler thread implementation.
 */

#include "serial/rx_thread.hpp"

RXThread::RXThread(I2CBus bus) : m_bus(bus) {

}

RXThread::~RXThread() {

}

