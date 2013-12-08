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

static unsigned char buf[20];

int main(int argc, char *argv[]) {

	iss_init("/dev/ttyACM0"); 

	display_version(); 	

	iss_set_mode(I2C_S_100KHZ, IO_MODE_DIGITAL_INPUT);

    iss_close(); 

	exit(EXIT_SUCCESS); 
}

static void display_version(void) {
    
    iss_info_t info; 

    iss_get_info(&info); 

    std::cout << "USB-ISS Module ID: " << (unsigned int) info.id << std::endl;  
	std::cout << "USB-ISS Firmware version: " << (unsigned int) info.firmware << std::endl; 
    std::cout << "USB-ISS serial number: " << info.serial << std::endl;
}

