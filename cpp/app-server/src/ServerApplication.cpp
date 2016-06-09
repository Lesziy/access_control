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
    logger = new Logger(logFilePath);
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
    Logger* logger = server->getLogger();
    std::string description;

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
                    logger->log(clientIP, username, std::string("HANDSHAKE"), description);
                    break;
                }
                case 3:                 //response
                {
                    bool authenticated;
                    std::string message;
                    std::string response = AuthenticationProtocol::getResponse(buf);
                    logger->log(clientIP, username, std::string("RESPONSE"), description);
                    std::string password = jsonFileManager::getPasswordHash(server->getUsersFilePath(), username);
                    std::string hashRes = server->hashPassword(password, challenge);
                    authenticated = response.compare(hashRes) == 0;
                    message = AuthenticationProtocol::createAuthenticatedFor(authenticated);
                    conn.sendMessage(sockfd, message);
                    if(!authenticated)
                        logger->log(clientIP, username, std::string("NOT_AUTHENTICATED"), description);
                    else
                        logger->log(clientIP, username, std::string("AUTHENTICATED"), description);
                    challenge = "";                                                                             // it's a protection against using challenge many times
                    break;
                }
                case 5:                 //reservation
                {
                    Reservation res = CommunicationProtocol::getReservation(buf);
                    res.changeUsername(username);                                                               //we need to add to reservation a username get from authorization
                    description = res.getReservationDescription();
                    logger->log(clientIP, username, std::string("RESERVATION"), description);
                    if(CalendarManager::addReservation(server->getCalendarFilePath(), res)) {
                        description.clear();
                        conn.sendMessage(sockfd, CommunicationProtocol::createReservedFor(true, Reservation::missingReservation));
                        logger->log(clientIP, username, std::string("RESERVED"), description);
                    }
                    else {
                        conn.sendMessage(sockfd, CommunicationProtocol::createReservedFor(false, res));
                        logger->log(clientIP, username, std::string("NOT_RESERVED"), description);
                        description.clear();
                    }
                    break;
                }
                case 7:                 //unlock
                {

                    auto ipToUnlock = CommunicationProtocol::ipToUnlock(buf);
                    conn.sendMessage(sockfd,
                                     CommunicationProtocol::createUnlockedFor(
                                             IptablesManager::unlock(username,
                                                                     ipToUnlock == "" ? clientIP : ipToUnlock,
                                                                     server->getCalendarFilePath(), logger)));

                    conn.sendMessage(sockfd, CommunicationProtocol::createUnlockedFor(IptablesManager::unlock(username, clientIP, server->getCalendarFilePath(), logger)));
                    logger->log(clientIP, username, std::string("UNLOCK"), description);

                    break;
                }
                case 9:                 //getMyMessages
                {
                    std::vector <Reservation> reservations = CalendarManager::getReservations(server->getCalendarFilePath());
                    conn.sendMessage(sockfd, CommunicationProtocol::createCalendarFor(reservations));
                    logger->log(clientIP, username, std::string("GET_CALENDAR"), description);
                    break;
                }
                case 11:                //cancel
                {
                    Reservation res = CommunicationProtocol::getCancel(buf);
                    description = res.getReservationDescription();
                    logger->log(clientIP, username, std::string("CANCEL"), description);
                    res.changeUsername(username);
                    conn.sendMessage(sockfd, CommunicationProtocol::createCanceledFor(CalendarManager::cancelReservation(server->getCalendarFilePath(), res)));
                    logger->log(clientIP, username, std::string("CANCELLED"), description);
                    description.clear();
                    break;
                }
                case 13: // getMyReservations
                {
                    auto reservations = CalendarManager::getReservations(server->getCalendarFilePath(), username);
                    conn.sendMessage(sockfd, CommunicationProtocol::createMyReservationsResponse(reservations));
                    break;
                }
                default:
                {
                    logger->log(clientIP, username, std::string("BAD_CALL"), description);
                    throw std::runtime_error("Unsupported action");
                }

            }
            buf.clear();
        }
    } catch(std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }

    logger->log(clientIP, username, std::string("SESSION_END"), description);
    close(sockfd);
    return nullptr;
}


Logger* ServerApplication::getLogger() {
    return logger;
}