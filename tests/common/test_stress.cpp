//
// Created by Arjun Varma on 09/09/25.
//

#include <gtest/gtest.h>
#include "RingBuffer.hpp"
#include <thread>

TEST(StressTest, RingBufferThroughput) {
    LockFreeRingBuffer<int> buf(1<<16);

    constexpr size_t N = 1'000'00;
    size_t pushed = 0, popped = 0;

    std::thread producer([&] {
        for (size_t i = 0; i < N; i++) {
            while (!buf.push(i)) std::this_thread::yield();
            pushed++;
        }
    });

    std::thread consumer([&] {
        int val;
        for (size_t i = 0; i < N; i++) {
            while (!buf.pop(val)) std::this_thread::yield();
            popped++;
        }
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(pushed, N);
    EXPECT_EQ(popped, N);
}
