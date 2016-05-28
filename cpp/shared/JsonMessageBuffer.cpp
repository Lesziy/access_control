#include "JsonMessageBuffer.h"

std::vector<std::string> JsonMessageBuffer::parseMessages(const std::string &message) {
    std::vector<std::string> parsedMessages;
    if(bracesCounter != 0) // previous message couldn't be fully parsed
        parsedMessages.push_back(parseWithBuffered(message));
    else // previous message was fully parsed
        parsedMessages.push_back(parseStandalone(message));
    parseBufferOnly(parsedMessages); // if single input provides more than one message the others are in buffer now
    parsedMessages.erase(std::remove_if(begin(parsedMessages), end(parsedMessages), [](const auto& elem) { return elem == ""; }),
                         end(parsedMessages));
    return parsedMessages;
}

std::string JsonMessageBuffer::parseWithBuffered(const std::string &message) {
    auto result = parseStandalone(message);
    if(result != "")
        return buffer + result;
    return result;
}

std::string JsonMessageBuffer::parseStandalone(const std::string &message, bool store) {
    for (auto it = message.begin(); it != message.end() ; ++it) {
        auto letter = *it;
        if(letter == '{')
            ++bracesCounter;
        else if(letter == '}') {
            --bracesCounter;
            if(bracesCounter == 0) {
                auto ret = std::string(message.begin(), it + 1);
                buffer = std::string(it + 1, message.end());
                return ret;
            }
        }
    }
    if(store)
        buffer += std::string(message.begin(), message.end());
    return "";
}

void JsonMessageBuffer::parseBufferOnly(std::vector<std::string> & results) {
    int currentCounter = bracesCounter;
    bracesCounter = 0;
    while(true) {
        auto onlyFromBuffer = parseStandalone(buffer, false);
        if(onlyFromBuffer == "")
            break;
        results.push_back(onlyFromBuffer);
    }
    bracesCounter = currentCounter;
}