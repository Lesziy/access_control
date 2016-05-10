/*
 * CommunicationProtocol.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "CommunicationProtocol.h"

const std::string CommunicationProtocol::createReservationFor(
        const Reservation& reservation) {
    auto msg = basicMessage("reservation");
    msg["start"] = Date::toString(reservation.start);
    msg["duration"] = reservation.duration;
    return msg.dump();
}

const std::string CommunicationProtocol::createReservedFor(
        const bool isReserved, const Reservation& reservation) {
    auto msg = basicMessage("reserved");
    msg["value"] = isReserved ? "true" : "false";
    msg["overlap"] = createReservationFor(reservation);
    return msg.dump();
}

const std::string CommunicationProtocol::createUnlockFor() {
    auto msg = basicMessage("unlock");
    //TODO
    return msg.dump();
}

const std::string CommunicationProtocol::createUnlockedFor(
        const bool isUnlocked) {
    auto msg = basicMessage("unlocked");
    //TODO
    return msg.dump();
}

const std::string CommunicationProtocol::createGetCalendarFor() {
    auto msg = basicMessage("getCalendar");
    //TODO
    return msg.dump();
}

const std::string CommunicationProtocol::createCalendarFor(
        std::vector<Reservation>& reservations) {
    auto msg = basicMessage("calendar");
    //TODO
    return msg.dump();
}

const std::string CommunicationProtocol::createCancelFor(const Date& startDate) {
    auto msg = basicMessage("cancel");
    //TODO
    return msg.dump();
}

const std::string CommunicationProtocol::createCanceledFor(
        const bool isCancelled) {
    auto msg = basicMessage("canceled");
    //TODO
    return msg.dump();
}

const Reservation CommunicationProtocol::getReservation(
        const std::string& reservationMessage) {
    auto msg = json::parse(reservationMessage);
    Reservation res;
    res.start = Date::parse(msg["start"]);
    res.duration = msg["duration"];
    return res;
}

const std::pair<bool, Reservation> CommunicationProtocol::isReserved(
        const std::string& reservedMessage) {
    auto msg = json::parse(reservedMessage);
    return std::make_pair(msg["value"], getReservation(msg["overlap"]));
}

const void CommunicationProtocol::getUnlock(const std::string& unlockMessage) {
    auto msg = json::parse(unlockMessage);
    //TODO
    return;
}

const bool CommunicationProtocol::isUnlocked(
        const std::string& unlockedMessage) {
    auto msg = json::parse(unlockedMessage);
    //TODO
    return true;
}

const void CommunicationProtocol::getGetCalendar(
        const std::string& getCalendarMessage) {
    auto msg = json::parse(getCalendarMessage);
    //TODO
    return;
}

const std::vector<Reservation> CommunicationProtocol::getCalendar(
        const std::string& calendarMessage) {
    auto msg = json::parse(calendarMessage);
    //TODO
}

const Date CommunicationProtocol::getCancel(const std::string& cancelMessage) {
    auto msg = json::parse(cancelMessage);
    //TODO
}

const bool CommunicationProtocol::getCanceled(
        const std::string& canceledMessage) {
    auto msg = json::parse(canceledMessage);
    //TODO
    return true;
}

const json CommunicationProtocol::basicMessage(const std::string & title) {
    return { {"msg", title}};
}
