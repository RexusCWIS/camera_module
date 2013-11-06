/**
 * @file image_test.cpp
 * @brief Image class unit tests. 
 */

#include "gtest/gtest.h"
#include "image.hpp"

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

bool isPNG(const char *pngFile); 

class ImageTest : public testing::Test {
    
    protected:  
        virtual void SetUp() {
            
            i_ = new Image(800, 600);
            char *buffer = i_->getImageBuffer();
            
            for (unsigned int r = 0; r < 600; r++) {
                
                for (unsigned c = 0; c < 800; c++) {
                    
                    buffer[(r* 800) + c] = (char) (rand() & 0xFF);
                }
            }
        }

        virtual void TearDown() {
            delete i_; 
        }

    Image *i_; 
        
};

TEST(Image, Constructors) {

    Image *i = new Image(800u, 600u); 
    EXPECT_EQ(i->getWidth(), 800u); 
    EXPECT_EQ(i->getHeight(), 600u);  
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

