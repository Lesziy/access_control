#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include "JsonMessageBuffer.h"

class SocketTransfer {
public:
    void sendMessage(const int fd, const std::string msg);
    const std::queue<std::string> receiveJSONMessage(const int fd);
private:
    ssize_t sendFragment(const int fd, const char *toSend, const size_t toSendSize);
    std::string receiveFragment(const int fd);

    JsonMessageBuffer messageBuffer_;
};