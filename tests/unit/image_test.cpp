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

        virtual void TearDown() {
            delete i_; 
        }

    Image *i_; 
        
};

/**
 * @brief Tests the constructors of the image class. 
 */
TEST_F(ImageTest, Constructors) {

    EXPECT_EQ(i_->getWidth(), IMAGE_WIDTH); 
    EXPECT_EQ(i_->getHeight(), IMAGE_HEIGHT);  
}

/**
 * @brief Tests that the image can be saved properly as a PNG file. 
 * @details Opens the file and extracts data using libpng. 
 */ 
TEST_F(ImageTest, SaveAsPNG) {

    /* Store the image as a PNG file */
    i_->writeToPNG("ImageTest.png");

    /* Check that the file has the same name and can be opened */
    FILE *(fp) = fopen("ImageTest.png", "rb"); 
    ASSERT_NE(fp, (FILE *) NULL);

    /* Check that the file respects the PNG format */
    unsigned char pngHeader[8];
    fread(pngHeader, 1, 8, fp);
    ASSERT_FALSE(png_sig_cmp(pngHeader, 0, 8));

    /* Initialize libpng structures */
	png_structp png_ptr; 
	png_infop info_ptr;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	ASSERT_NE(png_ptr, (png_structp) NULL); 

	info_ptr = png_create_info_struct(png_ptr);
    ASSERT_NE(info_ptr, (png_infop) NULL); 

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sizeof(pngHeader)); 
    png_read_info(png_ptr, info_ptr); 

    /* Check metadata */
    png_uint_32 width, height; 
    int bitDepth, colorType; 
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *) &width, (png_uint_32 *) &height,
		         &bitDepth, &colorType, NULL, NULL, NULL);

    ASSERT_EQ(bitDepth, 8); 
    ASSERT_EQ(colorType, PNG_COLOR_TYPE_GRAY);
    ASSERT_EQ(i_->getWidth(), width); 
    ASSERT_EQ(i_->getHeight(), height); 
   
    /* Read the actual image */
    png_bytepp rowPtrs; 

    rowPtrs = new png_bytep[height * sizeof(png_bytepp)]; 
    
    for(unsigned int incr = 0; incr < height; incr++) {
        
        rowPtrs[incr] = new png_byte[width * sizeof(png_bytepp)];
    }

    png_read_image(png_ptr, rowPtrs);

    /* Compare to the image buffer */
    png_bytep imageBuffer = (png_bytep) i_->getImageBuffer(); 

    for(unsigned int r = 0; r < height; r++) {
        
        for(unsigned int c = 0; c < width; c++) {

            EXPECT_EQ(rowPtrs[r][c], imageBuffer[r * width + c]);
        }
    }

    for(unsigned int incr = 0; incr < height; incr++) {
        
        delete [] rowPtrs[incr]; 
    }
    delete [] rowPtrs; 

    fclose(fp); 
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1); 
    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL); 
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




