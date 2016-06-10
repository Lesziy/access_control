/*
 * CommunicationProtocol.cpp
 *
 *  Created on: 7 maj 2016
 *      Author: secman
 */

#include "communication-protocol.h"

json CommunicationProtocol::fromReservation(const Reservation& r) {
    if(r.username() == "")
        return json{ {"start", r.startToString()}, {"duration", r.duration()} };
    else
        return json{ {"start", r.startToString()}, {"duration", r.duration()}, {"username", r.username()} };
}

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
    if(!isReserved)
        msg["overlap"] = fromReservation(reservation);
    return msg.dump();
}

const std::string CommunicationProtocol::createUnlockFor() {
    return basicMessage("unlock").dump();
}

const std::string CommunicationProtocol::createUnlockedFor(
        const bool isUnlocked) {
    auto msg = basicMessage("unlocked");
    msg["value"] = isUnlocked;
    return msg.dump();
}

const std::string CommunicationProtocol::createCalendarFor(
        std::vector<Reservation>& reservations) {
    auto msg = basicMessage("calendar");

    std::vector<json> parsedReservations;
    for(auto& res : reservations)
        parsedReservations.push_back(fromReservation(res));

    msg["reservations"] = parsedReservations;
    return msg.dump();
}

const std::string CommunicationProtocol::createCancelFor(const Reservation& reservation) {
    auto msg = basicMessage("cancel");
    msg["start"] = reservation.startToString();
    return msg.dump();
}

const std::string CommunicationProtocol::createCanceledFor(
        const bool isCancelled) {
    auto msg = basicMessage("canceled");
    msg["value"] = isCancelled;
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
    return std::make_pair(success, !success ? getReservation(msg["overlap"].dump()) : Reservation::missingReservation);
}

const std::string CommunicationProtocol::ipToUnlock(const std::string &unlockMessage) {
    auto msg = json::parse(unlockMessage);
    return msg["ip"];
}

const bool CommunicationProtocol::isUnlocked(
        const std::string& unlockedMessage) {
    auto msg = json::parse(unlockedMessage);
    return msg["value"];
}

const std::vector<Reservation> CommunicationProtocol::getCalendar(
    const std::string& calendarMessage) {
    auto msg = json::parse(calendarMessage);
    auto container = msg["reservations"];
    std::vector<Reservation> reservations;
    for(auto& res: container) {
        reservations.push_back(Reservation::create(res["start"], res["duration"], res["username"]));
    }

    return reservations;
}

const Reservation CommunicationProtocol::getCancel(const std::string& cancelMessage) {
    auto msg = json::parse(cancelMessage);
    return Reservation::create(msg["start"], 0);
}

const bool CommunicationProtocol::getCanceled(
        const std::string& canceledMessage) {
    auto msg = json::parse(canceledMessage);
    return msg["value"];
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
            {"canceled", 12},
            {"getMyReservations", 13}
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

const std::string CommunicationProtocol::createMyReservationsRequest() {
    return basicMessage("getMyReservations").dump();
}

const std::vector<Reservation> CommunicationProtocol::getMyReservations(const std::string &myReservationsMessage) {
    return getCalendar(myReservationsMessage);
}

const std::string CommunicationProtocol::createMyReservationsResponse(std::vector<Reservation> &reservations) {
    auto msg = json::parse(createCalendarFor(reservations));
    msg["msg"] = "myReservations";
    return msg.dump();
}






