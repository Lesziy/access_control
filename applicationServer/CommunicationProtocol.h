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

struct reservation {
    struct tm start;			//http://www.cplusplus.com/reference/ctime/tm/
    unsigned duration;
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
    static const std::string createCancelFor(const tm & startDate);
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
    static const tm getCancel(const std::string & cancelMessage);
    static const bool getCanceled(const std::string & canceledMessage);

private:
    static const json basicMessage(const std::string & title);
};

#endif /* COMMUNICATIONPROTOCOL_H_ */
