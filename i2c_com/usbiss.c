/**
 * @file usbiss.c
 * @brief USB-ISS interface library implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>

#include "usbiss.h"

/** @brief If this macro is defined, errors are printed on stderr. */
#define ISS_VERBOSE

/*
 * USB-ISS COMMANDS
 */

#define I2C_SGL     0x53u
#define I2C_AD0     0x54u
#define I2C_AD1     0x55u
#define I2C_AD2     0x56u
#define I2C_DIRECT  0x57u
#define I2C_TEST    0x58u

#define USB_ISS     0x5Au
#define ISS_VERSION 0x01u
#define ISS_MODE    0x02u
#define GET_SER_NUM 0x03u

#define ISS_ACKNOWLEDGE 0xFFu
#define ISS_UNKNOWN_COMMAND     0x05u
#define ISS_INTERNAL_ERROR1     0x06u
#define ISS_INTERNAL_ERROR2     0x07u

#define ISS_MAX_BUFFER_SIZE 60u

/*
 * STATIC DATA
 */

static struct termios iss_defaults; 
static struct termios iss_config;

static int dev_fd; 

static unsigned char iss_buf[ISS_MAX_BUFFER_SIZE];

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

    iss_transmission(buf, 2u, buf, 3u); 

    info->id = buf[0]; 
    info->firmware = buf[1];

    /* Retrieve the serial number */
    buf[0] = USB_ISS; 
    buf[1] = GET_SER_NUM;

    iss_transmission(buf, 2u, info->serial, 8u); 
}

int iss_set_mode(iss_mode_t mode, iss_io_mode_t io_mode) {

    unsigned char buf[4] = {USB_ISS, ISS_MODE, 
                            (unsigned char) mode, 
                            (unsigned char) io_mode};

    iss_transmission(buf, 4u, buf, 2u); 

    if(buf[0] != ISS_ACKNOWLEDGE) {
       
        #ifdef ISS_VERBOSE
        switch(buf[1]) {

            case ISS_UNKNOWN_COMMAND:
                fprintf(stderr, "iss_set_mode: unknown command\n");
                break; 

            case ISS_INTERNAL_ERROR1: 
                fprintf(stderr, "iss_set_mode: internal error 1\n"); 
                break;

            case ISS_INTERNAL_ERROR2:
                fprintf(stderr, "iss_set_mode: internal error 2\n");
                break; 

            default: 
                fprintf(stderr, "iss_set_mode: unknown error\n");
                break; 
        }
        #endif  /* ISS_VERBOSE */
    }

    return (int) buf[1]; 
}

int iss_i2c_write(unsigned char tx_buf[], 
                   unsigned int tx_bytes, 
                   unsigned char i2c_addr,
                   unsigned char dev_reg) {   
    
    iss_buf[0] = I2C_AD1; 
    iss_buf[1] = i2c_addr; 
    iss_buf[2] = dev_reg;
    iss_buf[3] = tx_bytes;

    memcpy(&(iss_buf[4]), tx_buf, tx_bytes);

    iss_transmission(iss_buf, (tx_bytes + 4u), iss_buf, 1u);

    if(iss_buf[0] == 0u) {
        
        #ifdef ISS_VERBOSE
            fprintf(stderr, "iss_i2c_write: Could not perform write operation"); 
        #endif  /* ISS_VERBOSE */
    }

    return (int) iss_buf[0]; 
}

void iss_close(void) {
   
    /* Restore port default */
    if (tcsetattr(dev_fd, TCSANOW, &iss_defaults) < 0) { 
        perror("tcsetattr default");
    }

    close(dev_fd); 
}



