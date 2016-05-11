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

class Logger {
public:
    Logger(std::string logFilePath);
    ~Logger();

private:
    std::ofstream logStream;
};

#endif /* LOGGER_H_ */
