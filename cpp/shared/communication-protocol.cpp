/*
 * CommunicationProtocol.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "communication-protocol.h"

const std::string CommunicationProtocol::createReservationFor(
        const Reservation& reservation) {
    auto msg = basicMessage("reservation");
    msg["start"] = reservation.startToString();
    msg["duration"] = reservation.duration();
    return msg.dump();
}

const std::string CommunicationProtocol::createReservedFor(
        const bool isReserved, const Reservation& reservation) {
    auto msg = basicMessage("reserved");
    msg["value"] = isReserved;
    if(isReserved)
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

const std::string CommunicationProtocol::createCancelFor(const Reservation& startDate) {
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
    return Reservation::create(msg["start"], msg["duration"]);
}

const std::pair<bool, Reservation> CommunicationProtocol::isReserved(
    const std::string& reservedMessage) {

    auto msg = json::parse(reservedMessage);
    bool success = msg["value"];
    return std::make_pair(success, success ? getReservation(msg["overlap"]) : Reservation::missingReservation);
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

const Reservation CommunicationProtocol::getCancel(const std::string& cancelMessage) {
    auto msg = json::parse(cancelMessage);
    //TODO
}

const bool CommunicationProtocol::getCanceled(
        const std::string& canceledMessage) {
    auto msg = json::parse(canceledMessage);
    //TODO
    return true;
}

const int CommunicationProtocol::getMessageType(const std::string &message) {
    static const std::map<std::string, int> msgTypes{
            {"handshake", 1},
            {"challenge", 2},
            {"response", 3},
            {"authenticated", 4},
            {"reservation", 5},
            {"reserved", 6},
            {"unlock", 7},
            {"unlocked", 8},
            {"getCalendar", 9},
            {"calendar", 10},
            {"cancel", 11},
            {"canceled", 12}
    };
    auto msg = json::parse(message);
    try {
        return msgTypes.at(msg["msg"]);
    }
    catch (const std::out_of_range& oor) {
        perror("ERROR bad message type");
        return -1;
    }
}

const json CommunicationProtocol::basicMessage(const std::string & title) {
    return { {"msg", title}};
}
