/**
 * @file camera.cpp
 * @brief Camera control functionalities.
 */

#include "camera.h"

#include <iostream>
#include <string>

extern unsigned int system_time;

ueye_camera::ueye_camera(HIDS camera_id) {

    m_camera_id = camera_id;
    m_running   = false;
    m_finished  = false;
    m_nb_of_images_acquired = 0;
    
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
    set_pixel_clock(16u);
}

ueye_camera::~ueye_camera() {

    INT status = is_ExitCamera(m_camera_id);
    (void) status;
}

void ueye_camera::set_aoi(int x, int y, int width, int height) {

    IS_RECT aoi;

    aoi.s32X = x;
    aoi.s32Y = y; 
    aoi.s32Width  = width;
    aoi.s32Height = height; 

    is_AOI(m_camera_id, IS_AOI_IMAGE_SET_AOI, (void*) &aoi, sizeof(aoi)); 
}

void ueye_camera::set_auto_exposure(void) {
    
    double auto_exposure = 1.0; 
    if(is_SetAutoParameter(m_camera_id, IS_SET_ENABLE_AUTO_SHUTTER, 
       					   &auto_exposure, NULL) != IS_SUCCESS) {
        std::cerr << "Could not set auto exposure time" << std::endl; 
    }
}

void ueye_camera::set_auto_gain(void) {

    double auto_gain = 1.0; 
    if(is_SetAutoParameter(m_camera_id, IS_SET_ENABLE_AUTO_GAIN, 
                           &auto_gain, NULL) != IS_SUCCESS) {
        std::cerr << "Could not set auto gain" << std::endl; 
    }
    
}

void ueye_camera::set_pixel_clock(unsigned int pixel_clock) {
    
    INT status = is_PixelClock(m_camera_id, IS_PIXELCLOCK_CMD_SET, 
                               (void *) &pixel_clock, sizeof(pixel_clock));
    (void) status;
}

double ueye_camera::set_framerate(double framerate) {

	double new_framerate = 0.0;

    INT status = is_SetFrameRate(m_camera_id, framerate, &new_framerate);
    
    if(status != IS_SUCCESS) {
        std::string msg = "Could not set the framerate."; 
        throw UEye_Exception(m_camera_id, status, msg); 
    }
    return new_framerate; 
}

void ueye_camera::start_acquisition(image_buffer* ring_buffer) {
					   
    /* If the camera was already running, top it */
    if(m_running || m_finished) {
        this->stop_acquisition(); 
    }
    
    m_finished = false;
    m_nb_of_images_acquired = 0;
    m_buffer = ring_buffer;

    /* Set the camera in live acquisition mode */
    INT status  = IS_SUCCESS;

    /* Set camera memory buffers */
    m_memory_ids = new int[ring_buffer->size];  

    for(unsigned int incr = 0; incr < ring_buffer->size; incr++) {
        status = is_SetAllocatedImageMem(m_camera_id, ring_buffer->width, ring_buffer->height, 8,
                                            ring_buffer->images[incr].buffer, &m_memory_ids[incr]);
        
        if(status == IS_SUCCESS) {
	    m_id_to_index_map[m_memory_ids[incr]] = incr;          
            status = is_AddToSequence(m_camera_id, ring_buffer->images[incr].buffer, m_memory_ids[incr]);
        }

        if(status != IS_SUCCESS) {
            std::string msg = "Could not set up the buffer for stream acquisition.";
            throw UEye_Exception(m_camera_id, status, msg);
        }
    }
    
    /* Install event handler threads */
    /** @todo Add status related event handlers */
    m_acquisition_event_thread = new ueye_event_thread(this, IS_SET_EVENT_FRAME, 
    												   &ueye_camera::acquisition_handler);
    m_acquisition_event_thread->start();

    /* Start live capture */
    status = is_CaptureVideo(m_camera_id, IS_DONT_WAIT);

    if(status != IS_SUCCESS) {
        std::string msg = "Could not start live camera acquisition.";
        throw UEye_Exception(m_camera_id, status, msg);
    }

    m_running = true;
    
    return;
}

void ueye_camera::acquisition_handler(ueye_camera* const camera) {

    int mem_id = 0;
    int index  = 0;

    char* dummy = NULL;

    is_GetActiveImageMem(camera->m_camera_id, &dummy, &mem_id);

    index = camera->m_id_to_index_map[mem_id] - 1;

    camera->m_buffer->images[index].time = system_time;
    camera->m_buffer->images[index].written = true;
    	
    camera->m_nb_of_images_acquired++;
	
    std::cout << "[" << system_time << "] " << camera->m_nb_of_images_acquired 
		  << " images" << std::endl;

    /** @todo Handle buffer overflow */
    if(camera->m_nb_of_images_acquired == (camera->m_buffer->size) - 1) {
        is_StopLiveVideo(camera->m_camera_id, IS_WAIT);
        camera->m_finished = true;
        std::cout << "Stopped the acquisition to avoid buffer overflow" << std::endl;
    }
}
 
void ueye_camera::stop_acquisition(void) {

    /* Stop live acquisition */
    INT status = IS_NO_SUCCESS;
    while(status != IS_SUCCESS) {
        status = is_StopLiveVideo(m_camera_id, IS_WAIT); 
    }

    this->m_running = false; 
   
    /* Clear image sequence from the camera memory */
    is_ClearSequence(m_camera_id);

    for(unsigned int incr = 0; incr < m_buffer->size; incr++) {
        is_FreeImageMem(m_camera_id, m_buffer->images[incr].buffer, m_memory_ids[incr]); 
    }
    
    /* Stop the event handler threads */
    m_acquisition_event_thread->stop(); 
   
    /* Free allocated memory */
    delete m_acquisition_event_thread; 
    delete [] m_memory_ids; 
}

HIDS ueye_camera::get_camera_id(void) const {

	return m_camera_id;
}

unsigned int ueye_camera::get_nb_of_images_acquired(void) const {
    
    return m_nb_of_images_acquired;
}

bool ueye_camera::is_over(void) const {
	return m_finished;
}
