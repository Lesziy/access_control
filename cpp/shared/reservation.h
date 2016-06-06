#pragma once

#include <ctime>
#include <string>
#include <cstring>
#include "json.hpp"

class Reservation {
public:
    static Reservation create(std::string start, int duration);

    std::string startToString() const;

    std::string startDateToString() const;

    std::string startTimeToString() const;

    inline int duration() const {
        return duration_;
    }

    inline struct tm toTmStruct() const {
        return start_;
    }

    inline time_t startTimeToTime_t() const {
        struct tm startTm = start_;
        return mktime(&startTm);
    }

    inline time_t endTimeToTime_t() const {
        struct tm startTm = start_;
        startTm.tm_hour += duration_;
        return mktime(&startTm);
    }

    static const Reservation missingReservation;
private:
    Reservation(){};
    constexpr static auto format_ = "%d.%m.%Y %T";
    constexpr static auto formatDate_ = "%d.%m.%Y";
    constexpr static auto formatTime_ = "%T";
    struct tm start_;
    int duration_;
};