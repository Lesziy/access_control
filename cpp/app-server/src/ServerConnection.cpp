/*
 * Connection.cpp
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#include "ServerConnection.h"

ServerConnection ServerConnection::messageObject(const int sockfd) {
    ServerConnection conn;
    conn.socketfd_ = sockfd;
    return conn;
}

ServerConnection ServerConnection::establishConnection(const std::string & port) {
    ServerConnection conn;
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(NULL, port.c_str(), &hints, &servinfo) != 0)
        perror("ERROR getting address info");

    if ((conn.socketfd_ = socket(servinfo->ai_family, servinfo->ai_socktype,
            servinfo->ai_protocol)) == -1)
        perror("ERROR creating socket");

    int yes = 1;
    if (setsockopt(conn.socketfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
            == -1)
        perror("ERROR set socket address to be reused");

    if (bind(conn.socketfd_, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(conn.socketfd_);
        perror("ERROR binding address to socket");
    }

    if (listen(conn.socketfd_, BACKLOG) == -1)
        perror("ERROR listening");

    return conn;
}

int ServerConnection::acceptConnection() {
    socklen_t sin_size;
    char str[INET_ADDRSTRLEN];
    struct sockaddr_in their_addr; 			// connector's address information
    int new_fd;

    std::cout << "Waiting for connection...\n\n";

    sin_size = sizeof(sockaddr_storage);
    new_fd = accept(this->socketfd_, (struct sockaddr*) &their_addr, &sin_size);

    if (new_fd == -1)
        perror("ERROR while accepting new connection");

    inet_ntop(AF_INET, &(their_addr.sin_addr.s_addr), str, INET_ADDRSTRLEN);

    std::cout << "Client with IP address: " << str << " connected, fd number: "
            << new_fd << " .\n";

    return new_fd;
}

std::string ServerConnection::getClientIP(const int sockfd) {
    socklen_t len;
    struct sockaddr_storage addr;
    char ip[INET_ADDRSTRLEN];
    len = sizeof addr;
    getpeername(sockfd, (struct sockaddr*)&addr, &len);
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    inet_ntop(AF_INET, &s->sin_addr, ip, INET_ADDRSTRLEN);
    return std::string(ip);
}