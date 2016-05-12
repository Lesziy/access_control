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

void ServerApplication::run() {
    loadConfiguration();
    conn = Connection::establishConnection(serverPort);
    while (true) {
        int clientFD = conn.acceptConnection();
        clientThread(clientFD);
    }
}

void ServerApplication::clientThread(int clientFD) {
    pthread_t thread;
    struct Msg
    {
        int* clientfd;
        ServerApplication* ptrServer;
    } *msg;

    clientDescriptors.push_back(clientFD);
    msg = new Msg();
    msg->clientfd = &(clientDescriptors.at(clientDescriptors.size()-1));
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
    return challenge;
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

    std::string buf;
    std::string username, challenge;
    Connection conn = Connection::messageObject(sockfd);

    while ((buf = conn.receiveMessage(sockfd)).length() != 0) {
        switch(CommunicationProtocol::getMessageType(buf))
        {
            case 1:                 //handshake
            {
                username = AuthenticationProtocol::getLogin(buf);
                challenge = server->generateChallenge();
                std::string message = AuthenticationProtocol::createChallengeFor(challenge);
                conn.sendMessage(sockfd, message);
                break;
            }
            case 3:                 //response
            {
                bool authenticated;
                std::string message;
                std::string response = AuthenticationProtocol::getResponse(buf);
                std::string password = "c0067d4af4e87f00dbac63b6156828237059172d1bbeac67427345d6a9fda484";
                std::string hashRes = server->hashPassword(password, challenge);
                authenticated = response.compare(hashRes) == 0;
                message = AuthenticationProtocol::createAuthenticatedFor(authenticated);
                conn.sendMessage(sockfd, message);
                break;
            }
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
            default:
                throw std::runtime_error("Unsupported action");
        }
        buf.clear();
    }

    close(sockfd);
    return nullptr;
}

