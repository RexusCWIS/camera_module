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

typedef enum {
    IO_MODE_OUTPUT_LOW    = 0x0u,
    IO_MODE_OUTPUT_HIGH   = 0x1u,
    IO_MODE_DIGITAL_INPUT = 0x2u, 
    IO_MODE_ANALOG_INPUT  = 0x3u
} iss_io_mode_t; 

typedef struct {
    unsigned char id; 
    unsigned char firmware; 
    unsigned char serial[8]; 
} iss_info_t; 

/**
 * @brief Initializes the library to interact with the given USB-ISS @p device.
 * @details Opens the USB-ISS device and configures it to transmit raw binary data.  
 * @param[in]   device  Name of the USB-ISS @p device in the device tree (i.e. /dev/). 
 * @note @ref iss_close() should always be called when done working with the USB-ISS module.
 * @see iss_close()
 */
int iss_init(const char *device); 

/**
 * @brief Fills the given @p info structure. 
 * @param[out]  info    USB-ISS info structure. Contains the module ID, the current firmware
 *                      version and the serial number of the device.
 */
void iss_get_info(iss_info_t *info);

/**
 * @brief Sets the USB-ISS device to operate in the given @p mode and @p io_mode. 
 * @param[in]   mode    Configures the main operation of the device: IO, serial, SPI or I2C.
 * @param[in]   io_mode Configures the IO operation of the device: analog or digital input, output. 
 */
int iss_set_mode(iss_mode_t mode, iss_io_mode_t io_mode);

void iss_i2c_read(unsigned char rx_buf[], 
                  unsigned int rx_bytes, 
                  unsigned char i2c_addr,
                  unsigned char dev_reg);

int iss_i2c_write(unsigned char tx_buf[], unsigned int tx_bytes, unsigned char i2c_addr); 

/**
 * @brief Closes the communication with the USB-ISS device. 
 * @details Restore the initial configuration of the USB-ISS device port and closes the file. 
 * @see iss_init()
 */
void iss_close(void); 

#endif	/* DEF_USBISS_H */

