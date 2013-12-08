/**
 * @file usbiss.c
 * @brief USB-ISS interface library implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <termios.h>
#include <unistd.h>

#include "usbiss.h"

#define USB_ISS 0x5Au
#define ISS_VERSION 0x01u
#define ISS_MODE    0x02u
#define GET_SER_NUM 0x03u

static struct termios iss_defaults; 
static struct termios iss_config;

static int dev_fd; 

/*
 * STATIC FUNCTIONS
 */

static inline void iss_transmission(unsigned char tx_buf[], unsigned int tx_bytes, 
                                    unsigned char rx_buf[], unsigned int rx_bytes) {
 
	if(write(dev_fd, tx_buf, tx_bytes) < 0) {
		perror("write"); 
	}
 
	if(tcdrain(dev_fd) < 0) {
		perror("tcdrain"); 
	}
 
	if(read(dev_fd, rx_buf, rx_bytes) < 0) {
		perror("read"); 
	}
}

/*
 * INTERFACE FUNCTIONS
 */

int iss_init(const char *device) {

	dev_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

    if(dev_fd == -1) {
        return -1; 
    }
    
	/* Grab current port settings */
	if(tcgetattr(dev_fd, &iss_defaults) < 0) {
		perror("tcgetattr"); 
	}

	/* Set configuration for raw data */
	cfmakeraw(&iss_config); 

	/* Set new configuration */
	if(tcsetattr(dev_fd, TCSANOW, &iss_config) < 0) {
		perror("tcsetattr config"); 
	}

    return 0; 
}

void iss_get_info(iss_info_t *info) {

    /* First retrieve the id and firmware version */
    unsigned char buf[3] = {USB_ISS, ISS_VERSION};

    iss_transmission(buf, 2, buf, 3); 

    info->id = buf[0]; 
    info->firmware = buf[1];

    /* Retrieve the serial number */
    buf[0] = USB_ISS; 
    buf[1] = GET_SER_NUM;

    iss_transmission(buf, 2, info->serial, 8); 
}

