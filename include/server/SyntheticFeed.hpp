//
// Created by Arjun Varma on 30/08/25.
//

#ifndef SYNTHETIC_FEED_HPP
#define SYNTHETIC_FEED_HPP

#include "MarketTick.hpp"
#include <random>
#include <chrono>
#include <cstring>

class SyntheticFeed {
private:
    std::mt19937 rng;
    std::normal_distribution<> price_change;
    std::uniform_int_distribution<uint32_t> vol_dist;

    double current_price;
    const char* symbol;

public:
    SyntheticFeed(const char* sym, double start_price = 100.0);

    MarketTick next_tick();
};

#endif // SYNTHETIC_FEED_HPP
