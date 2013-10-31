/**
 * @file image.hpp
 * @brief Image class definition. 
 */

#ifndef DEF_IMAGE_HPP
#define DEF_IMAGE_HPP

/** @brief Modular type for the pixel size. */
typedef char pixel_t; 

class Image {

    public: 
        Image(unsigned int width, unsigned int height);
        ~Image(); 

    private:
        /** @brief Image width. */
        unsigned int i_width; 
        /** @brief Image height. */
        unsigned int i_height;
        /** @brief Number of bytes required to store a single pixel. */
        
        /** @brief 2D memory buffer holding the actual image. */
        pixel_t **i_buffer; 
};


#endif  /* DEF_IMAGE_HPP */

