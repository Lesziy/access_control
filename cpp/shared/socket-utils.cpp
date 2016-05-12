#include "socket-utils.h"

void SocketUtils::sendMessage(const int fd, const std::string msg) {
    ssize_t totalSent = 0;
    do {
        sendFragment(fd, totalSent, msg.c_str() + totalSent, msg.length() - totalSent);
    } while(totalSent != msg.length());
}

void SocketUtils::sendFragment(const int fd, ssize_t & totalSent, const char* toSend, const size_t toSendSize) {
    ssize_t sent = send(fd, toSend, toSendSize, 0);
    if(sent < 0)
        perror("Send error");
    totalSent += sent;
}

const std::string SocketUtils::receiveMessage(const int fd) {
    const ssize_t buffer_size = 1000;
    std::string accumulator;
    while(receiveFragment(fd, accumulator, buffer_size) == buffer_size);
    return accumulator;
}

ssize_t SocketUtils::receiveFragment(const int fd, std::string & accumulator, const unsigned int buffer_size) {
    char rcvMsg[buffer_size];

    auto obtained = recv(fd, rcvMsg, buffer_size, 0);
    if( obtained < 0 )
        perror("ERROR receiving");
    rcvMsg[obtained] = '\0';
    accumulator += rcvMsg;

    return obtained;
}