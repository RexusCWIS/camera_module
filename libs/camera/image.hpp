/**
 * @file image.hpp
 * @brief Image class definition. 
 */

#ifndef DEF_IMAGE_HPP
#define DEF_IMAGE_HPP

#include <stdlib.h>
#include <string>

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
        /** 
         * @brief Creates an image of given @p width and @p height.
         * @details Allocates memory for the image and locks it in RAM to prevent 
         *          the allocated memory to be swapped back to the hard drive. 
         */
        Image(unsigned int width, unsigned int height);

        /**
         * @brief Returns the image buffer. 
         * @details The image buffer is the memory array allocated by the constructor. 
         *          This function provides a direct R/W access to this array for efficient
         *          image acquisition by a camera. 
         */
        pixel_t *getImageBuffer(void) const;

        /** 
         * @brief Returns the image width. 
         * @see getHeight()
         */
        unsigned int getWidth(void) const; 

        /**
         * @brief Returns the image height.
         * @see getWidth()
         */
        unsigned int getHeight(void) const;

        /**
         * @brief Writes the image to a PNG file. 
         * @param[in]   filename    Name of the PNG file.  
         * @param[in]   title       Title of the PNG file. 
         * @see writeToPGM()
         */
        void writeToPNG(const char *filename, char *title = NULL);

        /**
         * @brief Writes the image to a PGM file. 
         * @param[in]   filename    Name of the PGM file.   
         * @see writeToPNG()
         */ 
        size_t writeToPGM(const char *filename);

        /**
         * @brief Specifies if the image is currently being written to disk. 
         * @returns @p true if the image is currently being written. 
         */
        bool isBeingWritten(void); 

        /**
         * @brief Frees the image memory and unlocks it from RAM. 
         */
        ~Image(); 

    private:
        /** @brief Image width. */
        unsigned int i_width; 
        /** @brief Image height. */
        unsigned int i_height;

        unsigned int i_size; 
        /** @brief Number of bytes required to store a single pixel. */
 
        std::string i_pgmHeader;

        /** @brief Memory buffer holding the actual image. */
        pixel_t *i_buffer;
    
        /** @brief Indicates if the image buffer is currently being written by a process to persistent memory. */
        bool i_isBeingWritten; 
};


#endif  /* DEF_IMAGE_HPP */

