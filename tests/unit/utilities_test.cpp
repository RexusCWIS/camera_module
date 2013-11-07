/**
 * @file utilities_test.cpp
 * @brief Utilities unit tests.
 * @addtogroup unit_tests
 * @{
 */

#include "utilities.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(FileExtensionTest, Simple) {

    std::string bmpFile = "bmp_image.bmp";
    std::string pngFile = "png_image.png";
    std::string pgmFile = "pgm_image.pgm";

    ASSERT_STREQ(getFileExtension(bmpFile).c_str(), "bmp"); 
    ASSERT_STREQ(getFileExtension(pngFile).c_str(), "png"); 
    ASSERT_STREQ(getFileExtension(pgmFile).c_str(), "pgm"); 
}

TEST(FileExtensionTest, MultipleDots) {

    std::string file = "i.screw.with.dots.cpp"; 

    ASSERT_STREQ(getFileExtension(file).c_str(), "cpp"); 
}

TEST(FileExtensionTest, NonAsciiCharacters) {

    std::string file = "évaluation~numérique.pdf"; 
    EXPECT_STREQ(getFileExtension(file).c_str(), "pdf"); 
}

/** @} */

