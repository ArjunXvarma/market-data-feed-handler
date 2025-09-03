//
// Created by Arjun Varma on 29/08/25.
//

#include <iostream>
#include <csignal>
#include <atomic>
#include "server/Server.hpp"
#include "MarketTick.hpp"

std::atomic<bool> running(true);

void signal_handler(int) {
    running = false;
}

int main() {
    std::signal(SIGINT, signal_handler);

    try {
        constexpr int port = 30001;
        Server server("239.255.0.1", port, 1000000);
        server.run();

        std::cout << "Server running on " << port << std::endl;

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "Stopping server..." << std::endl;
        server.stop();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
