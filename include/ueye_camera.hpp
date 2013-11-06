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

        virtual void capture(Image *i); 

        virtual void setFrameRate(double frameRate);

        virtual void setAreaOfInterest(int x, int y, int width, int height); 

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
         * @brief Returns the current pixel clock of the camera. 
         */
        unsigned int getPixelClock(void); 

        /** 
         * @brief Sets the pixel clock of the camera. 
         */
        void setPixelClock(unsigned int pixelClock);

        /**
         * @brief Returns the pixel clock range of the camera.
         * @details This function modifies the @p range array parameter. 
         *          Once the function has ended, the first index of the array
         *          holds the minimum pixel clock, the second index holds the 
         *          maximum pixel clock and the third index holds the pixel
         *          clock step increment.
         * @param[out]  range   Pointer to the range array. 
         */
        void getPixelClockRange(unsigned int range[]); 

        /**
         * @brief Starts image acquisition. 
         */
        virtual void acquire(void); 

    private: 
        HIDS camID; 
        SENSORINFO sensorInfo; 

};

#endif  /* DEF_UEYE_CAMERA_HPP */

