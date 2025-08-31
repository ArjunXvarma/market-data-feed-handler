//
// Created by Arjun Varma on 31/08/25.
//

#include "client/VWAPCalculator.hpp"
void VWAPCalculator::update(const MarketTick& tick) {
    state_.add(tick.price, tick.volume);
}

double VWAPCalculator::getVWAP() const {
    return state_.getVWAP();
}

void VWAPCalculator::reset() {
    state_.reset();
}