//
// Created by Arjun Varma on 29/08/25.
//

#include <iostream>
#include <string>

#include "server/server.hpp"

int main() {
    Server server("239.255.0.1", 30001);
    server.run();

    return 0;
}
