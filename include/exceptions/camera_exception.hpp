/**
 * @file camera_exception.hpp
 * @brief Generic camera exception interface. 
 */

#ifndef DEF_CAMERA_EXCEPTION_HPP
#define DEF_CAMERA_EXCEPTION_HPP

#include <string>
#include <exception>

using namespace std; 

class CameraException: public exception {

    public:     
        CameraException(string msg) : 
            e_msg(msg) {}
        virtual const char* what() const throw() {
            return e_msg.c_str(); 
        }

        virtual ~CameraException() throw() {
        } 

    protected: 
        string e_msg; 
};

#endif  /* DEF_CAMERA_EXCEPTION_HPP */

