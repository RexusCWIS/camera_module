/**
 * @file ueye_camera.cpp
 * @brief uEye camera controller class implementation. 
 */

#include "ueye_camera.hpp"
#include "ueye_exception.hpp"

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
        
        string msg = "Could not initialize camera ";
        string_appendInt(msg, this->camID); 

        throw UEye_Exception(status, msg); 
    }

    /* Access sensor informations */
    status = is_GetSensorInfo(this->camID, &(this->sensorInfo));

    if(status != IS_SUCCESS) {
        
        string msg = "Could not retrieve sensor info.";

        throw UEye_Exception(status, msg); 
    }

    this->maxWidth  = this->sensorInfo.nMaxWidth; 
    this->maxHeight = this->sensorInfo.nMaxHeight; 
}

void UEye_Camera::capture(Image *i) {

}

void UEye_Camera::setFrameRate(double frameRate) {

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

void UEye_Camera::acquire(void) {

}

UEye_Camera::~UEye_Camera() {

    INT status = is_ExitCamera(this->camID);
    (void) status;
}

