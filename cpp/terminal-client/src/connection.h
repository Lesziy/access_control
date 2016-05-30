#pragma once

#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <socket-utils.h>

class Connection {
public:
    static Connection establishWith(const std::string & serverIp, const std::string & port);

    void sendMessage(const std::string msg) {
        SocketUtils::sendMessage(socketfd_, msg);
    }

    const std::string receiveMessage() {
        return SocketUtils::receiveJSONMessage(socketfd_);
    }

    void clean();
private:
    static sockaddr_in initialiseAddress(const std::string & ip, const std::string & port);

    int socketfd_;
};
