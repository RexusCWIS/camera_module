/**
 * @file ueye_camera.cpp
 * @brief uEye camera controller class implementation. 
 */

#include "ueye_camera.hpp"
#include "exceptions/ueye_exception.hpp"

#include "utilities.hpp"

#include <uEye.h>
#include <string>
#include <iostream>
#include <stdio.h>

UEye_Camera::UEye_Camera(HIDS cameraID) : camID(cameraID) {

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

Int32_t UEye_Camera::getNumberOfCameras(void) {

    INT nbOfCams = 0;
    INT status = is_GetNumberOfCameras(&nbOfCams);

    if (status == IS_INVALID_PARAMETER) {
        nbOfCams = -1; 
    }

    return (Int32_t) nbOfCams; 
}

void UEye_Camera::acquire(Image* ringBuffer[], unsigned int bufferSize) {

    INT status  = IS_SUCCESS;
    int *memIDs = new int[bufferSize];  

    for(unsigned int incr = 0; incr < bufferSize; incr++) {
        status = is_SetAllocatedImageMem(this->camID, ringBuffer[incr]->getWidth(), ringBuffer[incr]->getHeight(), 8,
                                            (char *)ringBuffer[incr]->getImageBuffer(), &memIDs[incr]);
        
        if(status == IS_SUCCESS) {
            
            status = is_AddToSequence(this->camID, (char *)ringBuffer[incr]->getImageBuffer(), memIDs[incr]);  
        }

        if(status != IS_SUCCESS) {
            string msg = "Could not set up the buffer for stream acquisition.";
            throw UEye_Exception(this->camID, status, msg); 
        }
    }

    delete [] memIDs; 
}

UEye_Camera::~UEye_Camera() {

    INT status = is_ExitCamera(this->camID);
    (void) status;
}

