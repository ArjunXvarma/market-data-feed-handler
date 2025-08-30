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

class Client {
private:
    int socket_fd;
    sockaddr_in local_addr{};
    ip_mreq mreq{};
    std::string ip_;
    int port_;

public:
    Client(const std::string& ip, int port);
    void joinGroup();
    void receive();
    ~Client();
};


#endif //CLIENT_HPP
