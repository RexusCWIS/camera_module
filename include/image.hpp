/**
 * @file image.hpp
 * @brief Image class definition. 
 */

#ifndef DEF_IMAGE_HPP
#define DEF_IMAGE_HPP

class Image {

    public: 
        Image(int width, int height);
        ~Image(); 

    private:
        /** @brief Image width. */
        int i_width; 
        /** @brief Image height. */
        int i_height;
        /** @brief Memory buffer holding the actual image. */
        unsigned char *i_buffer; 
};


#endif  /* DEF_IMAGE_HPP */

