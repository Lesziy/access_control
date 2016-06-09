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
#include "ServerConnection.h"
#include "jsonFileManager.h"
#include "calendarManager.h"
#include "Logger.h"
#include "IptablesManager.h"

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

    ServerConnection conn;
    Logger* log;

    bool running_ = true;
    std::string serverPort;
    std::string userFilePath, calendarFilePath, logFilePath;
    std::vector<int> clientDescriptors;
};

#endif /* SERVERAPPLICATION_H_ */
