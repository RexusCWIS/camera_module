/**
 * @file image_test.cpp
 * @brief Image class unit tests. 
 */

#include "gtest/gtest.h"
#include "image.hpp"

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH     (800u)
#define IMAGE_HEIGHT    (600u)

bool isPNG(const char *pngFile);

class ImageTest : public testing::Test {
    
    protected:  
        virtual void SetUp() {
            
            i_ = new Image(IMAGE_WIDTH, IMAGE_HEIGHT);
            char *buffer = i_->getImageBuffer();
            
            for (unsigned int r = 0; r < IMAGE_HEIGHT; r++) {
                
                for (unsigned c = 0; c < IMAGE_WIDTH; c++) {
                    
                    buffer[(r * IMAGE_WIDTH) + c] = (char) (rand() & 0xFF);
                }
            }
        }

        char* readPNG(const char *pngFile, unsigned int &width, unsigned int &height); 

        virtual void TearDown() {
            delete i_; 
        }

    Image *i_; 
        
};

TEST_F(ImageTest, Constructors) {

    EXPECT_EQ(i_->getWidth(), IMAGE_WIDTH); 
    EXPECT_EQ(i_->getHeight(), IMAGE_HEIGHT);  
}

TEST_F(ImageTest, SaveAsPNG) {

    i_->writeToPNG("ImageTest.png");
    ASSERT_EQ(isPNG("ImageTest.png"), true);
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}

bool isPNG(const char *pngFile) {

    unsigned char pngHeader[8]; 
    FILE *fp = fopen(pngFile, "rb"); 
    if(fp == NULL) {
        return false; 
    }

    fread(pngHeader, 1, 8, fp); 
    fclose(fp);
    
    return(!png_sig_cmp(pngHeader, 0, 8)); 
}


char* ImageTest::readPNG(const char *pngFile, unsigned int &width, unsigned int &height) {

    png_byte pngHeader[8]; 
    png_structp png_ptr; 
    png_infop info_ptr;

    int bitDepth, colorType; 

    char *image; 

    FILE *fp = fopen(pngFile, "rb");

    if(fp == NULL) {
        return NULL; 
    }
    
    fread(pngHeader, 1, sizeof(pngHeader), fp);
    if(!png_check_sig(pngHeader, sizeof(pngHeader))) {

        fclose(fp); 
        return NULL; 
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
    if(!png_ptr) {
        return NULL; 
    }

    info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL); 
        return NULL;
    }

    png_init_io(png_ptr, fp); 

    png_set_sig_bytes(png_ptr, sizeof(pngHeader)); 
 
    bitDepth  = png_get_bit_depth(png_ptr, info_ptr); 
    colorType = png_get_color_type(png_ptr, info_ptr); 

    ASSERT_EQ(bitDepth, 8);

    png_get_IHDR(png_ptr, info_ptr, (png_uint_32*) &width, (png_uint_32*) &height,
                 NULL, NULL, NULL, NULL, NULL);

    image = new char[width * height]; 
    
    png_destroy_read_struct(&png_ptr, NULL, NULL); 
    fclose(fp); 
}

