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
#include <queue>
#include "socket-utils.h"
#include "../shared/connection.h"

class ClientConnection: public Connection  {
public:
    static ClientConnection establishWith(const std::string & serverIp, const std::string & port);
    void sendMessage(const std::string msg);
    const std::string receiveMessage();
private:
    static sockaddr_in initialiseAddress(const std::string & ip, const std::string & port);
};
