/**
 * @file image_buffer.cpp
 * @brief Image buffer structure functions implementation.
 */

#include "image_buffer.h"

#include <png.h>

#include <stdio.h>
#include <sstream>

void image_buffer_item::save_to_png(const char* filename, int width, int height) {
    
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
                 
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    
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

image_buffer::image_buffer(unsigned int image_width, unsigned int image_height, 
		     			   unsigned int buffer_size) :
					width(image_width), height(image_height), 
					size(buffer_size) {
		
	images = new image_buffer_item[size]();
		
	unsigned int image_size = width * height;
		
	for(unsigned int index = 0; index < size; index++) {
		
		try {	
		    images[index].buffer = new char[image_size]();
		}

		catch(std::bad_alloc& e) {
		    std::cerr << "Allocation failure (" << index << " of " << size << ")" << std::endl;
		}
		mlock(images[index].buffer, image_size);
		
		images[index].time = 0;
	}
}

image_buffer::~image_buffer() {
	
	unsigned int image_size = width * height;
	
	for(unsigned int index = 0; index < size; index++) {
		munlock(images[index].buffer, image_size);
		delete [] images[index].buffer;
	}

	delete [] images;
}

void image_buffer::save_to_pgm(const char* directory) {

    for(unsigned int index = 0; index < size; index++) {

        /* Prepare the PGM header */
        std::stringstream header_stream;
        header_stream << "P5\n# " << images[index].time << "\n" << width << " " << height  << "\n255\n";
		
		std::stringstream filename_stream;
    	filename_stream << directory << "/image" << index << ".pgm";
        const char* filename = filename_stream.str().c_str();
        
        FILE *fp = fopen(filename, "wb");
        if (fp == NULL) {
            /** @todo Throw file exception */
    	}

    	/* PGM file header */
    	fwrite(header_stream.str().c_str(), sizeof(char), header_stream.str().size(), fp); 

    	/* Write the actual image data */
    	fwrite(images[index].buffer, sizeof(char), width * height, fp); 

    	fclose(fp);
    
	}
}

void image_buffer::save_to_png(const char* directory) {
	
	for(unsigned int index = 0; index < size; index++) {
	
		std::stringstream filename_stream;
    	filename_stream << directory << "/image" << index << ".png";
        const char* filename = filename_stream.str().c_str();
        
        images[index].save_to_png(filename, width, height);
	}
}

const char* image_buffer::get_image_filename(const char* directory, const char* format, unsigned int index) const {
	
	std::stringstream filename_stream;
    filename_stream << directory << "/image" << index << "." << format;
    
    return filename_stream.str().c_str();
}
