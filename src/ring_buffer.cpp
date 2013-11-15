/**
 * @file ring_buffer.cpp
 * @brief Ring buffer class implementation.
 */

#include "ring_buffer.hpp"

RingBuffer::RingBuffer(size_t nbOfImages) :
        m_ringBufferSize(nbOfImages) {

    this->m_imageArray = new Image *[nbOfImages];  
}

RingBuffer::~RingBuffer() {
    
    delete [] this->m_imageArray; 
    m_imageArray = NULL; 
}

