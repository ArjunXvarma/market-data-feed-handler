//
// Created by Arjun Varma on 30/08/25.
//

#include "client/Client.hpp"

Client::Client(const std::string& ip, int port)
    : socket_fd(-1), ip_(ip), port_(port) {

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
        throw std::runtime_error("Error creating socket");

    // Allow multiple clients on same machine/port
    int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0)
        throw std::runtime_error("Error setting SO_REUSEADDR");

    // Bind to port
    local_addr = {};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port_);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (sockaddr*)&local_addr, sizeof(local_addr)) < 0)
        throw std::runtime_error("Error binding socket");
}

void Client::joinGroup() {
    mreq.imr_multiaddr.s_addr = inet_addr(ip_.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0)
        throw std::runtime_error("Error joining multicast group");

    std::cout << "Joined multicast group "
              << ip_ << ":" << port_ << std::endl;
}

void Client::receive() {
    char buffer[1024];
    while (true) {
        int nbytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (nbytes < 0) {
            perror("recv");
            break;
        }
        buffer[nbytes] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
}

Client::~Client() {
    if (socket_fd >= 0) {
        close(socket_fd);
        std::cout << "Socket closed\n";
    }
}

