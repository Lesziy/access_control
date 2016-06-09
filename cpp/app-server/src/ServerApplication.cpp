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
    conn = ServerConnection::establishConnection(serverPort);
    while (running_) {
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
    json j = jsonFileManager::getJson("configuration.json");
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

ServerApplication::~ServerApplication() {
    conn.clean();
    for(auto fd: clientDescriptors)
        close(fd);
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
    ServerConnection conn = ServerConnection::messageObject(sockfd);

    std::string clientIP = conn.getClientIP(sockfd);

    try {
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
                    std::string password = jsonFileManager::getPasswordHash(server->getUsersFilePath(), username);
                    std::string hashRes = server->hashPassword(password, challenge);
                    authenticated = response.compare(hashRes) == 0;
                    message = AuthenticationProtocol::createAuthenticatedFor(authenticated);
                    conn.sendMessage(sockfd, message);
                    challenge = "";                                                                             // it's a protection before many times using challenge
                    break;
                }
                case 5:                 //reservation
                {
                    Reservation res = CommunicationProtocol::getReservation(buf);
                    res.changeUsername(username);                                                               //we need to add to reservation a username get from authorization

                    if(CalendarManager::addReservation(server->getCalendarFilePath(), res))
                        conn.sendMessage(sockfd, CommunicationProtocol::createReservedFor(true, Reservation::missingReservation));
                    else
                        conn.sendMessage(sockfd, CommunicationProtocol::createReservedFor(false, res));
                    break;
                }
                case 7:                 //unlock
                {
                    auto ipToUnlock = CommunicationProtocol::ipToUnlock(buf);
                    conn.sendMessage(sockfd,
                                     CommunicationProtocol::createUnlockedFor(
                                             IptablesManager::unlock(username,
                                                                     ipToUnlock == "" ? clientIP : ipToUnlock,
                                                                     server->getCalendarFilePath())));
                    break;
                }
                case 9:                 //getMyMessages
                {
                    std::vector <Reservation> reservations = CalendarManager::getReservations(server->getCalendarFilePath());
                    conn.sendMessage(sockfd, CommunicationProtocol::createCalendarFor(reservations));
                    break;
                }
                case 11:                //cancel
                {
                    Reservation res = CommunicationProtocol::getCancel(buf);
                    res.changeUsername(username);
                    conn.sendMessage(sockfd, CommunicationProtocol::createCanceledFor(CalendarManager::cancelReservation(server->getCalendarFilePath(), res)));
                    break;
                }
                case 13: // getMyReservations
                {
                    auto reservations = CalendarManager::getReservations(server->getCalendarFilePath(), username);
                    conn.sendMessage(sockfd, CommunicationProtocol::createMyReservationsResponse(reservations));
                    break;
                }
                default:
                    throw std::runtime_error("Unsupported action");
            }
            buf.clear();
        }
    } catch(std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }

    close(sockfd);
    return nullptr;
}