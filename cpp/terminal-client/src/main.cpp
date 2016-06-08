#include "client-application.h"

namespace argsvalidator {
    /**
     * Tests if passed string contains IPv4 readable
     * representation of ip address, ex. 127.0.0.1
     */
    bool isValidIPv4Address(const std::string &ip) {
        struct sockaddr_in sa;
        return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
    }

    bool argsAreValid(const std::string &ip, const std::string &port) {
        if(!isValidIPv4Address(ip)) {
            utils::println("Provided address is not valid IPv4 address.");
            return false;
        }

        if(!utils::isInteger(port)) {
            utils::println("Provided port is not an integer.");
            return false;
        }

        return true;
    }
}

int main(int argc, char** argv) {
    if(argc != 3) {
        utils::println("Invalid number of arguments.");
        utils::println("Application requires IPv4 address and port of server.");
        return 1;
    }

    const auto ip = argv[1];
    const auto port = argv[2];

    if(!argsvalidator::argsAreValid(ip, port))
        return 1;

    try {
        Client app(ip, port);
        app.start();
    } catch(std::runtime_error & err) {
        utils::println(err.what());
        return 1;
    }

    return 0;
}