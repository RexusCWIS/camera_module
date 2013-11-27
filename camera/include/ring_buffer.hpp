/**
 * @file ring_buffer.hpp
 * @brief Image acquisition ring buffer class definition. 
 */

#ifndef DEF_RING_BUFFER_HPP
#define DEF_RING_BUFFER_HPP

#include "image.hpp"
#include <map>

/**
 * @brief Ring buffer for image acquisition.
 * @details A ring buffer is a circular buffer holding image memory objects
 *          for acquisition. Each frame acquired by the acquisition device
 *          is stored sequentially in the buffer and overwrites images stored
 *          during the previous ring buffer walk. 
 */
class RingBuffer {
    public:
        /**
         * @brief Builds a ring buffer of the given size. 
         * @details Allocates memory for the given number of images and sets
         *          the mapping between image buffers and the Image objects.
         * @param[in]   nbOfImages  Number of images in the ring buffer. 
         * @note Allocated memory is freed in @ref ~RingBuffer().
         * @see ~RingBuffer()
         */
        RingBuffer(unsigned int width, unsigned int height, size_t nbOfImages); 

        /**
         * @brief Tears down a ring buffer object.
         * @details Frees the memory allocated for image objects.
         */
        ~RingBuffer();

        /** @brief Returns the number of images in the ring buffer. */
	    size_t getSize(void) const;
   
        Image * at(const size_t index) const;
        Image * getImageFromBuffer(char * const buffer) const; 
        /** @brief Returns a pointer to the image at the given @p index. */
        Image*& operator[] (const size_t index); 

    private:
        /** @brief Image objects array. Holds the ring buffer data. */
        Image **m_imageArray;

        /** @brief Size of the ring buffer, in images. */
        size_t m_size;
        unsigned int m_width; 
        unsigned int m_height; 

        /** @brief Maps an image buffer address to its corresponding Image object. */
        std::map<char * const, Image *> m_bufferToImage; 
};

#endif  /* DEF_RING_BUFFER_HPP */

