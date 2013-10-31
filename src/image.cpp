/** 
 * @file image.cpp
 * @brief Image class implementation. 
 */

#include "image.hpp"

Image::Image(unsigned int width, unsigned int height) : 
            i_width(width), i_height(height) {
 
    this->i_buffer = new pixel_t*[this->i_width];
    for(unsigned int incr = 0; incr < this->i_width; incr++) {
        
        this->i_buffer[incr] = new pixel_t[this->i_height]; 
    }
}

Image::~Image() {

    for(unsigned int incr = 0; incr < this->i_width; incr++) {
        
        delete [] this->i_buffer[incr]; 
    }

    delete [] this->i_buffer; 
}

