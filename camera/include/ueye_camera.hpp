/**
 * @file ueye_camera.hpp
 * @brief uEye camera controller class definition. 
 * @addtogroup cameras
 * @{
 */

#ifndef DEF_UEYE_CAMERA_HPP
#define DEF_UEYE_CAMERA_HPP

#include "camera.hpp"
#include "ueye_event_thread.hpp"
#include <uEye.h>

#include <pthread.h>

class UEye_EventThread; 

/**
 * @brief uEye camera class. 
 * @details This class interacts with the IDS uEye camera SDK to provide
 *          an abstraction of the camera model. It provides functions to set parameters
 *          and acquire single or multiple images. 
 */
class UEye_Camera: public Camera {
    
    public:

        /** 
         * @brief Initializes the camera with the given @p ID.
         * @details Each uEye camera has its own (configurable) camera ID. 
         *          This function tries to set up the given camera and retrieves
         *          basic parameters. 
         */
        UEye_Camera(HIDS cameraID);

        /**
         * @brief Acquires a single image on the camera. 
         * @param[out]  i   Image item storing the captured image. 
         * @note The image must be of the same size that the area of interest (AOI) of the camera. 
         *       Use the @ref setAreaOfInterest() method to set the AOI properly. 
         * @see setAreaOfInterest()
         */
        virtual void capture(Image *i); 

        virtual double setFramerate(double framerate);

        /**
         * @brief Sets the area of interest of the camera.
         * @details The area of interest (AOI) of the camera is a portion of the field of view
         *          selected for acquisition. The rest of the field of view is ignored. 
         *          This feature is used to reach higher framerates. 
         */
        virtual void setAreaOfInterest(int x, int y, int width, int height); 

        /** 
         * @brief Displays informations about the camera. 
         * @details This function prints various informations about the camera on the 
         *          standard output: default parameters, current configuration... 
         */
        virtual void displayInfo(void);

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
        static int getNumberOfCameras(void); 

        /**
         * @brief Returns the current pixel clock of the camera. 
         */
        unsigned int getPixelClock(void); 

        /** 
         * @brief Sets the pixel clock of the camera. 
         * @see getMinimumPixelClock(), getMaximumPixelClock(), getPixelClockStep(),
         *      getPixelClockRange(), getDefaultPixelClock()
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
         * @see setPixelClock(), getMinimumPixelClock(), getMaximumPixelClock(), 
         *      getPixelClockStep(), getDefaultPixelClock()
         */
        void getPixelClockRange(unsigned int range[]);

        /** 
         * @brief Returns the minimum authorized pixel clock value.
         * @see setPixelClock(), getMaximumPixelClock(), getPixelClockStep(), 
         *      getPixelClockRange(), getDefaultPixelClock()
         */
        unsigned int getMinimumPixelClock(void); 

        /**
         * @brief Returns the maximum authorized pixel clock value. 
         * @see setPixelClock(), getMinimumPixelClock(), getPixelClockStep(), 
         *      getPixelClockRange(), getDefaultPixelClock()
         */
        unsigned int getMaximumPixelClock(void); 

        /**
         * @brief Returns the discrete step for pixel clock values. 
         * @details The step represents the minimum difference between 
         *          two admissible values. Smaller steps must be rounded
         *          up to the value returned by this function. 
         * @see getMinimumPixelClock(), getMaximumPixelClock(), 
         *      getPixelClockRange(), getDefaultPixelClock()
         */
        unsigned int getPixelClockStep(void); 

        /**
         * @brief Returns the default  pixel clock value.
         * @see setPixelClock, getMinimumPixelClock(), getMaximumPixelClock(), 
         *      getPixelClockRange(), getPixelClockStep()
         */
        unsigned int getDefaultPixelClock(void); 

        /**
         * @brief Starts image acquisition. 
         */
        virtual void start(RingBuffer *ringBuffer, void (*callback)(char *) = NULL); 

        /**
         * @brief Stops the image acquisition. 
         */
        virtual void stop(void);

        HIDS getCameraID(void) const; 

    private: 
        HIDS camID; 
        SENSORINFO sensorInfo;
        int *m_memID;                       /**< @brief Pointer to an array of memory ID tags for acquisition. */
        bool m_stop;                        /**< @brief Specifies that the acquisition should stop. */
        UEye_EventThread *acquisitionEventThread;

        /** @brief Callback function for an acquisition event. */
        static void acquisitionCallback(const UEye_Camera *camera); 
};

#endif  /* DEF_UEYE_CAMERA_HPP */

/** @} */

