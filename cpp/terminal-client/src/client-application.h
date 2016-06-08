#pragma once

#include <iostream>
#include <cstdio>
#include <map>
#include <termios.h>
#include <functional>

#include "auth-protocol.h"
#include "client-connection.h"
#include "reservation.h"
#include "sha3.h"
#include "communication-protocol.h"

class ClientApplication {
public:
    ClientApplication(const std::string & ip, const std::string & port);
    ~ClientApplication();
    void run();
private:
    void authenticate();
    std::string hashPassword(const std::string & password, const std::string & challenge);
    int chooseCommand();
    void executeCommand(int);
    void reserveRemoteMachine();
    void setPasswordMode(bool);
    
    ClientConnection conn_;
    AuthenticationProtocol autProt_;
    bool running_ = true;
};