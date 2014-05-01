/**
 * @file camera.h
 * @brief Camera functionalities declaration.
 */
 
#ifndef DEF_CAMERA_H
#define DEF_CAMERA_H

#include "ueye_event_thread.h"
#include "ueye_exception.h"

#include "image_buffer.h"

#include <uEye.h>

#include <map>

class ueye_event_thread;

class ueye_camera {
public:
	ueye_camera(HIDS cameraID);
	
	~ueye_camera();

	void set_aoi(int x, int y, int width, int height);

    void set_saturation(int u, int v);

	void set_exposure(double exposure);

    double get_exposure(void) const;

	void set_max_exposure(void);

	void set_auto_exposure(void);

    void set_gain(void);

    int get_gain(void) const;

	void set_auto_gain(void);

	void set_pixel_clock(unsigned int pixel_clock);

    unsigned int get_pixel_clock(void) const;

	double set_framerate(double framerate);

	void start_acquisition(image_buffer *ring_buffer);
						   
	static void acquisition_handler(ueye_camera* const camera);

	void stop_acquisition(void);
	
	HIDS get_camera_id(void) const;
	
	unsigned int get_nb_of_images_acquired(void) const;
	
	bool is_over(void) const;
	
private:
	HIDS m_camera_id;
	char m_camera_status;
	unsigned int m_nb_of_images_acquired;
	
	int* m_memory_ids;
	std::map<int, int> m_id_to_index_map;
	image_buffer* m_buffer;
	
	ueye_event_thread* m_acquisition_event_thread;
	
	bool m_running;
	bool m_finished;
};

#endif  /* DEF_CAMERA_H */
