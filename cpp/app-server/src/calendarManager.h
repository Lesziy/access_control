//
// Created by secman on 06.06.16.
//

#ifndef ACCESS_CONTROL_CALENDARMANAGER_H
#define ACCESS_CONTROL_CALENDARMANAGER_H

#include <string>
#include "reservation.h"
#include "jsonFileManager.h"
#include "communication-protocol.h"

class calendarManager {
public:
    static bool addReservation(std::string path, Reservation res, std::string username);

private:
    static bool validateReservation(json calendar, Reservation res, std::string username);
};


#endif //ACCESS_CONTROL_CALENDARMANAGER_H
