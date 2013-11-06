/**
 * @file utilities_test.cpp
 * @brief Utilities unit tests. 
 */

#include "utilities.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(UtilitiesTest, SimpleExtensions) {

    std::string bmpFile = "bmp_image.bmp";
    std::string pngFile = "png_image.png";
    std::string pgmFile = "pgm_image.pgm";

    EXPECT_EQ(std::strcmp(getFileExtension(bmpFile), "bmp"); 
}

