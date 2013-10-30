/**
 * @file ueye_exception.hpp
 * @brief Exceptions thrown by the uEye camera class.
 */

#ifndef DEF_UEYE_EXCEPTION_HPP
#define DEF_UEYE_EXCEPTION_HPP

#include <exception>
#include <uEye.h>

#include "types.hpp"
using namespace std;

class UEye_Exception: public exception {
    
    public: 
        UEye_Exception(INT id = IS_FAILURE, string const& message = "") throw() : 
            e_id(id), e_message(message) {}

        INT id() const throw() {    
            return e_id; 
        }

        virtual const char* what() const throw() {
            return e_message.c_str(); 
        }

        virtual ~UEye_Exception() throw() {
        }

    private: 
        INT e_id;
        string e_message; 

};

#endif  /* DEF_UEYE_EXCEPTION_HPP */

