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
#include "socket-transfer.h"

class Connection {
public:
    void clean();
    void sendMessage(int socketfd, const std::string msg);
    const std::string receiveMessage(int socketfd);
protected:
    int socketfd_;
private:
    const std::string dequeue(std::queue<std::string> &q);

    SocketTransfer socketTransfer_;
    std::queue<std::string> queue;
};