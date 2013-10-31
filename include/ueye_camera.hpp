/**
 * @file ueye_camera.hpp
 * @brief uEye camera controller class definition. 
 */

#ifndef DEF_UEYE_CAMERA_HPP
#define DEF_UEYE_CAMERA_HPP

#include "types.hpp"
#include "camera.hpp"
#include <uEye.h>

#define INIT_SUCCESS                (0)
#define ERROR_CANT_RETRIEVE_INFO    (-1)
#define ERROR_CANT_INIT_CAMERA      (-2)

class UEye_Camera: public Camera {
    
    public:

        /** 
         * @brief Initializes the camera with the given @p ID.
         * @details Each uEye camera has its own (configurable) camera ID. 
         *          This function tries to set up the given camera and retrieves
         *          basic parameters. 
         */
        UEye_Camera(HIDS cameraID);

        virtual void capture(int *image); 

        virtual void setFrameRate(double frameRate);

        virtual void setImageSize(int x, int y, int width, int height); 

        /**
         * @brief Class destructor. 
         * @details Cleans dynamically allocated memory objects and puts the camera
         *          back in standby mode. 
         */
        virtual ~UEye_Camera();
 
        /**
         * @brief Detects the number of uEye cameras connected to the system.
         * @returns Number of connected cameras. Returns -1 if it was not possible to retrieve this information. 
         */
        static Int32_t getNumberOfCameras(void); 

        /**
         * @brief Starts image acquisition. 
         */
        virtual void acquire(void); 

    private: 
        HIDS camID; 
        SENSORINFO sensorInfo; 

};

#endif  /* DEF_UEYE_CAMERA_HPP */

