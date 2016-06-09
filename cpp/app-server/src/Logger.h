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
<<<<<<< HEAD
	Logger(std::string logFilePath);
	~Logger();
	void log(std::string ip, std::string user, std::string type, std::string description);
	

private:
	std::string getCurrentTime();
	std::ofstream logStream;
=======
    Logger(std::string logFilePath);
    ~Logger();
    void log(std::string ip, std::string user, std::string type, std::string description);

private:
    std::ofstream logStream;
    std::string getCurrentTime();
    static pthread_mutex_t mutex;
>>>>>>> refs/remotes/origin/cpp-development
};

#endif /* LOGGER_H_ */

