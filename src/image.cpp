/** 
 * @file image.cpp
 * @brief Image class implementation. 
 */

#include "image.hpp"
#include <sys/mman.h>

Image::Image(unsigned int width, unsigned int height) : 
            i_width(width), i_height(height) {
 
    unsigned int imageSize = width * height; 

    this->i_buffer = new pixel_t[imageSize];
    mlock(this->i_buffer, imageSize); 
}

pixel_t * Image::getImageBuffer(void) const {

    return this->i_buffer; 
}

unsigned int Image::getWidth(void) const {
    
    return this->i_width; 
}

unsigned int Image::getHeight(void) const {

    return this->i_height; 
}

void Image::writeToPNG(char *filename, char *title) {

}


Image::~Image() {

    munlock(this->i_buffer, this->i_width * this->i_height); 
    delete [] this->i_buffer; 
}

