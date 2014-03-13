/**
 * @file camera.h
 * @brief Camera functionalities declaration.
 */
 
#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H

#include "ueye_event_thread.hpp"

#include <uEye.h>

class ueye_camera {
public:
	void init(HIDS cameraID);

	void set_aoi(int x, int y, int width, int height);

	void set_pixel_clock(unsigned int pixel_clock);

	double set_framerate(double framerate);

	void start_acquisition(unsigned char* ring_buffer[], 
						   unsigned int ring_buffer_size, unsigned int width,
						   unsigned int height);

	void stop_acquisition(void);
	
private:
	HIDS m_camera_id;
	int* m_memory_ids;
	UEye_EventThread* m_acquisition_event_thread;
	
	bool m_running;
}

#endif  /* DEF_CAMERA_H */
