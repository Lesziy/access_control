/*
 * main.cpp
 *
 *  Created on: 4 maj 2016
 *      Author: secman
 */

#include <iostream>
#include "ServerApplication.h"
#include "jsonFileLoader.h"

int main() {
    ServerApplication server;
    server.run();
    reservation res;
    res.duration = 1;
    Date date("01", "01", "2000", "00", "00", "00");
    res.start = date;
    jsonFileLoader::addReservation("calendarFile.json", res, "Bartek");

    json js = jsonFileLoader::getJson("calendarFile.json");
    for (auto& element : js["reservations"]) {
        std::cout << element << '\n';
    }

    jsonFileLoader::addReservation("calendarFile.json", res, "Edek");

    js = jsonFileLoader::getJson("calendarFile.json");
    for (auto& element : js["reservations"]) {
        std::cout << element << '\n';
    }
    return 0;
}

