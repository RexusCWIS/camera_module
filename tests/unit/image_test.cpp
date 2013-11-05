/**
 * @file image_test.cpp
 * @brief Image class unit tests. 
 */

#include "gtest/gtest.h"
#include "image.hpp"

TEST(ImageTest, Constructors) {
    Image *i = new Image(800u, 600u); 
    EXPECT_EQ(i->getWidth(), 800u); 
    EXPECT_EQ(i->getHeight(), 600u);  
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS(); 
}

