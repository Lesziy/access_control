/*
 * jsonFileManager.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "jsonFileManager.h"

json jsonFileManager::getJson(std::string path) {
    return json::parse(loadFile(path));
}

std::string jsonFileManager::loadFile(std::string path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open())
        perror("ERROR opening file");

    std::string str;

    str.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
    file.close();
    return str;
}

void jsonFileManager::saveFile(std::string path, json data){
    std::ofstream file;
    file.open(path);
    if (!file.is_open())
        perror("ERROR opening file");

    file << data.dump(4);
    file.close();
}

std::string jsonFileManager::getPasswordHash(std::string path, std::string username) {
    auto data = getJson(path);
    for (auto& element : data["users"]) {
        if(username.compare(element["login"]) == 0)
            return element["password_hash"];
    }
    return "default";
}



