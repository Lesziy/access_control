/*
 * jsonFileLoader.h
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#ifndef JSONFILELOADER_H_
#define JSONFILELOADER_H_

#include <fstream>
#include <streambuf>
#include <string>

#include "json.hpp"
#include "communication-protocol.h"

using json = nlohmann::json;

class jsonFileLoader {
public:
    static json getJson(std::string path);
    static void addReservation(std::string path, Reservation res, std::string username);
private:
    static std::string loadFile(std::string path);
    static void saveFile(std::string path, json data);
};

#endif /* JSONFILELOADER_H_ */
