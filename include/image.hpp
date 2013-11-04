/**
 * @file image.hpp
 * @brief Image class definition. 
 */

#ifndef DEF_IMAGE_HPP
#define DEF_IMAGE_HPP

#include <stdlib.h>

/** @brief Modular type for the pixel size. */
typedef char pixel_t; 

/**
 * @brief Image class.
 * @details This class defines the behaviour of an image object. 
 *          It allocates memory for an image of the given size 
 *          and provides methods to save the image in different
 *          file formats. 
 * @todo Use template to define the pixel bit size. 
 */ 
class Image {

    public: 
        Image(unsigned int width, unsigned int height);
        pixel_t *getImageBuffer(void) const;
        unsigned int getWidth(void) const; 
        unsigned int getHeight(void) const;

        void writeToPNG(const char *filename, char *title = NULL);
        size_t writeToPGM(const char *filename); 
        ~Image(); 

    private:
        /** @brief Image width. */
        unsigned int i_width; 
        /** @brief Image height. */
        unsigned int i_height;
        /** @brief Number of bytes required to store a single pixel. */
        
        /** @brief Memory buffer holding the actual image. */
        pixel_t *i_buffer; 
};


#endif  /* DEF_IMAGE_HPP */

