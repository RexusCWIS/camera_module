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
static void set_i2c_mode(void); 

static unsigned char buf[20];

int main(int argc, char *argv[]) {

	iss_init("/dev/ttyACM0"); 

	display_version(); 	

	set_i2c_mode(); 	

	exit(EXIT_SUCCESS); 
}

void set_i2c_mode(void) {
/*	
	buf[0] = 0x5Au; 
	buf[1] = 0x02u;
	buf[2] = 0x40u;
	buf[3] = 0x00u; 	

	if(write(fd, buf, 4) < 0) {
		perror("set_i2c_mode write"); 
	}
 
	if(tcdrain(fd) < 0) {
		perror("set_i2c_mode tcdrain"); 
	}
 
	if(read(fd, buf, 2) < 0) {
		perror("set_i2c_mode read"); 
	} 
	
	if(buf[0] != 0xFFu) {
		printf("set_i2c_mode: Error setting I2C mode!\n\n"); 
	} */
}

static void display_version(void) {
    
    iss_info_t info; 

    iss_get_info(&info); 

    std::cout << "USB-ISS Module ID: " << (unsigned int) info.id << std::endl;  
	std::cout << "USB-ISS Firmware version: " << (unsigned int) info.firmware << std::endl; 
    std::cout << "USB-ISS serial number: " << info.serial << std::endl;
}

