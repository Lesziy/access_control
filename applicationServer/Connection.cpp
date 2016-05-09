/*
 * Connection.cpp
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#include "Connection.h"

Connection Connection::establishConnection(const std::string & port) {
    Connection conn;
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

void Connection::sendMessage(const int & fd, const std::string msg) {
    if (send(fd, msg.c_str(), msg.length(), 0) < 0)
        perror("ERROR sending");
}

const std::string Connection::receiveMessage(const int & fd) {
    char rcvMsg[2000];
    if (recv(fd, rcvMsg, 2000, 0) < 0)
        perror("ERROR receiving");
    return std::string(rcvMsg);
}

void Connection::clean() {
    close(socketfd_);
}

int Connection::acceptConnection() {
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

