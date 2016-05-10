/*
 * ServerApplication.cpp
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#include "ServerApplication.h"

void* clientThreadFunction(void * ptr);

ServerApplication::ServerApplication() {
    loadConfiguration();

}

ServerApplication::~ServerApplication() {

}

void ServerApplication::run() {
    loadConfiguration();
    conn = Connection::establishConnection(serverPort);
    while (true) {
        int clientFD = conn.acceptConnection();
        clientThread(clientFD);
    }
}

void ServerApplication::authenticate() {
}

void ServerApplication::clientThread(int clientFD) {
    pthread_t thread;
    int* fdptr = &clientFD;
    pthread_create(&thread, NULL, clientThreadFunction, (void*) fdptr);
    pthread_join(thread, NULL);
}

void ServerApplication::loadConfiguration() {
    json j = jsonFileLoader::getJson("configuration.json");
    serverPort = j["configuration"]["port"];
    userFilePath = j["configuration"]["user_file_path"];
    calendarFilePath = j["configuration"]["calendar_file_path"];
    logFilePath = j["configuration"]["log_file_path"];
}

void* clientThreadFunction(void *sockfd) {
    long int numbytes = 1;
    char buf[MAXDATASIZE];
    int* fd = (int*) sockfd;
    int iMode = 0;
    std::string welcomeMessage = "Witamy na serwerze!!! Udało Ci się połączyć!";
    if (send(*fd, welcomeMessage.c_str(), welcomeMessage.length(), 0) == -1)
        perror("ERROR Welcome message sending");
    ioctl(*(int*)sockfd, FIONBIO, &iMode);
    while (numbytes != 0) {
        buf[0] = 'Z';
        if (numbytes = recv(*fd, buf, MAXDATASIZE - 1, 0) < 0)
            perror("ERROR receiving message");

        std::cout << buf << "/n";
    }
    close(*(int*)sockfd);
}

