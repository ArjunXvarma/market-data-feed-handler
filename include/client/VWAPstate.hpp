//
// Created by Arjun Varma on 31/08/25.
//

#ifndef VWAPSTATE_HPP
#define VWAPSTATE_HPP


struct VWAPState {
    double pv = 0.0;
    double v = 0.0;

    void add(const double price, const double volume) {
        pv += price * volume;
        v  += volume;
    }

    void remove(const double price, const double volume) {
        pv -= price * volume;
        v  -= volume;
    }

    double getVWAP() const {
        return (v > 0.0) ? (pv / v) : 0.0;
    }

    void reset() {
        pv = 0.0;
        v  = 0.0;
    }
};


#endif //VWAPSTATE_HPP
