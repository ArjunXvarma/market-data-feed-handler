// Server.cpp
#include "server/Server.hpp"
#include <iostream>
#include <unistd.h>
#include <chrono>

Server::Server(const char* ip, int port, size_t total_ticks)
    : socket_fd_(-1),
      ip_(ip),
      port_(port),
      feed_("AAPL", 150.0),
      running_(false),
      buffer_(1 << 16),
      total_ticks_(total_ticks)
{
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd_ < 0)
        throw std::runtime_error("Error creating socket");

    constexpr int buf = 4 * 1024 * 1024; // 4mb
    setsockopt(socket_fd_, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf));

    server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
}

void Server::run() {
    running_ = true;
    start_time_ = std::chrono::high_resolution_clock::now();
    producer_thread_ = std::thread(&Server::produce_loop, this);
    consumer_thread_ = std::thread(&Server::consume_loop, this);
}

void Server::produce_loop() {
    size_t sent = 0;
    while (running_ && sent < total_ticks_) {
        MarketTick t = feed_.next_tick();
        auto now = std::chrono::high_resolution_clock::now();
        uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        t.send_timestamp = ns;
        MarketTickAligned aligned{};
        aligned.tick = t;
        while (!buffer_.push(aligned));
        sent++;
    }
    running_ = false;
}

void Server::consume_loop() {
    const auto start_time = std::chrono::high_resolution_clock::now();

    while (running_ || !buffer_.empty()) {
        MarketTickAligned aligned{};
        if (buffer_.pop(aligned)) {
            auto now = std::chrono::high_resolution_clock::now();
            aligned.tick.send_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
            sendto(socket_fd_, &aligned.tick, sizeof(MarketTick), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        }
    }

    end_time_ = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time_ - start_time;
    std::cout << "=== Server Stats ===\n"
              << "Ticks sent: " << total_ticks_ << "\n"
              << "Elapsed time: " << elapsed.count() << "s\n"
              << "Throughput: " << (total_ticks_ / elapsed.count()) << " ticks/sec\n";
}

void Server::stop() {
    running_ = false;
    if (producer_thread_.joinable()) producer_thread_.join();
    if (consumer_thread_.joinable()) consumer_thread_.join();
}

Server::~Server() {
    stop();
    if (socket_fd_ >= 0) {
        if (close(socket_fd_) < 0) std::cerr << "Warning: failed to close socket_fd_\n";
        else std::cout << "Socket closed successfully\n";
    }
}
