//
// Created by secman on 06.06.16.
//

#include "calendarManager.h"

bool calendarManager::addReservation(std::string path, Reservation res, std::string username) {
    int index = 0;

    auto data = jsonFileManager::getJson(path);

    if(!validateReservation(data, res, username))
        return false;

    for (auto& element : data["reservations"]) {
        index++;
    }
    data["reservations"][index]["date"] = res.startDateToString();
    data["reservations"][index]["interval_in_hours"] = res.duration();
    data["reservations"][index]["start_hour"] = res.startTimeToString();
    data["reservations"][index]["username"] = username;
    jsonFileManager::saveFile(path, data);
    return true;
}

bool calendarManager::validateReservation(json calendar, Reservation res, std::string username) {
    time_t resBegin = res.startTimeToTime_t();
    time_t resEnd = res.endTimeToTime_t();

    for (auto& element : calendar["reservations"]) {
        int i = element["interval_in_hours"];
        if(element["date"] == res.startDateToString() && element["interval_in_hours"] == res.duration() && element["start_hour"] == res.startTimeToString() && element["username"] == username)
        {
            //this user have already reserved this machine on that time
            return false;
        }

        std::string date = element["date"];
        std::string time = element["start_hour"];
        int interval = element["interval_in_hours"];
        Reservation elemReservation = Reservation::create(date + " " + time, interval);

        time_t elemBegin = elemReservation.startTimeToTime_t();
        time_t elemEnd = elemReservation.endTimeToTime_t();

        //check if reservation not collided with another reservations
        if((difftime(elemBegin, resBegin) > 0 && difftime(resEnd, elemBegin) > 0) ||                //reservations last when another begin
                (difftime(elemEnd, resBegin) > 0 && difftime(resEnd, elemEnd) > 0) ||               //reservations begin when another last
                (difftime(resBegin, elemBegin) > 0 && difftime(elemEnd, resEnd) > 0) ||             //reservation begin and end when another last
                (difftime(elemBegin, resBegin) > 0 && difftime(resEnd, elemEnd) > 0))               //reservation last when another begin and end
        {
            // this reservation cannot be done, because of another reservations
            return false;
        }
    }
    return true;
}
