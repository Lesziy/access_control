/*
 * ServerApplication.cpp
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#include "ServerApplication.h"
#include "sha3.h"

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
}

void ServerApplication::loadConfiguration() {
    json j = jsonFileLoader::getJson("configuration.json");
    serverPort = j["configuration"]["port"];
    userFilePath = j["configuration"]["user_file_path"];
    calendarFilePath = j["configuration"]["calendar_file_path"];
    logFilePath = j["configuration"]["log_file_path"];
}

std::string ServerApplication::hashPassword(const std::string & passwordHash, const std::string & challenge){
    SHA3 sha3;
    std::string response = challenge;
    response.append(passwordHash);
    response = sha3(response);
    return response;
}

std::string ServerApplication::generateChallenge() {
    std::random_device rd;
    SHA3 sha3;
    std::mt19937 pseudoRandomNumberGenerator(rd());
    std::string challenge = std::to_string(pseudoRandomNumberGenerator());
    challenge = sha3(challenge);
    return  challenge;

}

void* clientThreadFunction(void *sockfd) {
    long int numbytes = 1;
    std::string buf;
    //std::string welcomeMessage = "Witamy na serwerze!!! Udało Ci się połączyć!";

    Connection conn = Connection::messageObject(*(int*)sockfd);
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
                jsonFileLoader::addReservation("calendarFile.json", res, "Bartek");
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
    close(*(int*)sockfd);
}

