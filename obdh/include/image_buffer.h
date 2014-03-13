/**
 * @file image_buffer.h
 * @brief Camera image buffer structure.
 */

#include <iostream>
#include <sys/mman.h>

struct image_buffer {
	
	image_buffer(unsigned int image_width, unsigned int image_height, 
		     unsigned int buffer_size) :
					width(image_width), height(image_height), 
					size(buffer_size) {
		
		buffer = new unsigned char*[size]();
		
		unsigned int image_size = width * height;
		
		for(unsigned int index = 0; index < size; index++) {
		
			try {	
			    buffer[index] = new unsigned char[image_size]();
			}

			catch(std::bad_alloc& e) {
			    std::cerr << "Allocation failure (" << index << " of " << size << ")" << std::endl;
			}

			mlock(buffer[index], image_size);
		}				
	}
	
	~image_buffer() {
		
		unsigned int image_size = width * height;
		
		for(unsigned int index = 0; index < size; index++) {
			munlock(buffer[index], image_size);
			delete [] buffer[index];
		}
		
		delete [] buffer;
	}
	
	unsigned char **buffer;
	unsigned int width;
	unsigned int height;
	unsigned int size;
};
