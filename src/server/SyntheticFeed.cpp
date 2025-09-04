//
// Created by Arjun Varma on 30/08/25.
//

#include "server/SyntheticFeed.hpp"

SyntheticFeed::SyntheticFeed(const char* sym, double start_price)
        : rng(std::random_device{}()),
          price_change(0.0, 0.5),
          vol_dist(10, 1000),
          current_price(start_price),
          symbol(sym) {}

MarketTick SyntheticFeed::next_tick() {
    current_price += price_change(rng);
    if (current_price < 1.0) current_price = 1.0;

    MarketTick tick{};
    std::strncpy(tick.symbol, symbol, sizeof(tick.symbol));
    tick.price = current_price;
    tick.volume = vol_dist(rng);

    return tick;
}