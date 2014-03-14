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

#define SERIAL_IO   0x62u

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
static unsigned char *iss_serial_buf;
static unsigned int iss_serial_buf_size; 

/*
 * STATIC FUNCTIONS
 */

static inline void iss_get_serial_baudrate_divisor(iss_serial_baudrate_t baudrate, 
                                               unsigned char *high_byte, 
                                               unsigned char *low_byte); 

static inline void iss_transmission(const unsigned char tx_buf[], unsigned int tx_bytes, 
                                    unsigned char rx_buf[], unsigned int rx_bytes); 

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
    	
	/* Flush data waiting for transmission */
    	tcflush(dev_fd, TCIOFLUSH); 

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

int iss_set_i2c_mode(iss_mode_t mode, iss_io_mode_t io_mode, iss_serial_baudrate_t baudrate) {

    unsigned char buf[5] = {USB_ISS, ISS_MODE, mode};  

    /* I2C + serial */
    if(mode & 0x1u) {
        iss_get_serial_baudrate_divisor(baudrate, &buf[3], &buf[4]); 
        iss_transmission(buf, 5u, buf, 2u); 
    }

    /* I2C + IO */
    else {
        buf[3] = io_mode; 
        iss_transmission(buf, 4u, buf, 2u); 
    }

    /* Error handling */
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

int iss_i2c_read(unsigned char rx_buf[], 
                 unsigned int rx_bytes, 
                 unsigned char i2c_addr, 
                 unsigned char dev_reg) {
    
    iss_buf[0] = I2C_AD1; 
    iss_buf[1] = i2c_addr;
    iss_buf[2] = dev_reg;
    iss_buf[3] = rx_bytes;

    for(unsigned int index = 0; index < 4; index++) {

	printf("%x ", iss_buf[index]); 
    }

    printf("\n"); 

    iss_transmission(iss_buf, 4u, rx_buf, rx_bytes); 
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

    iss_buf[tx_bytes + 4] = '\0';
    for(unsigned int index = 0; index < (4); index++) {

	printf("%x ", iss_buf[index]); 
    }

    printf("\n%s\n", &iss_buf[4]); 

    iss_transmission(iss_buf, (tx_bytes + 4u), iss_buf, 1u);

    if(iss_buf[0] == 0u) {
        
        #ifdef ISS_VERBOSE
            fprintf(stderr, "iss_i2c_write: Could not perform write operation\n"); 
        #endif  /* ISS_VERBOSE */
    }

    return (int) iss_buf[0]; 
}

int iss_serial_send(const unsigned char tx_buf[], 
                    unsigned int tx_bytes) {

    
}

void iss_close(void) {
  
    /* Flush data waiting for transmission */
    tcflush(dev_fd, TCIOFLUSH); 
 
    /* Restore port default */
    if (tcsetattr(dev_fd, TCSANOW, &iss_defaults) < 0) { 
        perror("tcsetattr default");
    }

    close(dev_fd); 
}


static inline void iss_get_serial_baudrate_divisor(iss_serial_baudrate_t baudrate, 
                                                   unsigned char *high_byte, 
                                                   unsigned char *low_byte) {

    switch(baudrate) {
        
        case ISS_SERIAL_300:
            *high_byte = 0x27u;
            *low_byte  = 0x0Fu; 
            break;

        case ISS_SERIAL_1200:
            *high_byte = 0x09u;
            *low_byte  = 0xC3u; 
            break;

        case ISS_SERIAL_2400:
            *high_byte = 0x04u;
            *low_byte  = 0xE1u; 
            break;

        case ISS_SERIAL_9600:
            *high_byte = 0x01u;
            *low_byte  = 0x37u; 
            break; 

        case ISS_SERIAL_19_2K:
            *high_byte = 0x0u;
            *low_byte  = 0x9Bu; 
            break; 

        case ISS_SERIAL_38_4K:
            *high_byte = 0x0u;
            *low_byte  = 0x4Du; 
            break; 
        case ISS_SERIAL_57_6K:
            *high_byte = 0x0u;
            *low_byte  = 0x33u; 
            break; 

        case ISS_SERIAL_115_2K:
            *high_byte = 0x0u;
            *low_byte  = 0x19u; 
            break; 
        case ISS_SERIAL_250K:
            *high_byte = 0x0u;
            *low_byte  = 0xBu; 
            break; 
        case ISS_SERIAL_1M:
            *high_byte = 0x0u;
            *low_byte  = 0x03u; 
            break; 

        default:
            break; 
    }

    return;
}

static inline void iss_transmission(const unsigned char tx_buf[], unsigned int tx_bytes, 
                                    unsigned char rx_buf[], unsigned int rx_bytes) {
 
	if(write(dev_fd, tx_buf, tx_bytes) < 0) {
		perror("write"); 
	}
 
	if(tcdrain(dev_fd) < 0) {
		perror("tcdrain"); 
	}

	usleep(100000);
 
	if(read(dev_fd, rx_buf, rx_bytes) < 0) {
		perror("read"); 
	}
}

