cmake_minimum_required(VERSION 3.22)
project(market_data_feed_handler VERSION 0.1.0 LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(market_data_feed_handler
        src/main.cpp)