#pragma once

#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <socket-utils.h>
#include <queue>

class Connection {
public:
    static Connection establishWith(const std::string & serverIp, const std::string & port);

    void sendMessage(const std::string msg) {
        socketTransfer_.sendMessage(socketfd_, msg);
    }

    const std::string receiveMessage() {
        if(queue.empty()) {
            auto obtained = socketTransfer_.receiveJSONMessage(socketfd_);
            while(!obtained.empty())
                queue.push(dequeue(obtained));
        }
        return dequeue(queue);
    }

    void clean();
private:
    static sockaddr_in initialiseAddress(const std::string & ip, const std::string & port);
    const std::string dequeue(std::queue<std::string> &q) {
        auto toRet = std::string(q.front());
        q.pop();
        return toRet;
    }

    int socketfd_;
    SocketTransfer socketTransfer_;
    std::queue<std::string> queue;
};
