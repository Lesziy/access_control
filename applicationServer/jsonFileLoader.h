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

using json = nlohmann::json;

class jsonFileLoader {
public:
    static json getJson(std::string path);
private:
    static std::string loadFile(std::string path);
};

#endif /* JSONFILELOADER_H_ */
