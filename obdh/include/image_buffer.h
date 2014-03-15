/**
 * @file image_buffer.h
 * @brief Camera image buffer structure.
 */

#include <iostream>
#include <sys/mman.h>

#ifndef DEF_IMAGE_BUFFER_H
#define DEF_IMAGE_BUFFER_H

struct image_buffer_item {
    char *buffer;
    unsigned int time;
    
    void save_to_png(const char* filename, int height, int width);
};

struct image_buffer {
	
	image_buffer(unsigned int image_width, unsigned int image_height, 
		     unsigned int buffer_size);
	
	~image_buffer();
	
	void save_to_pgm(const char* directory);
	
	void save_to_png(const char* directory);
	
	image_buffer_item* images;
	unsigned int width;
	unsigned int height;
	unsigned int size;

private:
	const char* get_image_filename(const char* directory, const char* format, unsigned int index) const;
};

#endif  /* DEF_IMAGE_BUFFER_H */

