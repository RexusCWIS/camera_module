/** 
 * @file image.cpp
 * @brief Image class implementation. 
 */

#include "image.hpp"
#include <sys/mman.h>

Image::Image(unsigned int width, unsigned int height) : 
            i_width(width), i_height(height) {
 
    this->i_buffer = new pixel_t[width * height];
}

Image::~Image() {

    delete [] this->i_buffer; 
}

