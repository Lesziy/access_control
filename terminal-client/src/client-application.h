#pragma once

#include <iostream>
#include <cstdio>
#include <map>
#include <termios.h>

#include "auth-protocol.h"
#include "connection.h"

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
    void setPasswordMode(bool);
    
    Connection conn_;
    AuthenticationProtocol autProt_;
    bool running_ = true;
};