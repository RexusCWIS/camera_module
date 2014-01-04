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

#include "pipes/tx_pipe.hpp"

#include "usbiss.h"

static void display_version(void); 

static unsigned char rx_buf[32]; 
static std::string output_file = "cwis_data.txt"; 

int main(int argc, char *argv[]) {

    TXPipe camera_tx("/tmp/camera_pipe.p"); 
    unsigned char current_order = 'S'; 

	iss_init("/dev/ttyACM0"); 

	display_version(); 	

	iss_set_mode(I2C_H_400KHZ, IO_MODE_DIGITAL_INPUT);

    /* Open the output file for writing */
    FILE *of = fopen(output_file, "wb"); 

    /** @todo Check errno values */
    if(of == NULL) {
        std::cerr << "Cannot create file: " << output_file << std::endl; 
        exit(EXIT_FAILURE); 
    }

    std::stringstream ss; 

    while(true) {

        /* Read the camera order */
	    iss_i2c_read(rx_buf, 0x1u, 0x23u, 0x0u);

        if(rx_buf[0] != current_order) {
	        std::cout << "Wrote to buffer" << std::endl; 
            camera_tx.send((const char *) rx_buf, 1);
	        current_order = rx_buf[0];  
        }

        /* Read the sensor data */
	    iss_i2c_read(rx_buf, 0xCu, 0x23u, 0x1u); 
	
	    unsigned int time = (((unsigned int) rx_buf[1]) << 8) + rx_buf[0];
        ss << time << "\t";  
        
        for(unsigned int incr = 4; incr < 12; incr += 2) {
            
            unsigned int data = (((unsigned int) rx_buf[incr+1]) << 8) + rx_buf[incr]; 
            ss << data << "\t"; 
        }

        ss << rx_buf[12] << "\t" << rx_buf[13]; 

        /* Write data to the output file */
        fwrite(ss.str().c_str(), sizeof(unsigned char), ss.str.size(), of); 
    }

    /* Close the output file */
    fclose(of); 
    /* Close the I2C communication channel */
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

