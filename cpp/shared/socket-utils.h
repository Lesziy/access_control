#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>

class SocketUtils {
public:
    static void sendMessage(const int fd, const std::string msg);
    static const std::string receiveMessage(const int fd);
private:
    static void sendFragment(const int fd, ssize_t &totalSent, const char *toSend, const size_t toSendSize);
    static ssize_t receiveFragment(const int fd, std::string & accumulator, const unsigned int buffer_size);
};