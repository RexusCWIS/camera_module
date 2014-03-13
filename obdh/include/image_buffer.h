/**
 * @file image_buffer.h
 * @brief Camera image buffer structure.
 */

#include <sys/mman.h>

struct image_buffer {
	
	image_buffer(unsigned int width, unsigned int height, unsigned int size) :
					width(width), height(height), size(size) {
		
		buffer = new unsigned char*[size]();
		
		unsigned int image_size = width * height;
		
		for(unsigned int index = 0; index < size; index++) {
			
			buffer[index] = new unsigned char[image_size]();
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
