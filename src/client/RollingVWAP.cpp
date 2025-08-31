//
// Created by Arjun Varma on 31/08/25.
//

#include "client/RollingVWAP.hpp"

RollingVWAP::RollingVWAP(std::chrono::seconds windowSize)
        : windowSize(windowSize) {}

void RollingVWAP::update(const MarketTick& tick) {
    const auto now = std::chrono::milliseconds(tick.timestamp);

    ticks.push_back(tick);
    state.add(tick.price, tick.volume);

    while (!ticks.empty()) {
        auto oldest = std::chrono::milliseconds(ticks.front().timestamp);
        if (now - oldest > windowSize) {
            state.remove(ticks.front().price, ticks.front().volume);
            ticks.pop_front();
        } else {
            break;
        }
    }
}

double RollingVWAP::getVWAP() const {
    return state.getVWAP();
}
