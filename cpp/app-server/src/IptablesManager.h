//
// Created by secman on 08.06.16.
//

//We have to remember that in iptables should be rule:
//  iptables -A INPUT -p tcp --dport <OURPORT> -j DROP
// this rule blocked every connection to port OURPORT
// we can achieve that by command: sudo iptables -D INPUT -p tcp --dport <OURPORT> -j DROP

#ifndef ACCESS_CONTROL_IPTABLESMANAGER_H
#define ACCESS_CONTROL_IPTABLESMANAGER_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "calendarManager.h"

class IptablesManager {
public:
    static bool unlock(std::string username, std::string port, std::string userIP, std::string calendarPath);

private:
    static time_t checkIfReserved(std::string username, std::string calendarPath);
};


#endif //ACCESS_CONTROL_IPTABLESMANAGER_H
