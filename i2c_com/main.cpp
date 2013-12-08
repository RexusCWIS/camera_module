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

static void display_version(void); 
static void set_i2c_mode(void); 

static unsigned char buf[20];

/** Device file descriptor */
static int fd; 

int main(int argc, char *argv[]) {

	struct termios defaults; 
	struct termios config; 

	const char *device = "/dev/ttyACM0";

	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd == -1) {
		std::cerr << "Could not open device: " << device << std::endl; 
		exit(EXIT_FAILURE); 
	}

	/* Grab current port settings */
	if(tcgetattr(fd, &defaults) < 0) {
		perror("tcgetattr"); 
	}

	/* Set configuration for raw data */
	cfmakeraw(&config); 

	/* Set new configuration */
	if(tcsetattr(fd, TCSANOW, &config) < 0) {
		perror("tcsetattr config"); 
	}

	display_version(); 	

	set_i2c_mode(); 	

	close(fd); 

	exit(EXIT_SUCCESS); 
}

void set_i2c_mode(void) {
	
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
	}
}

static void display_version(void) {

	buf[0] = 0x5Au; 
	buf[1] = 0x01u; 

	if(write(fd, buf, 2) < 0) {
		perror("display_version write"); 
	}
 
	if(tcdrain(fd) < 0) {
		perror("display_version tcdrain"); 
	}
 
	if(read(fd, buf, 3) < 0) {
		perror("display_version read"); 
	} 

	printf("USB-ISS Module ID:\t%u\n", buf[0]); 
	printf("USB-ISS Firmware version:\t%u\n\n", buf[1]);  
}

