#pragma once

#include<string>
#include<algorithm>
#include <vector>

class JsonMessageBuffer {
public:
    using ValidMessages = std::vector<std::string>;
    ValidMessages parseMessages(const std::string &message);
private:
    std::string parse(const std::string & message, bool withBuffer = false);
    std::string::const_iterator parseStandalone(const std::string & message);
    void parseBufferOnly(ValidMessages &results);

    int bracesCounter = 0;
    std::string buffer = "";
};

