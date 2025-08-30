//
// Created by Arjun Varma on 30/08/25.
//

#include "server/Server.hpp"

#include <thread>

Server::Server(const char *ip, int port) : socket_fd(-1), ip_(ip), port_(port), feed(SyntheticFeed("AAPL", 150.0)) {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
        throw std::runtime_error("Error creating socket");

    server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
}

void Server::run() {
    while (true) {
        MarketTick t = feed.next_tick();
        std::cout << "Timestamp: " << t.timestamp
                  << " Symbol: " << t.symbol
                  << " Price: " << t.price
                  << " Volume: " << t.volume
                  << std::endl;
        sendto(socket_fd, &t, sizeof(t), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Sent market tick at timestamp: " << t.timestamp << std::endl;
    }
}

Server::~Server() {
    if (socket_fd >= 0) {
        if (close(socket_fd) < 0) {
            std::cerr << "Warning: failed to close socket_fd\n";
        } else {
            std::cout << "Socket closed successfully\n";
        }
    }
}


