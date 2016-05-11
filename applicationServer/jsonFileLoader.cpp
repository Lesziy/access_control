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

void jsonFileLoader::addReservation(std::string path, reservation res, std::string username) {
    int index = 0;

    json data = getJson(path);

    for (auto& element : data["reservations"]) {
        index++;
    }
    data["reservations"][index]["date"] = res.start.day + "." + res.start.month + "." + res.start.year;
    data["reservations"][index]["interval_in_hours"] = res.duration;
    data["reservations"][index]["start_hour"] = res.start.hour + ":" + res.start.minute;
    data["reservations"][index]["username"] = username;
    saveFile(path, data);
}

std::string jsonFileLoader::loadFile(std::string path) {
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

void jsonFileLoader::saveFile(std::string path, json data){
    std::ofstream file;
    file.open(path);
    if (!file.is_open())
        perror("ERROR opening file");

    file << data;
    file.close();
}

