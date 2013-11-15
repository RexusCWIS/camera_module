/**
 * @file ring_buffer.hpp
 * @brief Image acquisition ring buffer class definition. 
 */

#ifndef DEF_RING_BUFFER_HPP
#define DEF_RING_BUFFER_HPP

#include "image.hpp"
#include <map>

class RingBuffer {
    public: 
        RingBuffer(size_t nbOfImages); 
        ~RingBuffer();
	    size_t getSize(void) const;
        Image * at(const size_t index) const;

        Image*& operator[] (const size_t index); 

    private: 
        Image **m_imageArray;
        size_t m_size; 
        std::map<char *, Image *> m_bufferToImage; 

};

#endif  /* DEF_RING_BUFFER_HPP */

