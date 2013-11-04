/**
 * @file ueye_exception.hpp
 * @brief Exceptions thrown by the uEye camera class.
 */

#ifndef DEF_UEYE_EXCEPTION_HPP
#define DEF_UEYE_EXCEPTION_HPP

#include "exceptions/camera_exception.hpp"
#include "types.hpp"
#include <uEye.h>

using namespace std;

class UEye_Exception: public CameraException {
    
    public: 
        UEye_Exception(HIDS camID, INT error = IS_NO_SUCCESS, string const& msg = "") throw() : 
            CameraException(msg), e_camID(camID), e_id(error) {
            }

        HIDS camera() const throw() {
            return e_camID; 
        }

        INT id() const throw() {    
            return e_id; 
        }

        virtual ~UEye_Exception() throw() {
        }

    protected: 
        HIDS e_camID; 
        INT e_id;

};

#endif  /* DEF_UEYE_EXCEPTION_HPP */

