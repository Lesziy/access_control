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
    struct Msg
    {
        int* clientfd;
        ServerApplication* ptrServer;
    } *msg;

    clientDescriptors.push_back(10);
    msg = new Msg();
    msg->clientfd = &(clientDescriptors.at(clientDescriptors.size()-1));
    int test = *msg->clientfd;
    msg->ptrServer = this;


    pthread_create(&thread, NULL, clientThreadFunction, (void*) msg);
}

void ServerApplication::loadConfiguration() {
    json j = jsonFileLoader::getJson("configuration.json");
    serverPort = j["configuration"]["port"];
    userFilePath = j["configuration"]["user_file_path"];
    calendarFilePath = j["configuration"]["calendar_file_path"];
    logFilePath = j["configuration"]["log_file_path"];
}

void* clientThreadFunction(void *data) {
    struct Msg
    {
        int* clientfd;
        ServerApplication* ptrServer;
    } *msg;

    msg = (Msg*)data;
    int sockfd = *msg->clientfd;
    ServerApplication* server = msg->ptrServer;
    delete(msg);

    long int numbytes = 1;
    std::string buf;
    Connection conn = server->getConnection();

    //std::string welcomeMessage = "Witamy na serwerze!!! Udało Ci się połączyć!";
    //if (send(*(int*)sockfd, welcomeMessage.c_str(), welcomeMessage.length(), 0) == -1)
      //  perror("ERROR Welcome message sending");

    while (numbytes != 0) {
        buf.clear();
        numbytes = conn.receiveFragment(buf, 200);

        switch(CommunicationProtocol::getMessageType(buf))
        {
            case 1:                 //handshake
                break;
            case 3:                 //response
                break;
            case 5:                 //reservation
            {
                Reservation res = CommunicationProtocol::getReservation(buf);
                jsonFileLoader::addReservation(server->getCalendarFilePath(), res, "Bartek");
                break;
            }
            case 7:                 //unlock
                break;
            case 9:                 //getCalendar
                break;
            case 11:                //cancel
                break;
        }
        std::cout << buf << "/n";
    }
    close(sockfd);
}

