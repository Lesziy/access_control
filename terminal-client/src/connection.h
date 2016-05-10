#pragma once

#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

class Connection {
public:
    static Connection establishWith(const std::string & serverIp, const std::string & port);
    void sendMessage(const std::string msg);
    const std::string receiveMessage();
    void clean();
private:
    static sockaddr_in initialiseAddress(const std::string & ip, const std::string & port);
    int receiveFragment(std::string & accumulator, const unsigned int buffer_size);
    
    int socketfd_;
};
