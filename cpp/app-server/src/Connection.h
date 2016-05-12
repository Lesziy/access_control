/*
 * Connection.h
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 10

class Connection {
public:
    static Connection messageObject(const int sockfd);
    static Connection establishConnection(const std::string & port);
    int acceptConnection();
    void sendMessage(const int & fd, const std::string msg);
    const std::string receiveMessage(const int & fd);
    ssize_t receiveFragment(const int & fd, std::string & accumulator, const unsigned int buffer_size);
    void clean();
private:

    int socketfd_;
};

#endif /* CONNECTION_H_ */
