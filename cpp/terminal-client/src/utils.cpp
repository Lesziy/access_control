#include "utils.h"

bool utils::isInteger(const std::string &s) {
    return !s.empty()
           && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void utils::enablePasswordMode(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    enable ? tty.c_lflag &= ~ECHO : tty.c_lflag |= ECHO;
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

std::string utils::hashPassword(const std::string & password, const std::string & challenge) {
    SHA3 sha3;
    std::string passwordHash = sha3(password);
    std::string response = challenge;
    response.append(passwordHash);
    response = sha3(response);
    return response;
}

void utils::print(std::string &&msg) {
    std::cout << msg;
}

void utils::println(std::string &&msg) {
    std::cout << msg << std::endl;
}