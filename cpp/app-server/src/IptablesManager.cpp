//
// Created by secman on 08.06.16.
//

#include "IptablesManager.h"

Reservation IptablesManager::lastUnlockedRes = Reservation::create("01.01.1000 00:00:00", 0);

void* timeGuardianFunction(void *data);

bool IptablesManager::unlock(std::string username, std::string userIP, std::string calendarPath, Logger* logger) {
    Reservation reservation = checkIfReserved(username, calendarPath);

    if(reservation.username() == "")
        return false;

    if(lastUnlockedRes == reservation)
        return true;
    else
        lastUnlockedRes = Reservation::create(reservation);

    std::string addCommand = "iptables -I INPUT 1 -p tcp --dport 22 -s " + userIP + " -j ACCEPT";
    std::string deleteCommand = "iptables -D INPUT -p tcp --dport 22 -s " + userIP + " -j ACCEPT";

    system(addCommand.c_str());

    struct Msg {
        std::string command;
        std::string calendarPath;
        time_t endTime;
        Logger* logger;
        std::string userIP;
    } *msg;
    msg = new Msg();
    msg->command = deleteCommand;
    msg->calendarPath = calendarPath;
    msg->endTime = reservation.endTimeToTime_t();
    msg->logger = logger;
    msg->userIP = userIP;
    pthread_t timeGuardian;

    logger->log(userIP, username, std::string("UNLOCKED"), std::string(""));
    pthread_create(&timeGuardian, NULL, timeGuardianFunction, (void*) msg);
    return true;
}

Reservation IptablesManager::checkIfReserved(std::string username, std::string calendarPath) {
    std::vector<Reservation> reservations = CalendarManager::getReservations(calendarPath, username);

    time_t actualTime = utils::getActualUTCTime();

    for(int i = 0; i < reservations.size(); i++) {
        if(difftime(actualTime, reservations[i].startTimeToTime_t()) >= 0 &&
                difftime(reservations[i].endTimeToTime_t(), actualTime) > 0)
        return reservations[i];
    }
    return Reservation::missingReservation;
}

void* timeGuardianFunction(void *data) {
    struct Msg {
        std::string command;
        std::string calendarPath;
        time_t endTime;
        Logger* logger;
        std::string userIP;
    } *msg;

    msg = (Msg *) data;
    std::string deleteCommand = msg->command;
    std::string calendarPath = msg->calendarPath;
    time_t endTime = msg->endTime;
    Logger* logger = msg->logger;
    std::string userIP = msg->userIP;
    delete (msg);

    time_t actualTime = utils::getActualUTCTime();
    unsigned int howLongSleep = (unsigned int)difftime(endTime, actualTime);

    sleep(howLongSleep);

    system(deleteCommand.c_str());

    logger->log(userIP, std::string("SERVER"), std::string("IPTABLES_TIMEOUT"), std::string(""));

    std::vector<Reservation> reservations = CalendarManager::getReservations(calendarPath);
    for(int i = 0; i < reservations.size(); i++)
        if(difftime(endTime, reservations[i].endTimeToTime_t()) >= 0)
            CalendarManager::cancelReservation(calendarPath, reservations[i]);
}