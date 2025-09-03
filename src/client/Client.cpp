//
// Created by Arjun Varma on 30/08/25.
//

#include "client/Client.hpp"
Client::Client(const std::string& ip, const int port)
    : socket_fd_(-1),
      ip_(ip),
      port_(port),
      buffer_(1024),
      rolling_vwap_(std::chrono::seconds(60)),
      total_ticks_(1000000) {
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd_ < 0)
        throw std::runtime_error("Error creating socket");

    int reuse = 1;
    if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0)
        throw std::runtime_error("Error setting SO_REUSEADDR");
    local_addr_ = {};
    local_addr_.sin_family = AF_INET;
    local_addr_.sin_port = htons(port_);
    local_addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    constexpr int buf = 4 * 1024 * 1024; // 4mb
    if (setsockopt(socket_fd_, SOL_SOCKET, SO_RCVBUF, &buf, sizeof(buf)) < 0) {
        throw std::runtime_error("Error setting SO_RCVBUF");
    }

    if (bind(socket_fd_, (sockaddr*)&local_addr_, sizeof(local_addr_)) < 0)
        throw std::runtime_error("Error binding socket");
}

void Client::joinGroup() {
    mreq_.imr_multiaddr.s_addr = inet_addr(ip_.c_str());
    mreq_.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq_, sizeof(mreq_)) < 0)
        throw std::runtime_error("Error joining multicast group"); std::cout << "Joined multicast group " << ip_ << ":" << port_ << std::endl;
}

void Client::receive() {
    running_ = true;
    producer_thread_ = std::thread(&Client::produce_loop, this);
    consumer_thread_ = std::thread(&Client::consume_loop, this);
}

Client::~Client() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
        std::cout << "Socket closed\n";
    }
}

void Client::consume_loop() {
    size_t received = 0;
    std::vector<double> latencies;
    latencies.reserve(total_ticks_);

    const auto start_time = std::chrono::high_resolution_clock::now();

    while (received < total_ticks_) {
        MarketTickAligned aligned{};
        if (buffer_.pop(aligned)) {
            auto now = std::chrono::high_resolution_clock::now();
            uint64_t recv_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

            double latency_us = static_cast<double>(recv_ns - aligned.tick.send_timestamp) / 1000.0;

            latencies.push_back(latency_us);
            ++received;
        } else {
            std::this_thread::yield();
        }
    }

    running_ = false;
    const auto end_time = std::chrono::high_resolution_clock::now();
    const double elapsed = std::chrono::duration<double>(end_time - start_time).count();

    std::sort(latencies.begin(), latencies.end());
    std::cout << "=== Client Stats ===\n"
              << "Ticks received: " << received << "\n"
              << "Elapsed time: " << elapsed << "s\n"
              << "Throughput: " << (received / elapsed) << " ticks/sec\n"
              << "Latency p50: " << latencies[latencies.size() / 2] << " us\n"
              << "Latency p99: " << latencies[latencies.size() * 99 / 100] << " us\n";
}

void Client::produce_loop() {
    while (running_) {
        MarketTick wire{};
        int nbytes = recv(socket_fd_, &wire, sizeof(wire), 0);
        if (nbytes == sizeof(wire)) {
            MarketTickAligned aligned{};
            aligned.tick = wire;
            buffer_.push(aligned);
        } else if (nbytes <= 0) {
            break;
        }
    }
}

void Client::stop() {
    running_ = false;
    if (producer_thread_.joinable()) producer_thread_.join();
    if (consumer_thread_.joinable()) consumer_thread_.join();
}