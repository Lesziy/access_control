#include "JsonMessageBuffer.h"

/**
 * Return the iterator pointing to the closing brace of the json
 */
std::string::const_iterator JsonMessageBuffer::parseStandalone(const std::string &message) {
    for (auto it = message.begin(); it != message.end() ; ++it) {
        auto letter = *it;
        if(letter == '{')
            ++bracesCounter;
        else if(letter == '}') {
            --bracesCounter;
            if(bracesCounter == 0)
                return it;
        }
    }
    return message.end();
}

JsonMessageBuffer::ValidMessages JsonMessageBuffer::parseMessages(const std::string &message) {
    JsonMessageBuffer::ValidMessages parsedMessages;
    std::string first;
    first = parse(message, true);
    if(first != "") {
        parsedMessages.push(first);
        parseBufferOnly(parsedMessages);
    }
    return parsedMessages;
}

std::string JsonMessageBuffer::parse(const std::string &message, bool withBuffer) {
    auto it = parseStandalone(message);
    if(it == message.end()) {
        if(withBuffer)
         buffer += message;
        return "";
    }
    std::string result = withBuffer ? buffer : "";
    result += std::string(message.cbegin(), it + 1);
    buffer = std::string(it + 1, message.cend());
    return result;
}

void JsonMessageBuffer::parseBufferOnly(JsonMessageBuffer::ValidMessages & results) {
    while(true) {
        auto it = parseStandalone(buffer);
        if(it == buffer.end())
            break;
        results.push(std::string(buffer.cbegin(), it + 1));
        buffer = std::string(it + 1, buffer.cend());
    }
}

