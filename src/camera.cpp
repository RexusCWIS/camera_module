/**
 * @file camera.cpp
 * @brief uEye Camera controller class implementation. 
 */

#include "camera.hpp"
#include <uEye.h>

UInt32_t Camera::init(void) {

    HIDS camID = 0;
    INT status = is_InitCamera(&camID, NULL);

    if (status != IS_SUCCESS) {

        return -1;  
    }

    return 0; 
}

