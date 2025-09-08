cmake_minimum_required(VERSION 3.22)
project(market_data_feed_handler VERSION 0.1.0 LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Server executable
add_executable(server
        src/server.cpp
        src/server/Server.cpp
        include/MarketTick.hpp
        src/server/SyntheticFeed.cpp
        include/server/SyntheticFeed.hpp
        include/RingBuffer.hpp
)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Client executable
add_executable(client
        src/client.cpp
        src/client/Client.cpp
        include/client/Client.hpp
        include/MarketTick.hpp
        include/RingBuffer.hpp
        include/client/VWAPstate.hpp
        src/client/RollingVWAP.cpp
        include/client/RollingVWAP.hpp
        src/client/VWAPCalculator.cpp
        include/client/VWAPCalculator.hpp
)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
