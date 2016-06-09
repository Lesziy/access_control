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
#include "reservation.h"

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
    static const std::string createCancelFor(const Reservation & reservation);
    static const std::string createCanceledFor(const bool isCancelled);

    static const Reservation getReservation(
            const std::string & reservationMessage);
    static const std::pair<bool, Reservation> isReserved(
            const std::string & reservedMessage);
    static const bool unlockRequested(const std::string &unlockMessage);
    static const bool isUnlocked(const std::string & unlockedMessage);
    static const bool calendarRequested(const std::string &getCalendarMessage);
    static const std::vector<Reservation> getCalendar(
            const std::string & calendarMessage);
    static const Reservation getCancel(const std::string & cancelMessage);
    static const bool getCanceled(const std::string & canceledMessage);
    static const int getMessageType(const std::string & message);

    static const std::string createMyReservationsRequest();
    static const std::string createMyReservationsResponse(std::vector<Reservation> &reservations);
    static const std::vector<Reservation> getMyReservations(const std::string & myReservationsMessage);

private:
    static const json basicMessage(const std::string & title);

    static json fromReservation(const Reservation &r);
};

#endif /* COMMUNICATIONPROTOCOL_H_ */
