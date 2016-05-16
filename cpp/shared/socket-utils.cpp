#include "socket-utils.h"

void SocketUtils::sendMessage(const int fd, const std::string msg) {
    ssize_t totalSent = 0;
    do {
        auto toSend = msg.c_str() + totalSent;
        auto toSendLength = msg.length() - totalSent;
        totalSent += sendFragment(fd, toSend, toSendLength);
    } while(totalSent != msg.length());
}

ssize_t SocketUtils::sendFragment(const int fd, const char* toSend, const size_t toSendSize) {
    ssize_t sent = send(fd, toSend, toSendSize, 0);
    if(sent < 0)
        perror("Send error");
    return sent;
}

const std::string SocketUtils::receiveJSONMessage(const int fd) {
    std::stack<char> braces;
    std::string message;

    while(true) {
        auto received = receiveFragment(fd);
        auto msgEnd = validateBraces(received, braces);
        if(msgEnd == -1)
            message += received;
        else
            return message + received.substr(0, msgEnd + 1);
        /* this is simple request - response so second
           message following first one is not considered */
    }
}

std::string SocketUtils::receiveFragment(const int fd) {
    const ssize_t buffer_size = 20;
    char rcvMsg[buffer_size];
    auto obtained = recv(fd, rcvMsg, buffer_size, 0);

    if( obtained < 0 )
        throw std::runtime_error(strerror(errno));

    rcvMsg[obtained] = '\0';
    return std::string(rcvMsg);
}

long SocketUtils::validateBraces(const std::string received, std::stack<char> &braces) {
    for (unsigned i = 0; i < received.size(); ++i) {
        auto letter = received.at(i);
        if(letter == '{')
            braces.push('{');
        else if(letter == '}') {
            if(braces.empty())
                throw std::runtime_error("Matching brace not found");
            braces.pop();
            if(braces.empty())
                return i;
        }
    }
    return -1;
}