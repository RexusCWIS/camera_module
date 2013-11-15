/**
 * @file ueye_camera.cpp
 * @brief uEye camera controller class implementation. 
 */

#include "ueye_camera.hpp"
#include "exceptions/ueye_exception.hpp"

#include "utilities.hpp"

#include <uEye.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <stdio.h>

UEye_Camera::UEye_Camera(HIDS cameraID) : camID(cameraID), m_stop(false) {

    INT status = 0;

    /* Initialize the camera */
    status = is_InitCamera(&(this->camID), NULL);

    if(status != IS_SUCCESS) {
        
        string msg = "Could not initialize camera.";

        throw UEye_Exception(this->camID, status, msg); 
    }
    /* Access sensor informations */
    status = is_GetSensorInfo(this->camID, &(this->sensorInfo));

    if(status != IS_SUCCESS) {
        
        string msg = "Could not retrieve sensor info.";

        throw UEye_Exception(this->camID, status, msg); 
    }

    this->maxWidth  = this->sensorInfo.nMaxWidth; 
    this->maxHeight = this->sensorInfo.nMaxHeight;

    /* Set color mode to 8-bit monochromatic */
    status = is_SetColorMode(this->camID, IS_CM_MONO8); 

    if(status != IS_SUCCESS) {
        string msg = "Could not set the color mode.";
        throw UEye_Exception(this->camID, status, msg); 
    }

    /* Set the minimum pixel clock */
    this->setPixelClock(this->getMinimumPixelClock());
}

void UEye_Camera::capture(Image *i) {

    int memID = 0; 
    INT status = is_SetAllocatedImageMem(this->camID, i->getWidth(), i->getHeight(), 8,
                                         (char *)i->getImageBuffer(), &memID);

    if(status == IS_SUCCESS) {
        status = is_SetImageMem(this->camID, (char *)i->getImageBuffer(), memID); 
    }

    if(status != IS_SUCCESS) {
        string msg = "Could not set up the image for single capture mode.";
        throw UEye_Exception(this->camID, status, msg); 
    }

   
    status = is_SetDisplayMode(this->camID, IS_GET_DISPLAY_MODE); 

    if(status != IS_SET_DM_DIB) {
        cout << "Setting DIB mode." << endl;
        /* Set the camera in RAM acquisition mode. The image will be directly transferred to RAM. */
        status = is_SetDisplayMode(this->camID, IS_SET_DM_DIB); 
        
        if(status != IS_SUCCESS) {
            string msg = "Could not set the display mode.";
            throw UEye_Exception(this->camID, status, msg); 
        }
    }

    /* Put the camera in software trigger mode. */
    status = is_SetExternalTrigger(this->camID, IS_SET_TRIGGER_SOFTWARE);

    if(status != IS_SUCCESS) {
        string msg = "Could not set trigger mode.";
        throw UEye_Exception(this->camID, status, msg); 
    }

    /* One-shot acquisition, we wait for the image to be stored in RAM */
    status = is_FreezeVideo(this->camID, IS_WAIT);

    if(status != IS_SUCCESS) {
        string msg = "Could not acquire the image.";
        throw UEye_Exception(this->camID, status, msg); 
    }
    
    /* Free the image from the uEye SDK */
    status = is_FreeImageMem(this->camID, i->getImageBuffer(), memID);

    return; 
}

double UEye_Camera::setFramerate(double framerate) {
    
    INT status = is_SetFrameRate(this->camID, framerate, &this->m_framerate);
    
    if(status != IS_SUCCESS) {
        string msg = "Could not set the framerate."; 
        throw UEye_Exception(this->camID, status, msg); 
    }
    return this->m_framerate; 
}

void UEye_Camera::setAreaOfInterest(int x, int y, int width, int height) {

    IS_RECT aoi;

    aoi.s32X = x;
    aoi.s32Y = y; 
    aoi.s32Width  = width;
    aoi.s32Height = height; 

    is_AOI(this->camID, IS_AOI_IMAGE_SET_AOI, (void*) &aoi, sizeof(aoi)); 
}

void UEye_Camera::displayInfo(void) {

    unsigned int pixelClockRange[3];
    this->getPixelClockRange(pixelClockRange); 

    std::cout << "Pixel clock:\n============\n" <<
                 "Current:\t"   << this->getPixelClock() << 
                 "\nDefault:\t" << this->getDefaultPixelClock() << 
                 "\nRange:\t"   << pixelClockRange[0] << "(min)\t" <<
                                   pixelClockRange[1] << "(max)"   <<
                 "\nStep size:\t" << pixelClockRange[2] << std::endl; 
}

