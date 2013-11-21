/** 
 * @file rx_thread.cpp
 * @brief I2C message reception handler thread implementation.
 */

#include "i2c/rx_thread.hpp"

RXThread::RXThread(I2CBus bus) : m_bus(bus) {

}

RXThread::~RXThread() {

}

