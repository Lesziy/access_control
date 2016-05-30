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