//
// Created by Arjun Varma on 30/08/25.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

#include "SyntheticFeed.hpp"
#include "MarketTick.hpp"

class Server {
private:
    int socket_fd;
    sockaddr_in server_addr;
    const char* ip_;
    int port_;
    SyntheticFeed feed;

public:
    Server(const char* ip, int port);
    void run();
    ~Server();
};



#endif //SERVER_HPP
