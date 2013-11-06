/** 
 * @file camera.hpp
 * @brief Camera abstract class. 
 */

#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP

#include "image.hpp"

/**
 * @brief Abstract camera class. 
 * @details The @p Camera class represents the camera interface for the CWIS experiment. 
 *          Its only purpose is to provide a simple way to extend the camera acquisition
 *          software to different camera models. 
 */
class Camera {

    public: 
        virtual void acquire(Image* ringBuffer[], unsigned int bufferSize) = 0;
        virtual void capture(Image *i) = 0; 
        virtual double setFramerate(double frameRate) = 0;
        double getFramerate(void) {
            return this->m_framerate; 
        }
        virtual void setAreaOfInterest(int width, int height, int x, int y) = 0; 
        virtual ~Camera() {};

    protected:
        /** @brief Current camera framerate (in frames per second). */
        double m_framerate;
        /** @brief Max image width, defined by the camera sensor. */
        int maxWidth; 
        /** @brief Max image height, defined by the camera sensor. */
        int maxHeight; 
};


#endif  /* DEF_CAMERA_HPP */

