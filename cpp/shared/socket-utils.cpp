#include "socket-utils.h"

void SocketTransfer::sendMessage(const int fd, const std::string msg) {
    ssize_t totalSent = 0;
    do {
        auto toSend = msg.c_str() + totalSent;
        auto toSendLength = msg.length() - totalSent;
        totalSent += sendFragment(fd, toSend, toSendLength);
    } while(totalSent != msg.length());
}

ssize_t SocketTransfer::sendFragment(const int fd, const char* toSend, const size_t toSendSize) {
    ssize_t sent = send(fd, toSend, toSendSize, 0);
    if(sent < 0)
        perror("Send error");
    return sent;
}

const std::queue<std::string> SocketTransfer::receiveJSONMessage(const int fd) {
    JsonMessageBuffer::ValidMessages container;

    while(container.empty()) {
        auto received = receiveFragment(fd);
        container = messageBuffer_.parseMessages(received);
    }

    return container;
}

std::string SocketTransfer::receiveFragment(const int fd) {
    const ssize_t buffer_size = 20;
    char rcvMsg[buffer_size];
    auto obtained = recv(fd, rcvMsg, buffer_size, 0);

    if( obtained < 0 )
        throw std::runtime_error(strerror(errno));
    if(obtained == 0)
        throw std::runtime_error("Client disconnected");

    rcvMsg[obtained] = '\0';
    return std::string(rcvMsg);
}