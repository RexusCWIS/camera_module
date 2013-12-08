/**
 * @file usbicc.h 
 * @brief USB-ICC interface library. 
 */

#ifndef DEF_USBICC_H
#define DEF_USBICC_H

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
}icc_mode_t; 

int icc_init(void); 

void icc_display_version(void);

int icc_set_mode(icc_mode_t mode); 

#endif	/* DEF_USBICC_H */

