cmake_minimum_required(VERSION 3.22)
project(market_data_feed_handler VERSION 0.1.0 LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(market_data_feed_handler
        src/main.cpp
)
target_include_directories(market_data_feed_handler PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Server executable
add_executable(server
        src/server.cpp
        src/server/Server.cpp
)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Client executable
add_executable(client
        src/client.cpp
        src/client/Client.cpp
        include/client/Client.hpp
)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
