/** 
 * @file camera.hpp
 * @brief Camera abstract class.
 * @addtogroup cameras
 * @{
 */

#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP

#include "image.hpp"
#include "ring_buffer.hpp"

/**
 * @brief Abstract camera class. 
 * @details The @p Camera class represents the camera interface for the CWIS experiment. 
 *          Its only purpose is to provide a simple way to extend the camera acquisition
 *          software to different camera models. 
 */
class Camera {

    public: 
        /**
         * @brief Starts continuous image acquisition.
         */
        virtual void start(RingBuffer *ringBuffer, void (*callback)(char *) = NULL)  = 0;

        virtual void stop(void) = 0; 

        /**
         * @brief Acquires a single image. 
         */ 
        virtual void capture(Image *i) = 0; 

        /**
         * @brief Sets the framerate of the camera.  
         */
        virtual double setFramerate(double frameRate) = 0;
        
        double getFramerate(void) {
            return this->m_framerate; 
        }

        /**
         * @brief Sets the area of interest of the camera. 
         */
        virtual void setAreaOfInterest(int width, int height, int x, int y) = 0; 

        /**
         * @brief Displays manufacturing and runtime informations about the camera. 
         */
        virtual void displayInfo(void) = 0; 

        /**
         * @brief Puts the camera in a safe, low power state. 
         */
        virtual ~Camera() {};

    protected:
        /** @brief Current camera framerate (in frames per second). */
        double m_framerate;
        /** @brief Max image width, defined by the camera sensor. */
        int maxWidth; 
        /** @brief Max image height, defined by the camera sensor. */
        int maxHeight;
        /** @brief Pointer to the ring buffer currently used for image acquisition. */
        RingBuffer *m_ringBuffer;
        /** @brief User callback function called at each acquisition event. */
        void (*m_userCallback)(char *); 
        /** @brief Indicates if the camera is already running. */
        bool m_running; 
};


#endif  /* DEF_CAMERA_HPP */

/** @} */

