/*
 * CommunicationProtocol.h
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#ifndef COMMUNICATIONPROTOCOL_H_
#define COMMUNICATIONPROTOCOL_H_

#include <ctime>
#include "json.hpp"

struct Date{
    std::string day;
    std::string month;
    std::string year;
    std::string hour;
    std::string minute;
    std::string second;
    Date(){};
    Date(std::string day, std::string month, std::string year, std::string hour, std::string minute, std::string second = 0):
            day(day),
            month(month),
            year(year),
            hour(hour),
            minute(minute),
            second(second)
    {}
    Date(const Date& date)
    {
        day = date.day;
        month = date.month;
        year = date.year;
        hour = date.hour;
        minute = date.minute;
        second = date.second;
    }

    static std::string toString(const Date date)
    {
        return date.hour + ":" + date.minute + ":" + date.second + "/" + date.day + "/" + date.month + "/" + date.year;
    }

    static Date parse(std::string strDate)
    {
        Date date;
        date.hour = strDate.substr(0,2);
        date.minute = strDate.substr(3,2);
        date.second = strDate.substr(6,2);
        date.day = strDate.substr(9,2);
        date.month = strDate.substr(12,2);
        date.year = strDate.substr(15,4);
        return date;
    }

};

struct reservation {
    struct Date start;			//http://www.cplusplus.com/reference/ctime/tm/
    unsigned duration;
    reservation& operator=(const reservation& other)
    {
        if (this != &other) {
            this->duration = other.duration;
            this->start = other.start;
        }
        return *this;
    }
};

typedef struct reservation Reservation;

using json = nlohmann::json;

class CommunicationProtocol {
public:
    static const std::string createReservationFor(
            const Reservation& reservation);
    static const std::string createReservedFor(const bool isReserved,
            const Reservation& reservation);
    static const std::string createUnlockFor();
    static const std::string createUnlockedFor(const bool isUnlocked);
    static const std::string createGetCalendarFor();
    static const std::string createCalendarFor(
            std::vector<Reservation>& reservations);
    static const std::string createCancelFor(const Date & startDate);
    static const std::string createCanceledFor(const bool isCancelled);

    static const Reservation getReservation(
            const std::string & reservationMessage);
    static const std::pair<bool, Reservation> isReserved(
            const std::string & reservedMessage);
    static const void getUnlock(const std::string & unlockMessage);
    static const bool isUnlocked(const std::string & unlockedMessage);
    static const void getGetCalendar(const std::string & getCalendarMessage);
    static const std::vector<Reservation> getCalendar(
            const std::string & calendarMessage);
    static const Date getCancel(const std::string & cancelMessage);
    static const bool getCanceled(const std::string & canceledMessage);
    static const int getMessageType(const std::string & message);

private:
    static const json basicMessage(const std::string & title);
};

#endif /* COMMUNICATIONPROTOCOL_H_ */
