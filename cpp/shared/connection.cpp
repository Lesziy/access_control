//
// Created by julian on 29.05.16.
//

#include <iostream>
#include "connection.h"

void Connection::clean() {
    close(socketfd_);
}

void Connection::sendMessage(int socketfd, const std::string msg) {
    socketTransfer_.sendMessage(socketfd, msg);
}

const std::string Connection::receiveMessage(int socketfd) {
    if(queue.empty()) {
        auto obtained = socketTransfer_.receiveJSONMessage(socketfd);
        while(!obtained.empty())
            queue.push(dequeue(obtained));
    }
    return dequeue(queue);
}

const std::string Connection::dequeue(std::queue<std::string> &q) {
    auto toRet = std::string(q.front());
    q.pop();
    return toRet;
}