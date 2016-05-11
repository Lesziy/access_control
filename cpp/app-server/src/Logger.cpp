/*
 * Logger.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "Logger.h"

Logger::Logger(std::string logFilePath) {
    logStream.open(logFilePath, std::ios_base::out | std::ios_base::app);
    if (!logStream.is_open())
        perror("ERROR can't open log file");
}

Logger::~Logger() {
    logStream.close();
}
