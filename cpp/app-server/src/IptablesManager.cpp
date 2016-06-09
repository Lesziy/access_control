//
// Created by secman on 08.06.16.
//

#include "IptablesManager.h"

void* timeGuardianFunction(void *data);

bool IptablesManager::unlock(std::string username, std::string userIP, std::string calendarPath) {
    Reservation reservation = checkIfReserved(username, calendarPath);

    if(reservation.username() == "")
        return false;

    std::string addCommand = "iptables -A INPUT -p tcp --dport 22 -s " + userIP + " -j ACCEPT";
    std::string deleteCommand = "iptables -D INPUT -p tcp --dport 22 -s " + userIP + " -j ACCEPT";

    system(addCommand.c_str());
    struct Msg {
        std::string command;
        std::string calendarPath;
        time_t endTime;
    } *msg;
    msg = new Msg();
    msg->command = deleteCommand;
    msg->calendarPath = calendarPath;
    msg->endTime = reservation.endTimeToTime_t();
    pthread_t timeGuardian;

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
    } *msg;

    msg = (Msg *) data;
    std::string deleteCommand = msg->command;
    std::string calendarPath = msg->calendarPath;
    time_t endTime = msg->endTime;
    delete (msg);

    time_t actualTime = utils::getActualUTCTime();

    unsigned int howLongSleep = (unsigned int)difftime(endTime, actualTime);

    sleep(howLongSleep);
    system(deleteCommand.c_str());

    std::vector<Reservation> reservations = CalendarManager::getReservations(calendarPath);
    for(int i = 0; i < reservations.size(); i++)
        if(difftime(endTime, reservations[i].endTimeToTime_t()) >= 0)
            CalendarManager::cancelReservation(calendarPath, reservations[i]);
}