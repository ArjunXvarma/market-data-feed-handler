//
// Created by Arjun Varma on 09/09/25.
//

#include <gtest/gtest.h>
#include "client/RollingVWAP.hpp"
#include "MarketTick.hpp"

TEST(VWAPTest, SimpleAdd) {
    RollingVWAP vwap(std::chrono::seconds(60));

    MarketTick t1{};
    t1.price = 100.0;
    t1.volume = 10;
    t1.produce_timestamp = 1;

    MarketTick t2{};
    t2.price = 200.0;
    t2.volume = 20;
    t2.produce_timestamp = 2;

    vwap.update(t1);
    vwap.update(t2);

    double expected = (100.0 * 10 + 200.0 * 20) / (10 + 20);
    EXPECT_NEAR(vwap.getVWAP(), expected, 1e-9);
}
