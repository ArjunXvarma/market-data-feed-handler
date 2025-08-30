//
// Created by Arjun Varma on 30/08/25.
//

#ifndef MARKETTICK_HPP
#define MARKETTICK_HPP

#include <cstdint>

#pragma pack(push, 1)
struct MarketTick {
    uint64_t timestamp;  // 8 bytes
    char symbol[8];      // 8 bytes
    double price;        // 8 bytes
    uint32_t volume;     // 4 bytes
};
#pragma pack(pop)

#endif //MARKETTICK_HPP
