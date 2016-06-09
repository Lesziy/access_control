/*
 * main.cpp
 *
 *  Created on: 4 maj 2016
 *      Author: secman
 */

#include "ServerApplication.h"

int main() {

    try {
        ServerApplication server;
        server.run();
    } catch(std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}

