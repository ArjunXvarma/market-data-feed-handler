//
// Created by Arjun Varma on 30/08/25.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>
#include "SyntheticFeed.hpp"
#include "MarketTick.hpp"
#include "RingBuffer.hpp"

class Server {
private:
    void produce_loop();
    void consume_loop();

    int socket_fd_;
    sockaddr_in server_addr{};

    std::string ip_;
    int port_;
    SyntheticFeed feed_;

    std::thread producer_thread_;
    std::thread consumer_thread_;
    std::atomic<bool> running_;

    LockFreeRingBuffer<MarketTickAligned> buffer_;

    size_t total_ticks_;
    std::chrono::high_resolution_clock::time_point start_time_;
    std::chrono::high_resolution_clock::time_point end_time_;

public:
    Server(const char* ip, int port, size_t total_ticks);
    ~Server();

    void run();
    void stop();
};



#endif //SERVER_HPP
