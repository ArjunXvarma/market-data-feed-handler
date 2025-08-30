//
// Created by Arjun Varma on 30/08/25.
//

#include "server/Server.hpp"

#include <thread>

Server::Server(const char *ip, int port) : socket_fd(-1), ip_(ip), port_(port) {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
        throw std::runtime_error("Error creating socket");

    server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
}

void Server::run() {
    int counter = 0;
    while (true) {
        std::string message = "Hello Multicast " + std::to_string(counter++);

        if (int sent = sendto(socket_fd, message.c_str(), message.size(), 0, (sockaddr*) &server_addr, sizeof(server_addr)); sent < 0)
            throw std::runtime_error("Error sending message");

        std::cout << "Sent: " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

Server::~Server() {
    if (socket_fd >= 0) {
        if (close(socket_fd) < 0) {
            // Optional: log error instead of throwing inside destructor
            std::cerr << "Warning: failed to close socket_fd\n";
        } else {
            std::cout << "Socket closed successfully\n";
        }
    }
}


