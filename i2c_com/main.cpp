/**
 * @file main.cpp
 * @brief I2C communication process.
 */

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "usbiss.h"

static void display_version(void); 

static unsigned char tx_buf[32] = "I love those incredible buffers";
static unsigned char rx_buf[32]; 

int main(int argc, char *argv[]) {

	iss_init("/dev/ttyACM0"); 

	display_version(); 	

	iss_set_mode(I2C_H_400KHZ, IO_MODE_DIGITAL_INPUT);
	
//	iss_i2c_write(tx_buf, 32u, 0x22u, 0x0u);

//	sleep(1); 

	iss_i2c_read(rx_buf, 0x1u, 0x23u, 0x0u); 

	std::cout << "Camera order: " << rx_buf[0] << std::endl; 

	iss_i2c_read(rx_buf, 0xCu, 0x23u, 0x1u); 
	
	unsigned int time = (((unsigned int) rx_buf[1]) << 8) + rx_buf[0]; 
	std::cout << "Current experiment time: " << time << std::endl; 

    iss_close(); 

	exit(EXIT_SUCCESS); 
}

static void display_version(void) {
    
    iss_info_t info; 

    iss_get_info(&info); 

    std::cout << "USB-ISS Module ID: " << (unsigned int) info.id << std::endl;  
	std::cout << "USB-ISS Firmware version: " << (unsigned int) info.firmware << std::endl; 
    std::cout << "USB-ISS serial number: " << info.serial << std::endl << std::endl;
}

