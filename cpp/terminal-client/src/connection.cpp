#include "connection.h"

Connection Connection::establishWith(const std::string & serverIp, const std::string & port) {
    Connection conn;
    if( (conn.socketfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) 
        perror("ERROR creating socket");
    auto address = initialiseAddress(serverIp, port);
    if( connect(conn.socketfd_, (struct sockaddr *) &address, sizeof(address)) < 0 )
        perror("ERROR connecting");
    return conn;
}

sockaddr_in Connection::initialiseAddress(const std::string & ip, const std::string & port) {
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons((unsigned short)std::stoi(port));
    return server;
}

void Connection::clean() {
    close(socketfd_);
}

void Connection::sendMessage(const std::string msg) {
    ssize_t totalSent = 0;
    do {
        sendFragment(totalSent, msg.c_str() + totalSent, msg.length() - totalSent);
    } while(totalSent != msg.length());
}

void Connection::sendFragment(ssize_t & totalSent, const char* toSend, const size_t toSendSize) {
    ssize_t sent = send(socketfd_, toSend, toSendSize, 0);
    if(sent < 0)
        perror("Send error");
    totalSent += sent;
}

const std::string Connection::receiveMessage() {
    const ssize_t buffer_size = 1000;
    std::string accumulator;
    while(receiveFragment(accumulator, buffer_size) == buffer_size);
    return accumulator;
}

ssize_t Connection::receiveFragment(std::string & accumulator, const unsigned int buffer_size) {
    char rcvMsg[buffer_size];

    auto obtained = recv(socketfd_, rcvMsg, buffer_size, 0);
    if( obtained < 0 )
        perror("ERROR receiving");
    rcvMsg[obtained] = '\0';
    accumulator += rcvMsg;

    return obtained;
}