cmake_minimum_required(VERSION 3.22)
project(market_data_feed_handler VERSION 0.1.0 LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client_perf
        src/client.cpp
        src/client/Client.cpp
        include/client/Client.hpp
        include/MarketTick.hpp
        include/RingBuffer.hpp
        include/client/VWAPstate.hpp
        src/client/RollingVWAP.cpp
        include/client/RollingVWAP.hpp
        src/client/VWAPCalculator.cpp
        include/client/VWAPCalculator.hpp)

add_executable(server_perf
        src/server.cpp
        src/server/Server.cpp
        include/server/Server.hpp
        include/MarketTick.hpp
        src/server/SyntheticFeed.cpp
        include/server/SyntheticFeed.hpp
        include/RingBuffer.hpp
        )
target_include_directories(server_perf PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(client_perf PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_definitions(client_perf PRIVATE ENABLE_PERF)
target_compile_definitions(server_perf PRIVATE ENABLE_PERF)

# Main executable
add_executable(market_data_feed_handler
        src/main.cpp
        include/MarketTick.hpp
        include/RingBuffer.hpp
        include/client/VWAPstate.hpp
        src/client/RollingVWAP.cpp
        include/client/RollingVWAP.hpp
)
target_include_directories(market_data_feed_handler PRIVATE ${CMAKE_SOURCE_DIR}/include)

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
