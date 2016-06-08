#pragma once

#include <iostream>
#include <cstdio>
#include <map>
#include <functional>

#include "auth-protocol.h"
#include "client-connection.h"
#include "reservation.h"
#include "communication-protocol.h"
#include "utils.h"

class Client {
public:
    Client(const std::string &ip, const std::string &port);
    ~Client();
    void start();
private:
    void authenticate();
    void makeReservation();
    void unlockIpAddress();
    void cancelReservation();

    int chooseCommand();
    void executeCommand(int);
    std::vector<Reservation> getCalendar();
    
    ClientConnection conn_;
    bool running_ = true;
    std::string login_;
};