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

typedef enum {
    ISS_SERIAL_300      = 0u,
    ISS_SERIAL_1200     = 1u,
    ISS_SERIAL_2400     = 2u,
    ISS_SERIAL_9600     = 3u, 
    ISS_SERIAL_19_2K    = 4u,
    ISS_SERIAL_38_4K    = 5u,
    ISS_SERIAL_57_6K    = 6u,
    ISS_SERIAL_115_2K   = 7u,
    ISS_SERIAL_250K     = 8u,
    ISS_SERIAL_1M       = 9u,
    ISS_SERIAL_UNUSED   = 0xFFu
} iss_serial_baudrate_t; 

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
 * @brief Sets the USB-ISS device to operate in the given I2C @p mode.
 * @details Configures the USB-ISS device to operate in I2C mode. The two free pins are configured 
 *          either as IO (configured using the @p io_mode bitfield) or as a serial UART bus. 
 *          In the latter case, the baudrate is configured by the @p baudrate parameter. 
 * @param[in]   mode    Configures the main operation of the device: IO, serial, SPI or I2C.
 * @param[in]   io_mode Configures the IO operation of the device: analog or digital input, output. 
 * @note    When setting GPIO mode, set the @p baudrate parameter to @ref ISS_SERIAL_UNUSED. 
 *          When setting serial mode set the @p io_mode parameter to 0. 
 */
int iss_set_i2c_mode(iss_mode_t mode, iss_io_mode_t io_mode, iss_serial_baudrate_t baudrate);

/**
 * @brief Reads data from an I2C device register. 
 * @details Reads @p tx_bytes from the @p dev_reg device register of the I2C slave with the @p i2c_addr
 *          address. The data is stored in the @p rx_buf array. 
 * @param[in]   tx_buf      Array used to hold the read data.
 * @param[in]   tx_bytes    Number of bytes to read (up to the size of @p rx_buf). 
 * @param[in]   i2c_addr    Address of the slave I2C device. 
 * @param[in]   dev_reg     Address of the device register to read.
 * @see iss_i2c_write()
 */
int iss_i2c_read(unsigned char rx_buf[], 
                  unsigned int rx_bytes, 
                  unsigned char i2c_addr,
                  unsigned char dev_reg);

/**
 * @brief Writes to an I2C device register. 
 * @details Writes @p tx_bytes from the @p tx_buf array to a device register (@p dev_reg) address
 *          in the I2C slave with address @p i2c_addr. 
 * @param[in]   tx_buf      Array holding the data to write.
 * @param[in]   tx_bytes    Number of bytes to write (up to the size of @p tx_buf). 
 * @param[in]   i2c_addr    Address of the slave I2C device. 
 * @param[in]   dev_reg     Address of the device register requested for the write operation.
 * @see iss_i2c_read()
 */
int iss_i2c_write(unsigned char tx_buf[], 
                  unsigned int tx_bytes, 
                  unsigned char i2c_addr, 
                  unsigned char dev_reg); 

/**
 * @brief Sends the given data bytes for serial transmission. 
 * @param[in]   tx_buf      Array holding the data to send. 
 * @param[in]   tx_bytes    Number of bytes to send (up to the size of @p tx_buf). 
 * @returns 1 if the data was successfully transmitted, 0 if the data could not be transmitted. 
 * @note The size of the USB-ISS Tx buffer is 30 bytes, meaning that a maximum of 30 bytes can 
 *       be sent at once if the buffer was empty. If the buffer is full, this function will return
 *       0.
 * @see iss_serial_receive(), iss_i2c_write(), iss_i2c_read()
 */
int iss_serial_send(const unsigned char tx_buf[], 
                    unsigned int tx_bytes); 

/**
 * @brief Fills the given buffer with @p tx_bytes bytes from the internal library buffer.
 * @details An internal 62-bytes wide buffer stores received data on the USB-ISS module. 
 *          Due to the USB-ISS serial communication mechanism, this buffer must be emptied
 *          each time data is sent for transmission using the @ref iss_serial_send() function. 
 *          The library stores this data in an internal buffer to avoid loss of data while 
 *          waiting for the user to read this buffer.
 *
 *          This function retrieves the first @p tx_bytes from the internal library buffer
 *          and stores them in the @p rx_buf user buffer. Those bytes are removed from 
 *          the internal library buffer. 
 * @returns 1 if the data was successfully stored in @p rx_buf, 0 if it was not possible 
 *          to write @p tx_bytes in @p rx_buf. This can happen if @p rx_buf is not large
 *          enough or if the internal library buffer holds less than @p tx_bytes bytes. 
 * @see iss_serial_send(), iss_i2c_write(), iss_i2c_read(), iss_serial_rx_buffer_size()
 */
int iss_serial_receive(unsigned char rx_buf[], 
                       unsigned int tx_bytes);

/**
 * @brief Returns the number of bytes received and stored in the reception buffer. 
 * @details When called, this function first empties the internal USB-ISS buffer 
 *          and appends it to the host library buffer. 
 * @returns Size of the internal library buffer, in bytes. 
 * @note    To avoid data loss in the case of fast transfer speeds, the internal USB-ISS buffer
 *          must be flushed in the internal library buffer. Call the @ref iss_serial_flush_buffer()
 *          function manually to trigger this operation. 
 * @see iss_serial_flush_buffer()
 */
int iss_serial_rx_buffer_size(void); 

/**
 * @brief Flushes the internal USB-ISS serial reception buffer to a buffer managed by the library on the host. 
 * @returns Number of bytes transferred to the host buffer. 
 * @note    To read the received data and transfer it to user-defined structures, use the 
 *          @ref iss_serial_read() function.
 * @see iss_serial_receive(), iss_serial_rx_buffer_size()
 */
int iss_serial_flush_buffer(void); 

/**
 * @brief Closes the communication with the USB-ISS device. 
 * @details Restore the initial configuration of the USB-ISS device port and closes the file. 
 * @see iss_init()
 */
void iss_close(void); 

#endif	/* DEF_USBISS_H */

