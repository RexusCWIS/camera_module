/**
 * @file i2c_manager.cpp
 * @brief I2C Manager class implementation.
 */

#include "i2c_manager.h"

#include "usbiss.h"

#include <sstream>
#include <stdio.h>

i2c_manager::i2c_manager(const char* device, const char* log_file,
						 void (*order_callback)(char)):
				m_device(device), m_log_file(log_file) {

	m_stop    = false;
	m_running = false;
	
	m_time = 0;
	
	m_order_callback = order_callback;
	
	iss_init(device);
	iss_set_i2c_mode(I2C_H_400KHZ, IO_MODE_DIGITAL_INPUT, ISS_SERIAL_UNUSED);
	
	pthread_create(&m_listener_thread, NULL, listener, this);
}

i2c_manager::~i2c_manager() {
	
    m_stop = true;
    wait_for_thread_end();
    /* Close the I2C communication channel */
    iss_close(); 
}

void* i2c_manager::listener(void* arg) {

    i2c_manager* manager = reinterpret_cast<i2c_manager*>(arg);

	char rx_buf[32];
	char current_order = 'S';

	std::stringstream ss;
    	
	/* Open the output file for writing */
    	FILE *of = fopen(manager->m_log_file, "wb");

    while(!manager->m_stop) {
        
        /* Read the camera order */
	    iss_i2c_read((unsigned char*) rx_buf, 0x1u, 0x23u, 0x0u);

		/* If a new order has been given, transmit it to the callback function */
        if(rx_buf[0] != current_order) {
			manager->m_order_callback(rx_buf[0]); 
        }

        /* Read the sensor data */
	    iss_i2c_read((unsigned char*) rx_buf, 0xCu, 0x23u, 0x1u); 
	
	    unsigned int time = (((unsigned int) rx_buf[3]) << 24) + 
				(((unsigned int) rx_buf[2]) << 16) + 
				(((unsigned int) rx_buf[1]) << 8) + 
				(unsigned int) rx_buf[0];


		manager->m_time = time;

		
        ss << time << "\t"; 
        
        for(unsigned int incr = 4; incr < 12; incr += 2) {
            
            unsigned int data = (((unsigned int) rx_buf[incr+1]) << 8) + rx_buf[incr]; 
            ss << data << "\t"; 
        }

        ss << rx_buf[12] << "\t" << rx_buf[13]; 

        /* Write data to the output file */
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), of); 
    }
    
    /* Close the output file */
    fclose(of); 

    manager->m_running = false;
    return NULL; 
}

void i2c_manager::write(char* data, unsigned int size, unsigned char device_register) {

	iss_i2c_write((unsigned char*) data, size, 0x23u, device_register);

}

unsigned int i2c_manager::get_time(void) const {
	
	return m_time;
}

void i2c_manager::wait_for_thread_end(void) {
	pthread_join(m_listener_thread, NULL);
}
