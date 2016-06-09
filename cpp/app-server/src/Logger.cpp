/*
* Logger.cpp
*
*  Created on: 7 maj 2016
*      Author: secman
*/

#include "Logger.h"

pthread_mutex_t Logger::mutex = PTHREAD_MUTEX_INITIALIZER;


Logger::Logger(std::string logFilePath) {
	logStream.open(logFilePath, std::ios_base::out | std::ios_base::app);
	if (!logStream.is_open())
		perror("ERROR can't open log file");
}

std::string Logger::getCurrentTime() {
	time_t time_1;
	struct tm * date = new struct tm;
	char dateArray[80];

	time(&time_1);
	date = localtime(&time_1);
	strftime(dateArray, 80, "%H:%M:%S/%d/%m/%Y", date);
	
	std::string properData(dateArray);
	return properData;
}

void Logger::log(std::string ip, std::string user, std::string type, std::string description)
{
	logStream << getCurrentTime() << "; " << ip << "; " << user << "; " << type << "; " << description << "\n";
}

std::string Logger::getCurrentTime() {
    time_t time_1;
    struct tm * date;
    char dateArray[80];

    time(&time_1);
    date = localtime(&time_1);
    strftime(dateArray, 80, "%H:%M:%S/%d/%m/%Y", date);

    std::string properData(dateArray);
    return properData;
}


void Logger::log(std::string ip, std::string user, std::string type, std::string description)
{
    pthread_mutex_lock(&mutex);
    logStream << getCurrentTime() << "; " << ip << "; " << user << "; " << type << "; " << description << "\n";
    logStream.flush();
    pthread_mutex_unlock(&mutex);
}


Logger::~Logger() {
	logStream.close();
}
