/**
 * @file ueye_exception.hpp
 * @brief Exceptions thrown by the uEye camera class.
 */

#ifndef DEF_UEYE_EXCEPTION_HPP
#define DEF_UEYE_EXCEPTION_HPP


#include "types.hpp"
#include <uEye.h>

#include <string>
#include <exception>
using namespace std;

class UEye_Exception: public exception {
    
    public: 
        UEye_Exception(HIDS camID, INT error = IS_NO_SUCCESS, string const& message = "") throw() : 
            e_camID(camID), e_id(error), e_message(message) {}

        HIDS camera() const throw() {
            return e_camID; 
        }

        INT id() const throw() {    
            return e_id; 
        }

        virtual const char* what() const throw() {
            return e_message.c_str(); 
        }

        virtual ~UEye_Exception() throw() {
        }

    private: 
        HIDS e_camID; 
        INT e_id;
        string e_message; 

};

#endif  /* DEF_UEYE_EXCEPTION_HPP */

