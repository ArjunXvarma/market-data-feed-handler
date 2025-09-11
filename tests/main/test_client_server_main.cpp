//
// Created by Arjun Varma on 10/09/25.
//

#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "server/Server.hpp"
#include "client/Client.hpp"

TEST(ClientServerTest, ContinuousStreamNoCrash) {
    constexpr int port = 31001;

    // Main branch server: only ip + port
    Server server("239.255.0.1", port);
    Client client("239.255.0.1", port);

    std::thread server_thread([&] { server.run(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::thread client_thread([&] {
        client.joinGroup();

        // Only receive for a short while
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        client.stop();
    });

    // Allow some streaming before stopping
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    server.stop();

    client_thread.join();
    server_thread.join();

    SUCCEED() << "Client and Server ran without crash on continuous stream";
}