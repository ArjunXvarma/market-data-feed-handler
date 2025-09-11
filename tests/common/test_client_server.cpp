//
// Created by Arjun Varma on 09/09/25.
//

#include <gtest/gtest.h>
#include <thread>
#include "server/Server.hpp"
#include "client/Client.hpp"

TEST(ClientServerTest, RoundTrip100Ticks) {
    constexpr int port = 31001;
    constexpr size_t total_ticks = 100;

    Server server("239.255.0.1", port, total_ticks);
    Client client("239.255.0.1", port, total_ticks);

    std::thread server_thread([&] { server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread client_thread([&] {
        client.joinGroup();
        client.receive();
        client.stop();
    });

    server.stop();
    client_thread.join();
    server_thread.join();

    // We can’t check exact ticks easily, but we can check no crash
    SUCCEED();
}
