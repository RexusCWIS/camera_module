/**
 * @file camera.cpp
 * @brief uEye Camera controller class implementation. 
 */

#include "camera.hpp"
#include <uEye.h>
#include <iostream>

static inline void printCamInfo(UEYE_CAMERA_INFO *uci, SENSOR_INFO &sinfo) {
    
    std::cout << "Camera ID:\t" << uci->dwCameraID << "\n" <<
                 "Device ID:\t" << uci->dwDeviceID << "\n\n" <<
                 "Camera model:\t" << uci->Model << "\n" << 
                 "Serial number:\t" << uci->SerNo << "\n";
                 "Sensor model:\t" << sinfo->strSensorName << 
                 "Max image size:\t" << sinfo->nMaxWith << "x" << 
                                        sinfo->nMaxHeight << "\n\n"; 
}

Int32_t Camera::getNumberOfCameras(void) {

    INT nbOfCams = 0;
    INT status = is_GetNumberOfCameras(&nbOfCams);

    if (status == IS_INVALID_PARAMETER) {
        nbOfCams = -1; 
    }

    return (Int32_t) nbOfCams; 
}

UInt32_t Camera::init(void) {

    INT status = 0;

    /* Basic camera info fetching */

    /* Allocate memory for the camera info fields */
    this->camInfo = (UEYE_CAMERA_LIST *) new UInt8_t[sizeof(ULONG) + sizeof(UEYE_CAMERA_INFO)];
    /* We work with a single camera connected to the system */
    this->camInfo->dwCount = 1;  

    status = is_GetCameraList(this->camInfo);
    if (status != IS_SUCCESS) {
        return ERROR_CANT_RETRIEVE_INFO; 
    }

<<<<<<< HEAD
=======
    printCamInfo(this->camInfo->uci);
>>>>>>> 96fd6ea8a1eb51f3b89f43e80eff62d9da391604

    /* Camera sensor informations */
    this->camID = camInfo->uci[0].dwCameraID;
    status = is_GetSensorInfo(this->camID, &this->sensorInfo); 
    
    if (status != IS_SUCCESS) {
        return ERROR_CANT_RETRIEVE_INFO; 
    }

    /* Display informations on the standard output */
    printCamInfo(&(this->camInfo.uci));


    /* Initialize the camera */
    status = is_InitCamera(&this->camID, NULL);

    if (status != IS_SUCCESS) {
        return ERROR_CANT_INIT_CAMERA;  
    }

    return INIT_SUCCESS; 
}

Camera::~Camera() {

    INT status = is_ExitCamera(this->camID);
    (void) status;

    delete [] camInfo; 
}

