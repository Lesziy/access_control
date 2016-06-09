//
// Created by secman on 08.06.16.
//

//We have to remember that in iptables should be rule:
//  iptables -A INPUT -p tcp --dport 22 -j DROP
// this rule blocked every connection to port ssh - 22
// we can achieve that by command: sudo iptables -D INPUT -p tcp --dport 22 -j DROP

#ifndef ACCESS_CONTROL_IPTABLESMANAGER_H
#define ACCESS_CONTROL_IPTABLESMANAGER_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "utils.h"
#include "calendarManager.h"

class IptablesManager {
public:
    static bool unlock(std::string username, std::string userIP, std::string calendarPath);

private:
    static Reservation checkIfReserved(std::string username, std::string calendarPath);
};


#endif //ACCESS_CONTROL_IPTABLESMANAGER_H
