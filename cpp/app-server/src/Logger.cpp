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

std::string Logger::getCurrentTime() {
	time_t time_1;
	struct tm * date = new struct tm;
	char dateArray[80];

	time(&time_1);
	localtime_s(date, &time_1);
	strftime(dateArray, 80, "%H:%M:%S/%d/%m/%Y", date);
	
	std::string properData(dateArray);
	return properData;
}

void Logger::log(std::string ip, std::string user, std::string type, std::string description)
{
	logStream << getCurrentTime() << "; " << ip << "; " << user << "; " << type << "; " << description << "\n";
}

Logger::~Logger() {
	logStream.close();
}
