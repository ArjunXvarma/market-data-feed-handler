#include "server/Server.hpp"
#include <iostream>
#include <unistd.h>

Server::Server(const char* ip, int port)
    : socket_fd_(-1),
      ip_(ip),
      port_(port),
      feed_("AAPL", 150.0),
      running_(false),
      buffer_(1024) {
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd_ < 0)
        throw std::runtime_error("Error creating socket");

    server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
}

void Server::run() {
    running_ = true;
    producer_thread_ = std::thread(&Server::produce_loop, this);
    consumer_thread_ = std::thread(&Server::consume_loop, this);
}

void Server::stop() {
    running_ = false;
    if (producer_thread_.joinable()) producer_thread_.join();
    if (consumer_thread_.joinable()) consumer_thread_.join();
}

void Server::produce_loop() {
    while (running_) {
        MarketTick t = feed_.next_tick();
        while (!buffer_.push(t));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Server::consume_loop() {
    while (running_) {
        MarketTick t{};
        if (buffer_.pop(t)) {
            sendto(socket_fd_, &t, sizeof(t), 0,
                   (sockaddr*)&server_addr, sizeof(server_addr));
            std::cout << "Sent tick ts=" << t.timestamp
                      << " symbol=" << t.symbol
                      << " price=" << t.price
                      << " vol=" << t.volume
                      << std::endl;
        } else {
            std::this_thread::yield();
        }
    }
}

Server::~Server() {
    stop();
    if (socket_fd_ >= 0) {
        if (close(socket_fd_) < 0) {
            std::cerr << "Warning: failed to close socket_fd_\n";
        } else {
            std::cout << "Socket closed successfully\n";
        }
    }
}
