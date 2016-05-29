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
#include <socket-utils.h>
#include "../../shared/connection.h"

#define BACKLOG 10

class ServerConnection : public Connection {
public:
    static ServerConnection messageObject(const int sockfd);
    static ServerConnection establishConnection(const std::string & port);
    int acceptConnection();
};

#endif /* CONNECTION_H_ */
