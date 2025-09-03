//
// Created by Arjun Varma on 30/08/25.
//

#ifndef MARKETTICK_HPP
#define MARKETTICK_HPP

#include <cstdint>
#include <chrono>

#pragma pack(push, 1)
struct MarketTick {
    uint64_t timestamp;   // 8 bytes (nanoseconds since epoch)
    char symbol[8];       // 8 bytes
    double price;         // 8 bytes
    uint32_t volume;      // 4 bytes
    uint64_t send_timestamp;     // 8 bytes (send timestamp in nanoseconds)
    // total = 36 bytes
};
#pragma pack(pop)

// Cache-line aligned version for queues/batching
struct alignas(64) MarketTickAligned {
    MarketTick tick;
    char padding[64 - (sizeof(MarketTick) % 64)];
};

#endif //MARKETTICK_HPP
