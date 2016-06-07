//
// Created by secman on 06.06.16.
//

#ifndef ACCESS_CONTROL_CALENDARMANAGER_H
#define ACCESS_CONTROL_CALENDARMANAGER_H

#include <string>
#include "reservation.h"
#include "jsonFileManager.h"
#include "communication-protocol.h"

class CalendarManager {
public:
    static json createNewCalendar(std::string path, std::vector <Reservation> reservations);
    static bool addReservation(std::string path, Reservation res);
    static bool cancelReservation(std::string path, Reservation res);
    static std::vector <Reservation> getReservations(std::string path);
    static std::vector <Reservation> getReservations(std::string path, std::string username);
    static json getCalendar(std::string path);
private:
    static bool validateReservation(json calendar, Reservation res);
};


#endif //ACCESS_CONTROL_CALENDARMANAGER_H
