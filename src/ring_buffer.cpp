/**
 * @file ring_buffer.cpp
 * @brief Ring buffer class implementation.
 */

#include "ring_buffer.hpp"

RingBuffer::RingBuffer(size_t nbOfImages) :
        m_size(nbOfImages) {

    this->m_imageArray = new Image *[nbOfImages];  
}

RingBuffer::~RingBuffer() {
    
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

Image*& RingBuffer::operator[] (const size_t index) {

    return m_imageArray[index]; 
}

