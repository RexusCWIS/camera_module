/**
 * @file camera.h
 * @brief Camera functionalities declaration.
 */
 
#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H

#include "ueye_event_thread.h"

#include <uEye.h>

class ueye_event_thread;

class ueye_camera {
public:
	ueye_camera(HIDS cameraID);
	
	~ueye_camera();

	void set_aoi(int x, int y, int width, int height);

	void set_auto_exposure(void);

	void set_auto_gain(void);

	void set_pixel_clock(unsigned int pixel_clock);

	double set_framerate(double framerate);

	void start_acquisition(char* ring_buffer[], 
						   unsigned int ring_buffer_size, unsigned int width,
						   unsigned int height);
						   
	static void acquisition_handler(ueye_camera* const camera);

	void stop_acquisition(void);
	
	HIDS get_camera_id(void) const;
	
	unsigned int get_nb_of_images_acquired(void) const;
	
	bool is_over(void) const;
	
private:
	HIDS m_camera_id;
	int* m_memory_ids;
	unsigned int m_nb_of_images_acquired;
	
	char** m_buffer;
	unsigned int m_buffer_size;
	
	ueye_event_thread* m_acquisition_event_thread;
	
	bool m_running;
	bool m_finished;
};

#endif  /* DEF_CAMERA_H */
