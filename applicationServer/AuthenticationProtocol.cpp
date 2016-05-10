/*
 * AuthenticationProtocol.cpp
 *
 *  Created on: 5 maj 2016
 *      Author: secman
 */

#include "AuthenticationProtocol.h"

template<typename T>
const std::string AuthenticationProtocol::createMessageFor(
        const std::string & basicMsg, const std::string & field,
        const T & value) {
    auto msg = basicMessage(basicMsg);
    msg[field] = value;
    return msg.dump();
}

const std::string AuthenticationProtocol::createHandshakeFor(
        const std::string & login) {
    return createMessageFor("handshake", "login", login);
}

const std::string AuthenticationProtocol::createChallengeFor(
        const std::string & challenge) {
    return createMessageFor("challenge", "challenge", challenge);
}

const std::string AuthenticationProtocol::createResponseFor(
        const std::string & password) {
    return createMessageFor("response", "password", password);
}

const std::string AuthenticationProtocol::createAuthenticatedFor(
        const bool isAuthenticated) {
    return createMessageFor("authenticated", "value", isAuthenticated);
}

const std::string AuthenticationProtocol::getLogin(
        const std::string & message) {
    auto msg = json::parse(message);
    return msg["login"];
}

const std::string AuthenticationProtocol::getChallenge(
        const std::string & message) {
    auto msg = json::parse(message);
    return msg["challenge"];
}

const std::string AuthenticationProtocol::getResponse(
        const std::string & message) {
    auto msg = json::parse(message);
    return msg["password"];
}

const bool AuthenticationProtocol::isAuthenticated(
        const std::string & message) {
    auto msg = json::parse(message);
    return msg["authenticated"];
}

const json AuthenticationProtocol::basicMessage(const std::string & title) {
    return { {"msg", title}};
}

