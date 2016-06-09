//
// Created by julian on 29.05.16.
//

#include "reservation.h"

Reservation Reservation::create(std::string start, int duration) {
    Reservation reservation;

    reservation.duration_ = duration;
    auto cStart = start.c_str();
    auto startRef = &(reservation.start_);
    memset(startRef, 0, sizeof(struct tm));
    auto parseResult = strptime(cStart, format_, startRef);

    if(parseResult == NULL)
        throw std::runtime_error("Invalid input");
    if(parseResult != cStart + start.length())
        throw std::runtime_error("Input contains unnecessary trailing characters");

    reservation.username_ = "";

    return reservation;
}

Reservation Reservation::create(std::string start, int duration, std::string username) {
    Reservation reservation = create(start, duration);
    reservation.username_ = username;
    return reservation;
}

std::string Reservation::startToString() const {
    const int bufferSize = 30;
    char buffer[bufferSize];
    strftime(buffer, bufferSize, format_, &start_);
    return std::string(buffer);
}

std::string Reservation::startDateToString() const {
    const int bufferSize = 30;
    char buffer[bufferSize];
    strftime(buffer, bufferSize, formatDate_, &start_);
    return std::string(buffer);
}

std::string Reservation::startTimeToString() const {
    const int bufferSize = 30;
    char buffer[bufferSize];
    strftime(buffer, bufferSize, formatTime_, &start_);
    return std::string(buffer);
}

std::string Reservation::getDuration() const {
    const int bufferSize = 30;
    char buffer[bufferSize];
    sprintf(buffer, "%d", duration_);
    return std::string(buffer);
}

std::string Reservation::getReservationDescription(){
    std::string description = startDateToString() + std::string(" ")  + startTimeToString() + std::string(" ")
                                +  getDuration()  + std::string(" hours");
    return description;
}

const Reservation Reservation::missingReservation = Reservation();