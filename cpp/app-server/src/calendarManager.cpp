//
// Created by secman on 06.06.16.
//

#include "calendarManager.h"

json CalendarManager::createNewCalendar(std::string path, std::vector<Reservation> reservations) {
    json calendar;
    for(int i = 0; i < reservations.size(); i++) {
        calendar["reservations"][i]["date"] = reservations[i].startDateToString();
        calendar["reservations"][i]["interval_in_hours"] = reservations[i].duration();
        calendar["reservations"][i]["start_hour"] = reservations[i].startTimeToString();
        calendar["reservations"][i]["username"] = reservations[i].username();
    }

    jsonFileManager::saveFile(path, calendar);

    return calendar;
}

bool CalendarManager::addReservation(std::string path, Reservation res) {
    int index = 0;

    auto data = jsonFileManager::getJson(path);

    if(!validateReservation(data, res))
        return false;

    for (auto& element : data["reservations"]) {
        index++;
    }
    data["reservations"][index]["date"] = res.startDateToString();
    data["reservations"][index]["interval_in_hours"] = res.duration();
    data["reservations"][index]["start_hour"] = res.startTimeToString();
    data["reservations"][index]["username"] = res.username();
    jsonFileManager::saveFile(path, data);
    return true;
}

bool CalendarManager::cancelReservation(std::string path, Reservation res) {
    std::vector <Reservation> reservations = getReservations(path);
    bool isFound = false;

    for(int i = 0; i < reservations.size(); i++) {
        if(reservations[i].username() == res.username() && reservations[i].startToString() == res.startToString())
        {
            reservations.erase(reservations.begin() + i);
            isFound = true;
        }
    }

    if(isFound) {
        createNewCalendar(path, reservations);
        return true;
    }
    else {
        return false;
    }
}

std::vector <Reservation> CalendarManager::getReservations(std::string path){
    std::vector <Reservation> reservations;
    auto calendar = jsonFileManager::getJson(path);

    for (auto& element : calendar["reservations"]) {
        std::string date = element["date"];
        std::string time = element["start_hour"];
        std::string username = element["username"];
        int interval = element["interval_in_hours"];
        reservations.push_back(Reservation::create(date + " " + time, interval, username));
    }
    return reservations;
}

std::vector <Reservation> CalendarManager::getReservations(std::string path, std::string username){
    std::vector <Reservation> reservations;
    auto calendar = jsonFileManager::getJson(path);

    for (auto& element : calendar["reservations"]) {
        if(element["username"] == username) {
            std::string date = element["date"];
            std::string time = element["start_hour"];
            std::string username = element["username"];
            int interval = element["interval_in_hours"];
            reservations.push_back(Reservation::create(date + " " + time, interval, username));
        }
    }
    return reservations;
}

json CalendarManager::getCalendar(std::string path) {
    return jsonFileManager::getJson(path);
}

bool CalendarManager::validateReservation(json calendar, Reservation res) {
    time_t resBegin = res.startTimeToTime_t();
    time_t resEnd = res.endTimeToTime_t();

    for (auto& element : calendar["reservations"]) {
        if(element["date"] == res.startDateToString() && element["interval_in_hours"] == res.duration() && element["start_hour"] == res.startTimeToString() && element["username"] == res.username())
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