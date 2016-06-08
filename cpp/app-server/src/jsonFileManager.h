/*
 * jsonFileLoader.h
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#ifndef JSONFILEMANAGER_H_
#define JSONFILEMANAGER_H_

#include <fstream>
#include <streambuf>
#include <string>

#include "json.hpp"
#include "communication-protocol.h"

using json = nlohmann::json;

class jsonFileManager {
public:
    static json getJson(std::string path);
    static std::string getPasswordHash(std::string path, std::string username);
    static std::string loadFile(std::string path);
    static void saveFile(std::string path, json data);
};

#endif /* JSONFILELOADER_H_ */
