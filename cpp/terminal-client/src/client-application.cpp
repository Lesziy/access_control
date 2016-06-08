#include "client-application.h"

Client::Client(const std::string & ip, const std::string & port) {
    conn_ = ClientConnection::establishWith(ip, port);
}

Client::~Client() {
    conn_.clean();
}

void Client::start() {
    authenticate();
    while (running_)
    	executeCommand( chooseCommand() );
    utils::println("Application terminated successfuly.");
}

void Client::authenticate() {
    using namespace utils;
    using auth = AuthenticationProtocol;
    do {
        print("Enter login: ");
        login_ = getLine();

        print("Enter password: ");
        enablePasswordMode(true);
        auto password = getLine();
        enablePasswordMode(false);
        println();

        conn_.sendMessage(auth::makeHandshake(login_));
        auto challenge = auth::getChallenge(conn_.receiveMessage());
        std::string response = utils::hashPassword(password, challenge);
        conn_.sendMessage(auth::makeResponse(response));
    } while(!auth::isAuthenticated(conn_.receiveMessage()));
    println("Logged in.");
}

int Client::chooseCommand() {
    static const std::map<int, std::string> actions {
        { 1, "Reserve time on remote machine" },
        { 2, "Unlock my IP on remote machine" },
        { 3, "Cancel my reservation" },
        { 4, "Exit" }
    };

    utils::println("Choose action: ");
	for(auto & action : actions)
        utils::println("[" + std::to_string(action.first) + "] " + action.second);

    utils::print("Choice: ");
    auto decision = utils::getLine();
	return stoi(decision);
}

void Client::executeCommand(int decision) {
    static const std::map<int, std::function<void()>> handlers {
        { 1, [this]{ makeReservation(); } },
        { 2, [this]{ unlockIpAddress(); } },
        { 3, [this]{ cancelReservation(); } },
        { 4, [this]{ running_ = false; } }
    };

    auto optionalHandler = handlers.find(decision);
    if(optionalHandler != end(handlers))
        (optionalHandler->second)();
}

void Client::makeReservation() {
    do {
        try {
            utils::print("Date (ex. 01.01.2014 12:54:12): ");
            auto date = utils::getLine();

            utils::print("Duration (in hours): ");
            auto duration = utils::getLine();

            auto reservation = Reservation::create(date, std::stoi(duration));
            conn_.sendMessage(CommunicationProtocol::createReservationFor(reservation));
            break;
        } catch(std::runtime_error& err) {
            utils::println("Invalid input. Try once again.");
        }
    } while(true);

    auto isReserved = CommunicationProtocol::isReserved(conn_.receiveMessage());
    if(isReserved.first)
        utils::println("Reservation successful");
    else {
        utils::println("Specified date already reserved");
        utils::println(isReserved.second.startTimeToString() + " " +
                       std::to_string(isReserved.second.duration()) + " hours.");
    }
}

std::vector<Reservation> Client::getCalendar() {
    conn_.sendMessage(CommunicationProtocol::createGetCalendarFor());
    return CommunicationProtocol::getCalendar(conn_.receiveMessage());
}

void Client::cancelReservation() {
    utils::println("Getting info about your reservations...");
    auto all = getCalendar();
    all.erase(std::remove_if(all.begin(), all.end(), [this](Reservation& elem){ return elem.username() != login_; }),
              all.end());
    int counter = 1;
    for(auto& singleReservation: all)
        utils::println("[" + std::to_string(counter++ ) + "] " + singleReservation.startToString());
    utils::println("Choose date to cancel (by number in brackets or 0 to cancel): ");

    auto decision = utils::getLine();

    if(!utils::isInteger(decision)) {
        utils::println("Invalid input.");
        return;
    }

    unsigned long dec = std::stoul(decision);

    if(dec == 0)
        return;
    else if(dec < 1 || dec > counter) {
        utils::println("Not valid choice");
        return;
    }

    utils::print("Cancelling chosen date...");
    conn_.sendMessage(CommunicationProtocol::createCancelFor(all.at(dec - 1)));
    if(CommunicationProtocol::getCanceled(conn_.receiveMessage()))
        utils::println("Reservation cancelled.");
    else
        utils::println("Reservation couldn't be cancelled.");
}

void Client::unlockIpAddress() {
    conn_.sendMessage(CommunicationProtocol::createUnlockFor());
    utils::println("Unlock request sent.");
    if(CommunicationProtocol::isUnlocked(conn_.receiveMessage()))
        utils::println("IP unlocked.");
    else
        utils::println("IP couldn't be unlocked.");
}