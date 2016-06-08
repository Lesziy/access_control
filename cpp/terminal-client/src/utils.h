#pragma once

#include <algorithm>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "sha3.h"

namespace utils {
    bool isInteger(const std::string &s);
    void enablePasswordMode(bool enable);
    std::string hashPassword(const std::string & password,
                             const std::string & challenge);
    void print(std::string&& msg);
    void println(std::string&& msg = "");
    std::string getLine();
}