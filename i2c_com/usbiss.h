/**
 * @file usbiss.h 
 * @brief USB-ISS interface library. 
 */

#ifndef DEF_USBISS_H
#define DEF_USBISS_H

typedef enum {
	IO_MODE  	= 0x00u,
	IO_CHANGE 	= 0x10u,
	I2C_S_20KHZ	= 0x20u,
	I2C_S_50KHZ     = 0x30u,
	I2C_S_100KHZ	= 0x40u, 
	I2C_S_400KHZ	= 0x50u, 
	I2C_H_100KHZ    = 0x60u, 
	I2C_H_400KHZ    = 0x70u, 
	I2C_H_1000KHZ	= 0x80u,
	SPI_MODE 	= 0x90u,
	SERIAL		= 0x1u
} iss_mode_t;

typedef struct {
    unsigned char id; 
    unsigned char firmware; 
    unsigned char serial[8]; 
} iss_info_t; 

int iss_init(const char *device); 

void iss_get_info(iss_info_t *info);

int iss_set_mode(iss_mode_t mode); 

#endif	/* DEF_USBISS_H */

