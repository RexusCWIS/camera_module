/**
 * @file ueye_camera.cpp
 * @brief uEye camera controller class implementation. 
 */

#include "ueye_camera.hpp"
#include "ueye_exception.hpp"

#include <uEye.h>
#include <string>
#include <iostream>
#include <stdio.h>

UEye_Camera::UEye_Camera(HIDS cameraID) : camID(cameraID) {

    INT status = 0;

    /** @todo Throw an exception if the initialization went wrong. */
    /* Initialize the camera */
    status = is_InitCamera(&this->camID, NULL);

    if(status != IS_SUCCESS) {
        
        string msg = "Could not initialize camera ";
        char camString[21];
        sprintf(camString, "%d", this->camID);
        msg += camString; 

        throw UEye_Exception(status, msg); 
    }

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

    delete [] camInfo; 
}

