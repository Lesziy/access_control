#pragma once

#include<string>
#include<algorithm>
#include <vector>

class JsonMessageBuffer {
public:
    std::vector<std::string> parseMessages(const std::string &message);
private:
    std::string parseWithBuffered(const std::string & message);
    std::string parseStandalone(const std::string & message, bool store = true);
    void parseBufferOnly(std::vector<std::string> & results);

    int bracesCounter = 0;
    std::string buffer = "";
};

