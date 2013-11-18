/**
 * @file ring_buffer_test.cpp
 * @brief RingBuffer class unit tests.
 * @addtogroup unit_tests
 * @{
 */

#include "ring_buffer.hpp"
#include "gtest/gtest.h"
#include <stdlib.h>

#define RING_BUFFER_SIZE    (27u)
#define IMAGE_WIDTH         (800u)
#define IMAGE_HEIGHT        (600u)

class RingBufferTest : public testing::Test {
    
    protected: 
        virtual void SetUp() {

            rb_ = new RingBuffer(IMAGE_WIDTH, IMAGE_HEIGHT, RING_BUFFER_SIZE); 
        }

        virtual void TearDown() {
            delete rb_; 
        }

    RingBuffer *rb_; 
};

TEST_F(RingBufferTest, Constructors) {

    EXPECT_EQ(rb_->getSize(), RING_BUFFER_SIZE); 
}

TEST_F(RingBufferTest, Accessors) {

    size_t rbSize = rb_->getSize(); 
    ASSERT_EQ(rb_->at(rbSize + 1), (Image *) NULL); 

    for(unsigned int incr = 0; incr < rbSize; incr++) {
        ASSERT_EQ((*rb_)[incr], rb_->at(incr));  
    }
}

TEST_F(RingBufferTest, MemoryBuffers) {

    Image *i;

    for(unsigned int incr = 0; incr < rb_->getSize(); incr++) {
        i = (*rb_)[incr]; 
        EXPECT_EQ(rb_->getImageFromBuffer(i->getImageBuffer()), i); 
    }
}

/** @} */

