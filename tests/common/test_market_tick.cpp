//
// Created by Arjun Varma on 09/09/25.
//

#include <gtest/gtest.h>
#include "MarketTick.hpp"

TEST(MarketTickTest, SizeAndAlignment) {
    EXPECT_EQ(sizeof(MarketTick), 36); // packed struct
    EXPECT_EQ(alignof(MarketTickAligned), 64);
}

TEST(MarketTickTest, FieldAssignment) {
    MarketTick tick{};
    tick.produce_timestamp = 123;
    strcpy(tick.symbol, "AAPL");
    tick.price = 150.5;
    tick.volume = 100;

    EXPECT_EQ(tick.produce_timestamp, 123);
    EXPECT_STREQ(tick.symbol, "AAPL");
    EXPECT_DOUBLE_EQ(tick.price, 150.5);
    EXPECT_EQ(tick.volume, 100u);
}
