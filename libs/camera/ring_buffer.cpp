/**
 * @file ring_buffer.cpp
 * @brief Ring buffer class implementation.
 */

#include "ring_buffer.hpp"

RingBuffer::RingBuffer(unsigned int width, unsigned int height, size_t nbOfImages) :
        m_size(nbOfImages), m_width(width), m_height(height) {

    this->m_imageArray = new Image *[nbOfImages]; 
    
    for(unsigned int incr = 0; incr < nbOfImages; incr++) {
        this->m_imageArray[incr] = new Image(width, height); 
        this->m_bufferToImage[this->m_imageArray[incr]->getImageBuffer()] = 
                this->m_imageArray[incr];
    }
}

RingBuffer::~RingBuffer() {

    for(unsigned int incr = 0; incr < this->m_size; incr++) {
        delete m_imageArray[incr];
        m_imageArray[incr] = NULL;
    }

    delete [] this->m_imageArray; 
    m_imageArray = NULL; 
}

size_t RingBuffer::getSize(void) const {
    return this->m_size; 
}

Image * RingBuffer::at(const size_t index) const {
    
    if(index < this->m_size) {
        return m_imageArray[index];
    }

    return NULL; 
}

Image * RingBuffer::getImageFromBuffer(char * const buffer) const {

    std::map<char * const, Image *>::const_iterator iter; 

    iter = this->m_bufferToImage.find(buffer);
    
    if(iter != this->m_bufferToImage.end()) {
        return iter->second; 
    }

    return NULL; 
}

Image*& RingBuffer::operator[] (const size_t index) {

    return m_imageArray[index]; 
}

