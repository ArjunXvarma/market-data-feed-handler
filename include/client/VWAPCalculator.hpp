//
// Created by Arjun Varma on 31/08/25.
//

#ifndef VWAPCALCULATOR_HPP
#define VWAPCALCULATOR_HPP



#include "VWAPState.hpp"
#include "MarketTick.hpp"

class VWAPCalculator {
private:
    VWAPState state_;

public:
    void update(const MarketTick& tick);
    double getVWAP() const;
    void reset();
};



#endif //VWAPCALCULATOR_HPP
