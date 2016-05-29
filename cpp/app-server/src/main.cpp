/*
 * main.cpp
 *
 *  Created on: 4 maj 2016
 *      Author: secman
 */

#include <iostream>
#include "ServerApplication.h"

int main() {
    ServerApplication server;
    server.run();
    jsonFileLoader::addReservation("calendarFile.json", Reservation::create("01.01.2000, 12:00:00", 1), "Bartek");

    json js = jsonFileLoader::getJson("calendarFile.json");
    for (auto& element : js["reservations"]) {
        std::cout << element << '\n';
    }

    jsonFileLoader::addReservation("calendarFile.json", Reservation::create("01.01.2000, 12:00:00", 1), "Edek");

    js = jsonFileLoader::getJson("calendarFile.json");
    for (auto& element : js["reservations"]) {
        std::cout << element << '\n';
    }
    return 0;
}

