/**
 * @file i2c_manager.h
 * @brief I2C Manager class.
 */

#ifndef DEF_I2C_MANAGER_H
#define DEF_I2C_MANAGER_H

#include <pthread.h>

class i2c_manager {

public:
	i2c_manager(const char* device, const char* log_file, void (*order_callback)(void));
	
	~i2c_manager();
	
	static void* listener(void* arg);
	
	void write(char* data, unsigned int size, unsigned char device_register);
	
	unsigned int get_time(void) const;

private:
	void wait_for_thread_end(void);
	
	const char* m_device;
	const char* m_log_file;
	void (*m_order_callback)(char);
	
	unsigned int m_time;
	
	pthread_t m_listener_thread;
	
	bool m_stop;
	bool m_running;
};

#endif  /* DEF_I2C_MANAGER_H */ 
