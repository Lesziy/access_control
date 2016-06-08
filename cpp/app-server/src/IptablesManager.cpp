//
// Created by secman on 08.06.16.
//

#include "IptablesManager.h"

void* timeGuardianFunction(void *data);

bool IptablesManager::unlock(std::string username, std::string port, std::string userIP, std::string calendarPath) {
    time_t endReservationTime = checkIfReserved(username, calendarPath);
    if(endReservationTime == 0)
        return false;

    std::string addCommand = "iptables -A INPUT -p tcp --dport " + port + " -s " + userIP + " -j ACCEPT";
    std::string deleteCommand = "iptables -D INPUT -p tcp --dport " + port + " -s " + userIP + " -j ACCEPT";

    system(addCommand.c_str());
    struct Msg {
        std::string command;
        time_t endTime;
    } *msg;

    msg = new Msg();
    msg->command = deleteCommand;
    msg->endTime = endReservationTime;
    pthread_t timeGuardian;

    pthread_create(&timeGuardian, NULL, timeGuardianFunction, (void*) msg);
    return true;
}

time_t IptablesManager::checkIfReserved(std::string username, std::string calendarPath) {
    std::vector<Reservation> reservations = CalendarManager::getReservations(calendarPath, username);

    time_t actualTime;
    time(&actualTime);
    tm* act = localtime(&actualTime);
    actualTime = timegm(act);

    for(int i = 0; i < reservations.size(); i++) {
        if(difftime(actualTime, reservations[i].startTimeToTime_t()) >= 0 &&
                difftime(reservations[i].endTimeToTime_t(), actualTime) > 0)
        return reservations[i].endTimeToTime_t();
    }
    return time_t (0);
}

void* timeGuardianFunction(void *data) {
    struct Msg {
        std::string command;
        time_t endTime;
    } *msg;

    msg = (Msg *) data;
    std::string deleteCommand = msg->command;
    time_t endTime = msg->endTime;
    delete (msg);

    time_t actualTime;
    time(&actualTime);
    tm* act = localtime(&actualTime);
    actualTime = timegm(act);

    unsigned int howLongSleep = (unsigned int)difftime(endTime, actualTime);

    sleep(howLongSleep);
    system(deleteCommand.c_str());
}