#include "client-connection.h"

ClientConnection ClientConnection::establishWith(const std::string & serverIp, const std::string & port) {
    ClientConnection conn;
    if( (conn.socketfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) 
        throw std::runtime_error("Socket couldn't be created.");
    auto address = initialiseAddress(serverIp, port);
    if( connect(conn.socketfd_, (struct sockaddr *) &address, sizeof(address)) < 0 )
        throw std::runtime_error("Connection to server couldn't be established.");
    return conn;
}

sockaddr_in ClientConnection::initialiseAddress(const std::string & ip, const std::string & port) {
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons((unsigned short) std::stoi(port));
    return server;
}

const std::string ClientConnection::receiveMessage() {
    auto msg = Connection::receiveMessage(socketfd_);
    return msg;
}

void ClientConnection::sendMessage(const std::string msg) {
    Connection::sendMessage(socketfd_, msg);
}