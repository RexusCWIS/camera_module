/**
 * @file ring_buffer_test.cpp
 * @brief RingBuffer class unit tests.
 * @addtogroup unit_tests
 * @{
 */

#include "ring_buffer.hpp"
#include "gtest/gtest.h"
#include <stdlib.h>

/** @brief Size of the ring buffer. */
#define RING_BUFFER_SIZE    (27u)
/** @brief Width of images in the ring buffer. */
#define IMAGE_WIDTH         (800u)
/** @brief Height of images in the ring buffer. */
#define IMAGE_HEIGHT        (600u)

/**
 * @brief Fixture class for the RingBuffer class tests. 
 */
class RingBufferTest : public testing::Test {
    
    protected: 
        /** @brief Sets up the fixture class. */
        virtual void SetUp() {

            rb_ = new RingBuffer(IMAGE_WIDTH, IMAGE_HEIGHT, RING_BUFFER_SIZE); 
        }

        /** @brief Tears down the fixture class. */
        virtual void TearDown() {
            delete rb_; 
        }
    /** @brief Test ring buffer object. */
    RingBuffer *rb_; 
};

/** 
 * @brief Tests the constructors of the RingBuffer class. 
 */
TEST_F(RingBufferTest, Constructors) {

    EXPECT_EQ(rb_->getSize(), RING_BUFFER_SIZE); 
}

/** 
 * @brief Tests the accessors of the RingBuffer class. 
 * @details Tries to access elements in the ring buffer by index
 *          and checks that the result is valid. 
 */
TEST_F(RingBufferTest, Accessors) {

    size_t rbSize = rb_->getSize(); 
    ASSERT_EQ(rb_->at(rbSize + 1), (Image *) NULL); 

    for(unsigned int incr = 0; incr < rbSize; incr++) {
        ASSERT_NE((rb_->at(incr), (Image *) NULL)); 
        ASSERT_EQ((*rb_)[incr], rb_->at(incr));  
    }
}

/**
 * @brief Tests the mapping between memory buffers and Image object instances. 
 */
TEST_F(RingBufferTest, MemoryBuffers) {

    Image *i;

    for(unsigned int incr = 0; incr < rb_->getSize(); incr++) {
        i = (*rb_)[incr]; 
        EXPECT_EQ(rb_->getImageFromBuffer(i->getImageBuffer()), i); 
    }
}

/** @} */

