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

