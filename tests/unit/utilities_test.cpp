/**
 * @file utilities_test.cpp
 * @brief Utilities unit tests. 
 */

#include "utilities.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(UtilitiesTest, Simple) {

    std::string bmpFile = "bmp_image.bmp";
    std::string pngFile = "png_image.png";
    std::string pgmFile = "pgm_image.pgm";

    ASSERT_EQ(strcmp(getFileExtension(bmpFile).c_str(), "bmp"), 0); 
    ASSERT_EQ(strcmp(getFileExtension(pngFile).c_str(), "png"), 0); 
    ASSERT_EQ(strcmp(getFileExtension(pgmFile).c_str(), "pgm"), 0); 
}

TEST(UtilitiesTest, MultipleDots) {

    std::string file = "i.screw.with.dots.cpp"; 

    ASSERT_EQ(strcmp(getFileExtension(file).c_str(), "cpp"), 0); 
}

