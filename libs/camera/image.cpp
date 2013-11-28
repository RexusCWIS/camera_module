/** 
 * @file image.cpp
 * @brief Image class implementation. 
 */

#include "image.hpp"
#include <png.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sstream>

Image::Image(unsigned int width, unsigned int height) : 
            i_width(width), i_height(height) {
 
    unsigned int imageSize = width * height; 
    this->i_isBeingWritten = false; 

    this->i_buffer = new pixel_t[imageSize];
    mlock(this->i_buffer, imageSize);

    /* Prepare the PGM header */
    std::stringstream ss;

    ss << "P5\n# COMMENT\n" << width << " " << height  << "\n255\n";

    std::string header = ss.str();

    this->i_pgmHeader = header.c_str();
    this->i_pgmHeaderSize = header.size(); 
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

void Image::writeToPNG(const char *filename, char *title) {

    FILE *fp; 

    /* PNG related pointers */
    png_structp png_ptr; 
    png_infop info_ptr; 
    png_bytep row;

    this->i_isBeingWritten = true;
    
    /* Open the file */
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        /** @todo Throw file exception */
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        /** @todo Throw PNG write struct exception */
    }

    info_ptr = png_create_info_struct(png_ptr); 
    if(info_ptr == NULL) {
        /** @todo Throw PNG info struct exception */
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        /** @todo Throw yet another exception */
    }

    png_init_io(png_ptr, fp);

    /* Write PNG header: 8-bit colour depth, black and white */
    png_set_IHDR(png_ptr, info_ptr, this->i_width, this->i_height, 
                 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 

    if(title != NULL) {
        png_text title_text; 
        title_text.compression = PNG_TEXT_COMPRESSION_NONE; 
        title_text.key = (png_charp) "Title"; 
        title_text.text = title; 
        png_set_text(png_ptr, info_ptr, &title_text, 1); 
    }

    png_write_info(png_ptr, info_ptr); 
    
    /* Allocate memory for one row */
    row = new png_byte[this->i_width];

    /* Write data one row at a time */
    for(unsigned int y = 0; y < this->i_height; y++) {

        for(unsigned int x = 0; x < this->i_width; x++) {
            row[x] = (png_byte) this->i_buffer[y * this->i_width + x]; 
        }
        png_write_row(png_ptr, row); 
    }
    
    png_write_end(png_ptr, NULL);

    fclose(fp);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1); 
    png_destroy_write_struct(&png_ptr, (png_infopp) NULL); 
    delete [] row; 
    
    this->i_isBeingWritten = false;
}

size_t Image::writeToPGM(const char *filename) {

    this->i_isBeingWritten = true;
    
    size_t bytes = 0; 
    FILE *fp = fopen(filename, "wb"); 
    if (fp == NULL) {
        /** @todo Throw file exception */
    }
    
    /* PGM file header.
     * The P5 indicator on the first line specifies the PGM format.
     */
    bytes += fwrite(this->i_pgmHeader, sizeof(char), this->i_pgmHeaderSize, fp); 

    /* Write the actual image data */
    bytes += fwrite(this->i_buffer, sizeof(pixel_t), this->i_width * this->i_height, fp); 

    fclose(fp);
    
    this->i_isBeingWritten = false;

    return bytes; 
}

bool Image::isBeingWritten(void) {
    
    return this->i_isBeingWritten; 
}

Image::~Image() {

    munlock(this->i_buffer, this->i_width * this->i_height); 
    delete [] this->i_buffer; 
}

