//
// Created by Arjun Varma on 30/08/25.
//

#include "client/Client.hpp"

Client::Client(const std::string& ip, int port)
    : socket_fd_(-1),
      ip_(ip),
      port_(port),
      buffer_(1024),
      rolling_vwap_(std::chrono::seconds(60)) {

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

    if (bind(socket_fd_, (sockaddr*)&local_addr_, sizeof(local_addr_)) < 0)
        throw std::runtime_error("Error binding socket");
}

void Client::joinGroup() {
    mreq_.imr_multiaddr.s_addr = inet_addr(ip_.c_str());
    mreq_.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(socket_fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq_, sizeof(mreq_)) < 0)
        throw std::runtime_error("Error joining multicast group");

    std::cout << "Joined multicast group "
              << ip_ << ":" << port_ << std::endl;
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
    while (running_) {
        MarketTick tick{};
        if (buffer_.pop(tick)) {
            session_vwap_.update(tick);
            rolling_vwap_.update(tick);

            const double session_val = session_vwap_.getVWAP();
            const double rolling_val = rolling_vwap_.getVWAP();

            std::cout << "Tick: " << tick.symbol
                      << " Price: " << tick.price
                      << " Volume: " << tick.volume
                      << " Session VWAP: " << session_val
                      << " Rolling VWAP: " << rolling_val
                      << std::endl;
        } else {
            std::this_thread::yield(); // no data yet
        }
    }
}

void Client::produce_loop() {
    while (running_) {
        MarketTick tick{};
        int nbytes = recv(socket_fd_, &tick, sizeof(tick), 0);
        if (nbytes == sizeof(tick)) {
            buffer_.push(tick);
        }
    }
}

void Client::stop() {
    running_ = false;
    if (producer_thread_.joinable()) producer_thread_.join();
    if (consumer_thread_.joinable()) consumer_thread_.join();
}



