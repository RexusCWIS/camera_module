/**
 * @file image_buffer.cpp
 * @brief Image buffer structure functions implementation.
 */

#include "image_buffer.h"

#include <stdio.h>
#include <sstream>

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
	
	/* Prepare the PGM header */
    std::stringstream header_stream;
    header_stream << "P5\n# COMMENT\n" << width << " " << height  << "\n255\n";
    
    const std::string header = header_stream.str();

	for(unsigned int index = 0; index < size; index++) {
		
		std::stringstream filename_stream;
		filename_stream << directory << "/image" << index << ".pgm";
		
		const char* filename = filename_stream.str().c_str();
    	FILE *fp = fopen(filename, "wb"); 
    	if (fp == NULL) {
        	/** @todo Throw file exception */
    	}
    
    	/* PGM file header */
    	fwrite(header.c_str(), sizeof(char), header.size(), fp); 

    	/* Write the actual image data */
    	fwrite(images[index].buffer, sizeof(char), width * height, fp); 

    	fclose(fp);
    
	}
}
