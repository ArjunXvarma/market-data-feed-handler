//
// Created by Arjun Varma on 31/08/25.
//

#ifndef ROLLINGVWAP_HPP
#define ROLLINGVWAP_HPP



#include <deque>
#include <chrono>
#include "client/VWAPState.hpp"
#include "MarketTick.hpp"

class RollingVWAP {
private:
    std::deque<MarketTick> ticks;
    VWAPState state;
    std::chrono::milliseconds windowSize;

public:
    explicit RollingVWAP(std::chrono::seconds windowSize);

    void update(const MarketTick& tick);
    double getVWAP() const;
};



#endif //ROLLINGVWAP_HPP
