#include "client-application.h"

/**
 * Tests if passed string contains IPv4 readable
 * representation of ip address, ex. 127.0.0.1
 */
bool isValidIPv4Address(const std::string &ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) == 1;
}

int main(int argc, char** argv) {

    if(argc != 3) {
        std::cout << "Not enough arguments passed. Provide ip address and port." << std::endl;
        return 1;
    }

    const auto ip = argv[1];
    const auto port = argv[2];

    if(!isValidIPv4Address(ip)) {
        std::cout << "Invalid ip address passed as an argument" << std::endl;
        return 1;
    }

    if(!isInteger(port)) {
        std::cout << "Invalid port number passed as an argument" << std::endl;
        return 1;
    }

    try {
        ClientApplication app(ip, port);
        app.run();
    } catch(std::runtime_error & err) {
        std::cout << err.what() << std::endl;
        return 1;
    }

    return 0;
}