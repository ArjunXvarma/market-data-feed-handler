//
// Created by Arjun Varma on 29/08/25.
//

#include "client/Client.hpp"

int main() {
    Client client("239.255.0.1", 30001);
    client.joinGroup();
    client.receive();

    return 0;
}
