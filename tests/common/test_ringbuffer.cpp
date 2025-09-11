//
// Created by Arjun Varma on 09/09/25.
//

#include <gtest/gtest.h>
#include "RingBuffer.hpp"

TEST(RingBufferTest, PushPopSingle) {
    LockFreeRingBuffer<int> buf(4);
    EXPECT_TRUE(buf.push(42));
    int value;
    EXPECT_TRUE(buf.pop(value));
    EXPECT_EQ(value, 42);
}

TEST(RingBufferTest, BufferFullAndEmpty) {
    LockFreeRingBuffer<int> buf(2);
    EXPECT_TRUE(buf.push(1));
    EXPECT_TRUE(buf.push(2));
    EXPECT_FALSE(buf.push(3)); // full

    int val;
    EXPECT_TRUE(buf.pop(val));
    EXPECT_TRUE(buf.pop(val));
    EXPECT_FALSE(buf.pop(val)); // empty
}

TEST(RingBufferTest, WrapAround) {
    LockFreeRingBuffer<int> buf(3);
    buf.push(1);
    buf.push(2);
    int val;
    buf.pop(val);
    buf.push(3);
    buf.push(4);
    EXPECT_EQ(buf.size(), 3);
}
