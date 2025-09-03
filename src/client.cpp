//
// Created by Arjun Varma on 29/08/25.
//

#include "client/Client.hpp"
#include <csignal>
#include <atomic>
#include <iostream>

std::atomic<bool> stop_flag{false};

void signal_handler(int) {
    stop_flag = true;
    std::cout << "\nReceived interrupt. Stopping client..." << std::endl;
}

int main() {
    std::signal(SIGINT, signal_handler);

    try {
        Client client("239.255.0.1", 30001);
        client.joinGroup();
        client.receive();

        while (!stop_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        client.stop();

    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
