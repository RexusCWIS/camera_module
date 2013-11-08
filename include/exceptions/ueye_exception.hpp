/**
 * @file ueye_exception.hpp
 * @brief Exceptions thrown by the uEye camera class.
 * @addtogroup exceptions 
 * @{
 */

#ifndef DEF_UEYE_EXCEPTION_HPP
#define DEF_UEYE_EXCEPTION_HPP

#include "exceptions/camera_exception.hpp"
#include "types.hpp"
#include <uEye.h>

using namespace std;

/**
 * @brief Exception class for uEye cameras. 
 * @details The uEye camera class throws this exception class
 *          whenever it cannot access the connected camera properly.  
 */
class UEye_Exception: public CameraException {
    
    public:
        /**
         * @brief Exception constructor.
         * @details Creates a uEye exception item identifying the encountered error.
         * @param[in]   camID   ID of the camera.
         * @param[in]   error   Error code (from the uEye API specification).
         * @param[in]   msg     String that describes the exception.
         */
        UEye_Exception(HIDS camID, INT error = IS_NO_SUCCESS, string const& msg = "") throw() : 
            CameraException(msg), e_camID(camID), e_id(error) {
            }

        /**
         * @brief Returns the ID of the camera that generated the exception.
         */
        HIDS camera() const throw() {
            return e_camID; 
        }

        /** 
         * @brief Returns the error code of the exception.
         */
        INT id() const throw() {    
            return e_id; 
        }
        
        virtual ~UEye_Exception() throw() {
        }

    protected: 
        /** @brief ID of the camera that generated the fault status. */
        HIDS e_camID; 
        /** @brief Error code. */
        INT e_id;

};

#endif  /* DEF_UEYE_EXCEPTION_HPP */

/** @} */

