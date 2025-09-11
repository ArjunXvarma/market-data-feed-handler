cmake_minimum_required(VERSION 3.14)
project(app_tests LANGUAGES CXX)

# -----------------------------
# C++ Standard
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# GoogleTest via FetchContent
# -----------------------------
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# -----------------------------
# Include Project Headers
# -----------------------------
include_directories(${CMAKE_SOURCE_DIR}/../include)

# -----------------------------
# Application Sources (no mains!)
# -----------------------------
set(APP_SOURCES
        ${CMAKE_SOURCE_DIR}/../src/server/Server.cpp
        ${CMAKE_SOURCE_DIR}/../src/server/SyntheticFeed.cpp
        ${CMAKE_SOURCE_DIR}/../src/client/Client.cpp
        ${CMAKE_SOURCE_DIR}/../src/client/RollingVWAP.cpp
        ${CMAKE_SOURCE_DIR}/../src/client/VWAPCalculator.cpp
)

# -----------------------------
# Common Tests
# -----------------------------
set(COMMON_TEST_SOURCES
        common/test_market_tick.cpp
        common/test_ringbuffer.cpp
        common/test_vwap.cpp
        common/test_client_server.cpp
        common/test_stress.cpp
)

# -----------------------------
# Branch Detection
# -----------------------------
if(NOT DEFINED BRANCH)
    # Auto-detect Git branch if not set
    execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

message(STATUS "Building tests for branch: ${BRANCH}")

# -----------------------------
# Branch-Specific Tests
# -----------------------------
set(BRANCH_TEST_SOURCES "")

if(BRANCH STREQUAL "main")
    list(APPEND BRANCH_TEST_SOURCES
            main/test_rolling_vwap.cpp
            main/test_client_server_main.cpp
    )
elseif(BRANCH STREQUAL "vanilla-server")
    list(APPEND BRANCH_TEST_SOURCES
            vanilla/test_latency.cpp
    )
elseif(BRANCH STREQUAL "batching-server")
    list(APPEND BRANCH_TEST_SOURCES
            batching/test_batching.cpp
    )
else()
    message(WARNING "Unknown branch type: ${BRANCH}. Only common tests will be built.")
endif()

# -----------------------------
# Final Test Sources
# -----------------------------
set(TEST_SOURCES
        ${COMMON_TEST_SOURCES}
        ${BRANCH_TEST_SOURCES}
        ${APP_SOURCES}
)

# -----------------------------
# Build Test Runner
# -----------------------------
add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests PRIVATE gtest gtest_main pthread)

add_test(NAME AllTests COMMAND run_tests)
