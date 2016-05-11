#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include "json.hpp"

struct Reservation {
public:
    static Reservation create(std::string start, int duration) {
        Reservation reservation;

        reservation.duration_ = duration;
        auto cStart = start.c_str();
        auto startRef = &(reservation.start_);
        memset(startRef, 0, sizeof(struct tm));
        auto parseResult = strptime(cStart, format_, startRef);

        if(parseResult == NULL)
            throw new std::runtime_error("Invalid input");
        if(parseResult != cStart + start.length())
            throw new std::runtime_error("Input contains unnecessary trailing characters");

        return reservation;
    }

    std::string startToString() const {
        const int bufferSize = 30;
        char buffer[bufferSize];
        strftime(buffer, bufferSize, format_, &start_);
        return std::string(buffer);
    }

    std::string startDateToString() const {
        const int bufferSize = 30;
        char buffer[bufferSize];
        strftime(buffer, bufferSize, formatDate_, &start_);
        return std::string(buffer);
    }

    std::string startTimeToString() const {
        const int bufferSize = 30;
        char buffer[bufferSize];
        strftime(buffer, bufferSize, formatTime_, &start_);
        return std::string(buffer);
    }

    inline int duration() const {
        return duration_;
    }

    inline struct tm toTmStruct() const {
        return start_;
    }

private:
    constexpr static const char* format_ = "%d.%m.%Y %T";
    constexpr static const char* formatDate_ = "%d.%m.%Y";
    constexpr static const char* formatTime_ = "%T";
    struct tm start_;
    int duration_;
};