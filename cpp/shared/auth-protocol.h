#pragma once
#include "json.hpp"

using json = nlohmann::json;

class AuthenticationProtocol {
public:
    static const std::string createHandshakeFor(const std::string & login);
    static const std::string createChallengeFor(const std::string & challenge);
    static const std::string createResponseFor(const std::string & password);
    static const std::string createAuthenticatedFor(const bool isAuthenticated);

    static const std::string getLogin(const std::string & handshakeMessage);
    static const std::string getChallenge(const std::string & challengeMessage);
    static const std::string getResponse(const std::string & responseMessage);
    static const bool isAuthenticated(const std::string & authenticationMessage);
private:
    template<typename T>
    static const std::string createMessageFor(const std::string & basicMsg, 
                                              const std::string & field, 
                                              const T & value);
    static const json basicMessage(const std::string & title);
};