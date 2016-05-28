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

#define BACKLOG 10

class Connection {
public:
    static Connection messageObject(const int sockfd);
    static Connection establishConnection(const std::string & port);
    int acceptConnection();

    void sendMessage(const int fd, const std::string msg) {
        socketTransfer_.sendMessage(fd, msg);
    }

    const std::string receiveMessage(const int fd) {
        if(queue.empty()) {
            auto obtained = socketTransfer_.receiveJSONMessage(socketfd_);
            while(!obtained.empty())
                queue.push(dequeue(obtained));
        }
        return dequeue(queue);
    }

    void clean();
private:
    const std::string dequeue(std::queue<std::string> &q) {
        auto toRet = std::string(q.front());
        q.pop();
        return toRet;
    }

    int socketfd_;
    SocketTransfer socketTransfer_;
    std::queue<std::string> queue;
};

#endif /* CONNECTION_H_ */
