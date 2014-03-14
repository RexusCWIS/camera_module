/**
 * @file image_buffer.h
 * @brief Camera image buffer structure.
 */

#include <iostream>
#include <sys/mman.h>

struct image_buffer {
	
	image_buffer(unsigned int image_width, unsigned int image_height, 
		     unsigned int buffer_size);
	
	~image_buffer();
	
	void save_to_pgm(const char *directory);
	
	char **buffer;
	unsigned int width;
	unsigned int height;
	unsigned int size;
};
