/**
 * @file camera.hpp
 * @brief uEye camera controller class definition. 
 */

#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP

#include "types.hpp"
#include <uEye.h>

#define INIT_SUCCESS                (0)
#define ERROR_CANT_RETRIEVE_INFO    (-1)
#define ERROR_CANT_INIT_CAMERA      (-2)

class Camera {
    
    public: 
        Camera(); 
        ~Camera();
 
        /**
         * @brief Detects the number of uEye cameras connected to the system.
         * @returns Number of connected cameras. Returns -1 if it was not possible to retrieve this information. 
         */
        static Int32_t getNumberOfCameras(void); 

        /**
         * @brief Initializes the camera. 
         * @details This function tries to initialize the camera. 
         */
        UInt32_t init(void);

        /**
         * @brief Starts image acquisition. 
         */
        void acquire(void); 

    private: 
        HIDS camID; 
        UEYE_CAMERA_LIST *camInfo;
        SENSOR_INFO sensorInfo; 

};

#endif  /* DEF_CAMERA_HPP */

