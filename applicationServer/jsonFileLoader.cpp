/*
 * jsonFileLoader.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "jsonFileLoader.h"

json jsonFileLoader::getJson(std::string path) {
    return json::parse(loadFile(path));
}

std::string jsonFileLoader::loadFile(std::string path) {
    std::ifstream file;
    file.open("configuration.json");
    if (!file.is_open())
        perror("ERROR can't load configuration file");

    std::string str;

    str.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
    file.close();
    return str;
}
