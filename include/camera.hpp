/**
 * @file camera.hpp
 * @brief uEye camera controller class definition. 
 */

#ifndef DEF_CAMERA_HPP
#define DEF_CAMERA_HPP

#include "types.hpp"

class Camera {
    
    public: 
        Camera(); 
        ~Camera();
        
        /**
         * @brief Initializes the camera. 
         * @details This function tries to initialize the camera. 
         */
        UInt32_t init(void); 

};

#endif  /* DEF_CAMERA_HPP */

