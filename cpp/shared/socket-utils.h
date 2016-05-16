#pragma once

#include <string>
#include <cstring>
#include <stdexcept>
#include <stack>
#include <sys/types.h>
#include <sys/socket.h>

class SocketUtils {
public:
    static void sendMessage(const int fd, const std::string msg);

    static const std::string receiveJSONMessage(const int fd);

private:
    static ssize_t sendFragment(const int fd, const char *toSend, const size_t toSendSize);
    static std::string receiveFragment(const int fd);
    static long validateBraces(const std::string received, std::stack<char> &braces);
};