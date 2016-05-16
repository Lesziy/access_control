/*
 * ServerApplication.h
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#ifndef SERVERAPPLICATION_H_
#define SERVERAPPLICATION_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <pthread.h>

#include "auth-protocol.h"
#include "Connection.h"
#include "jsonFileLoader.h"
#include "Logger.h"

#define MAXDATASIZE 200

class ServerApplication {
public:
    ServerApplication();
    ~ServerApplication();
    void run();

    const std::string getCalendarFilePath() const {
        return calendarFilePath;
    }

    const std::string getUsersFilePath() const {
        return userFilePath;
    }

    std::string hashPassword(const std::string & passwordHash, const std::string & challenge);
    std::string generateChallenge();

private:
    void clientThread(int clientFD);
    void loadConfiguration();

    Connection conn;
    Logger* log;

    std::string serverPort;
    std::string userFilePath, calendarFilePath, logFilePath;
    std::vector<int> clientDescriptors;
};

#endif /* SERVERAPPLICATION_H_ */
