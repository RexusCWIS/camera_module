/** 
 * @file camera.hpp
 * @brief Camera abstract class. 
 */

#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP

/**
 * @brief Abstract camera class. 
 * @details The @p Camera class represents the camera interface for the CWIS experiment. 
 *          Its only purpose is to propose a simple way to extend the camera acquisition
 *          software to different camera models. 
 */
class Camera {

    public: 
        virtual void acquire(void) = 0; 
        virtual ~Camera() {}; 
};


#endif  /* DEF_CAMERA_HPP */

