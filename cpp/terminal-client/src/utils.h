#pragma once

inline bool isInteger(const std::string &s) {
    return !s.empty()
           && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
