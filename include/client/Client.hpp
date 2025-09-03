//
// Created by Arjun Varma on 30/08/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#include "MarketTick.hpp"
#include "RingBuffer.hpp"
#include "RollingVWAP.hpp"
#include "VWAPCalculator.hpp"

class Client {
private:
    int socket_fd_;
    sockaddr_in local_addr_{};
    ip_mreq mreq_{};
    std::string ip_;
    int port_;
    size_t total_ticks_{};

    std::thread producer_thread_;
    std::thread consumer_thread_;
    std::atomic<bool> running_{false};

    LockFreeRingBuffer<MarketTickAligned> buffer_;
    VWAPCalculator session_vwap_;
    RollingVWAP rolling_vwap_;

    void produce_loop();
    void consume_loop();

public:
    Client(const std::string& ip, int port);
    void joinGroup();
    void receive();
    void stop();
    ~Client();
};

#endif // CLIENT_HPP
