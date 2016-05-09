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
    server.sin_port = htons(std::stoi(port));
    return server;
}

void Connection::sendMessage(const std::string msg) {
    if(send(socketfd_, msg.c_str(), msg.length(), 0) < 0)
        perror("ERROR sending");
}

const std::string Connection::receiveMessage() {
    const unsigned int buffer_size = 201;
    std::string accumulator;

    while(receiveFragment(accumulator, buffer_size) == buffer_size);

    return accumulator;
}

int Connection::receiveFragment(std::string & accumulator, const unsigned int buffer_size) {
    char rcvMsg[buffer_size];

    int obtained = recv(socketfd_, rcvMsg, buffer_size, 0);
    if( obtained < 0 )
        perror("ERROR receiving");
    rcvMsg[obtained] = '\0';
    accumulator += rcvMsg;

    return obtained;
}

void Connection::clean() {
    close(socketfd_);
}
