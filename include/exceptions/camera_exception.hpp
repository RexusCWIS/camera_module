/**
 * @file camera_exception.hpp
 * @brief Generic camera exception interface. 
 */

#ifndef DEF_CAMERA_EXCEPTION_HPP
#define DEF_CAMERA_EXCEPTION_HPP

#include <string>
#include <exception>

using namespace std; 

/**
 * @brief Generic camera exception.
 * @details This abstract class is the interface for all camera exceptions. 
 *          A specific exception class should be defined for each camera type to
 *          account for differences in camera and issue identifications. 
 */
class CameraException: public exception {

    public: 
        /**
         * @brief Basic exception constructor. Initializes the message string.
         */
        CameraException(string msg) : 
            e_msg(msg) {}

        /**
         * @brief Returns a string describing the exception.
         */
        virtual const char* what() const throw() {
            return e_msg.c_str(); 
        }

        virtual ~CameraException() throw() {
        } 

    protected: 
        /** @brief Exception message. */
        string e_msg; 
};

#endif  /* DEF_CAMERA_EXCEPTION_HPP */

