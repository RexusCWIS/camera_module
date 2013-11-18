/**
 * @file i2c_bus.hpp
 * @brief Object representation of an I2C bus. 
 */

#ifndef DEF_I2C_BUS_HPP
#define DEF_I2C_BUS_HPP

#include <string>

class I2CBus {
    public: 
        I2CBus(std::string tty); 
        ~I2CBus(); 
};

#endif  /* DEF_I2C_BUS_HPP */
 
