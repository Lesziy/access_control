#pragma once
/*
* Logger.h
*
*  Created on: 7 maj 2016
*      Author: secman
*/

#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

class Logger {
public:
	Logger(std::string logFilePath);
	~Logger();
	void log(std::string ip, std::string user, std::string type, std::string description);
	std::string getCurrentTime();

private:
	std::ofstream logStream;
};

#endif /* LOGGER_H_ */

