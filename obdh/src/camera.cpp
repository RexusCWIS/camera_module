/**
 * @file camera.cpp
 * @brief Camera control functionalities.
 */

#include "camera.h"

ueye_camera::ueye_camera(HIDS camera_id): m_camera_id(camera_id), m_running(false) {
    
    INT status = 0;
    
    /* Initialize the camera */
    status = is_InitCamera(&(camera_id), NULL);

    if(status != IS_SUCCESS) {
        
        string msg = "Could not initialize camera.";

        throw UEye_Exception(camera_id, status, msg); 
    }

    if(status != IS_SUCCESS) {
        
        string msg = "Could not retrieve sensor info.";

        throw UEye_Exception(camera_id, status, msg); 
    }

    /* Set color mode to 8-bit monochromatic */
    status = is_SetColorMode(m_camera_id, IS_CM_MONO8); 

    if(status != IS_SUCCESS) {
        string msg = "Could not set the color mode.";
        throw UEye_Exception(m_camera_id, status, msg); 
    }

    /* Set the minimum pixel clock */
    ueye_set_pixel_clock(camera_id, 16u);
}

void ueye_camera::set_aoi(int x, int y, int width, int height) {

    IS_RECT aoi;

    aoi.s32X = x;
    aoi.s32Y = y; 
    aoi.s32Width  = width;
    aoi.s32Height = height; 

    is_AOI(m_camera_id, IS_AOI_IMAGE_SET_AOI, (void*) &aoi, sizeof(aoi)); 
}

void ueye_camera::set_pixel_clock(unsigned int pixel_clock) {
    
    INT status = is_PixelClock(m_camera_id, IS_PIXELCLOCK_CMD_SET, 
                               (void *) &pixel_clock, sizeof(pixel_clock));
    (void) status;
}

double ueye_camera::set_framerate(double framerate) {

	double new_framerate = 0.0;

    INT status = is_SetFrameRate(m_camera_id, framerate, new_framerate);
    
    if(status != IS_SUCCESS) {
        string msg = "Could not set the framerate."; 
        throw UEye_Exception(m_camera_id, status, msg); 
    }
    return new_framerate; 
}

void ueye_camera::start_acquisition(unsigned char* ring_buffer[], 
					   unsigned int ring_buffer_size, unsigned int width,
					   unsigned int height) {
					   
    /* If the camera was already running, stop it */
    if(m_running) {
        this->stop(); 
    }

    /* Set the camera in live acquisition mode */
    INT status  = IS_SUCCESS;

    /* Set camera memory buffers */
    m_memory_ids = new int[ring_buffer_size];  

    for(unsigned int incr = 0; incr < ring_buffer_size; incr++) {
        status = is_SetAllocatedImageMem(m_camera_id, width, height, 8,
                                            ring_buffer[incr], &m_memory_ids[incr]);
        
        if(status == IS_SUCCESS) {
            
            status = is_AddToSequence(m_camera_id, ring_buffer[incr], m_memory_ids[incr]);  
        }

        if(status != IS_SUCCESS) {
            string msg = "Could not set up the buffer for stream acquisition.";
            throw UEye_Exception(m_camera_id, status, msg); 
        }
    }
    
    /* Install event handler threads */
    /** @todo Add status related event handlers */
    m_acquisition_event_thread = new ueye_event_thread(this, IS_SET_EVENT_FRAME, &ueye_camera::acquisition_callback);
    acquisition_event_thread->start();

    /* Start live capture */
    status = is_CaptureVideo(m_camera_id, IS_DONT_WAIT); 

    if(status != IS_SUCCESS) {
        string msg = "Could not start live camera acquisition."; 
        throw UEye_Exception(m_camera_id, status, msg); 
    }

    m_running = true; 
    
    return; 
}

void ueye_camera::stop(void) {

}

HIDS ueye_camera::get_camera_id(void) const {

	return m_camera_id;
}