int UEye_Camera::getNumberOfCameras(void) {

    INT nbOfCams = 0;
    INT status = is_GetNumberOfCameras(&nbOfCams);

    if (status == IS_INVALID_PARAMETER) {
        nbOfCams = -1; 
    }

    return (int) nbOfCams; 
}

unsigned int UEye_Camera::getPixelClock(void) {

    UINT pixelClock = 0; 
    INT status = is_PixelClock(this->camID, IS_PIXELCLOCK_CMD_GET, 
                               (void *) &pixelClock, sizeof(pixelClock)); 
    (void) status;

    return (unsigned int) pixelClock;
}

void UEye_Camera::setPixelClock(unsigned int pixelClock) {
    
    INT status = is_PixelClock(this->camID, IS_PIXELCLOCK_CMD_SET, 
                               (void *) &pixelClock, sizeof(pixelClock));
    (void) status; 
}

void UEye_Camera::getPixelClockRange(unsigned int range[]) {
    
    INT status = is_PixelClock(this->camID, IS_PIXELCLOCK_CMD_GET_RANGE, 
                               (void *) range, 3 * sizeof(unsigned int));

    (void) status;
}

unsigned int UEye_Camera::getMinimumPixelClock(void) {
    
    unsigned int range[3]; 
    this->getPixelClockRange(range);

    return range[0]; 
}

unsigned int UEye_Camera::getMaximumPixelClock(void) {
    
    unsigned int range[3]; 
    this->getPixelClockRange(range);

    return range[1]; 
}

unsigned int UEye_Camera::getPixelClockStep(void) {
    
    unsigned int range[3]; 
    this->getPixelClockRange(range);

    return range[2]; 
}

unsigned int UEye_Camera::getDefaultPixelClock(void) {
    
    unsigned int defaultPixelClock; 

    INT status = is_PixelClock(this->camID, IS_PIXELCLOCK_CMD_GET_DEFAULT, 
                               (void *) &defaultPixelClock, sizeof(defaultPixelClock));

    (void) status;

    return defaultPixelClock; 
}

void UEye_Camera::start(RingBuffer *ringBuffer) {
 
    /* If the camera was already running, stop it */
    if(this->m_running) {
        this->stop(); 
    }

    this->m_ringBuffer = ringBuffer; 

    INT status  = IS_SUCCESS;

    /* Set camera memory buffers */
    this->m_memID = new int[ringBuffer->getSize()];  

    for(unsigned int incr = 0; incr < ringBuffer->getSize(); incr++) {
        status = is_SetAllocatedImageMem(this->camID, ringBuffer->at(incr)->getWidth(), ringBuffer->at(incr)->getHeight(), 8,
                                            ringBuffer->at(incr)->getImageBuffer(), &this->m_memID[incr]);
        
        if(status == IS_SUCCESS) {
            
            status = is_AddToSequence(this->camID, ringBuffer->at(incr)->getImageBuffer(), this->m_memID[incr]);  
        }

        if(status != IS_SUCCESS) {
            string msg = "Could not set up the buffer for stream acquisition.";
            throw UEye_Exception(this->camID, status, msg); 
        }
    }
    
    /* Install event handler threads */
    /** @todo Add status related event handlers */
    acquisitionEventThread = new UEye_EventThread(this, IS_SET_EVENT_FRAME, &UEye_Camera::acquisitionCallback);
    acquisitionEventThread->start();

    /* Start live capture */
    status = is_CaptureVideo(this->camID, IS_DONT_WAIT); 

    if(status != IS_SUCCESS) {
        string msg = "Could not start live camera acquisition."; 
        throw UEye_Exception(this->camID, status, msg); 
    }
    
    return; 
}

void UEye_Camera::stop(void) {

    /* Stop live acquisition */
    INT status = IS_NO_SUCCESS;
    while(status != IS_SUCCESS) {
        status = is_StopLiveVideo(this->camID, IS_WAIT); 
    }
   
    /* Clear image sequence from the camera memory */
    is_ClearSequence(this->camID); 

    for(unsigned int incr; incr < this->m_ringBuffer->getSize(); incr++) {

        is_FreeImageMem(this->camID, this->m_ringBuffer->at(incr)->getImageBuffer(), this->m_memID[incr]); 
    }
    
    /* Stop the event handler threads */
    acquisitionEventThread->stop(); 
   
    /* Free allocated memory */
    delete acquisitionEventThread; 
    delete [] this->m_memID; 
}

void UEye_Camera::acquisitionCallback(const UEye_Camera *camera) {

}

UEye_Camera::~UEye_Camera() {

    INT status = is_ExitCamera(this->camID);
    (void) status;
}

HIDS UEye_Camera::getCameraID(void) const {

    return this->camID; 
}

