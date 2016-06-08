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
    using auth = AuthenticationProtocol;
    do {
        utils::print("Enter login: ");
        getline(std::cin, login_);

        utils::print("Enter password: ");
        utils::enablePasswordMode(true);
        std::string password;
        getline(std::cin, password);
        utils::enablePasswordMode(false);
        utils::println();

        conn_.sendMessage(auth::makeHandshake(login_));
        auto challenge = auth::getChallenge(conn_.receiveMessage());
        std::string response = utils::hashPassword(password, challenge);
        conn_.sendMessage(auth::makeResponse(response));
    } while(!auth::isAuthenticated(conn_.receiveMessage()));
    utils::println("Logged in.");
}

int Client::chooseCommand() {
	static const std::map<int, std::string> options{
		{ 1, "Reserve time on remote machine" },
		{ 2, "Unlock my IP on remote machine" },
		{ 3, "Cancel my reservation" },
		{ 4, "Exit" }
	};

    std::cout << "Choose action: " << std::endl;
	for(auto & option : options)
		std::cout << "[" << option.first << "] " << option.second << std::endl;

    std::string decision;
	std::cout << "Choice: ";
    getline(std::cin, decision);
	return stoi(decision);
}

void Client::executeCommand(int decision) {
	static const std::map<int, std::function<void()>> handlers{
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
            std::cout << "Date (ex. 01.01.2014 12:54:12): ";
            std::string date;
            getline(std::cin, date);

            std::cout << "Duration (in hours): ";
            std::string duration;
            getline(std::cin, duration);
            auto reservation = Reservation::create(date, std::stoi(duration));
            conn_.sendMessage(CommunicationProtocol::createReservationFor(reservation));
            break;
        } catch(std::runtime_error& err) {
            std::cout << "Invalid input. Try once again." << std::endl;
        }
    } while(true);

    try {
        auto isReserved = CommunicationProtocol::isReserved(conn_.receiveMessage());
        if(isReserved.first)
            std::cout << "Reservation successful" << std::endl;
        else {
            std::cout << "Specified date already reserved" << std::endl;
            std::cout << isReserved.second.startTimeToString() << " "
                      << isReserved.second.duration() << " hours." << std::endl;
        }
    } catch(std::runtime_error& error) {
        std::cout << error.what();
    }
}

std::vector<Reservation> Client::getCalendar() {
    conn_.sendMessage(CommunicationProtocol::createGetCalendarFor());
    return CommunicationProtocol::getCalendar(conn_.receiveMessage());
}

void Client::cancelReservation() {
    std::cout << "Getting info about your reservations..." << std::endl;
    auto all = getCalendar();
    all.erase(std::remove_if(all.begin(), all.end(), [this](Reservation& elem){ return elem.username() != login_; }),
              all.end());
    int counter = 1;
    for(auto& singleReservation: all)
        std::cout << "[" << counter++ << "] " << singleReservation.startToString() << std::endl;
    std::cout << "Choose date to cancel (by number in brackets or 0 to cancel): ";

    std::string decision;
    getline(std::cin, decision);
    unsigned long dec = std::stoul(decision);

    if(!utils::isInteger(decision)) {
        std::cout << "Invalid number provided." << std::endl;
    } else if(dec == 0)
        return;
    else if(dec < 1 || dec > counter) {
        std::cout << "Invalid number provided." << std::endl;
        return;
    }

    std::cout << "Cancelling chosen date...";
    conn_.sendMessage(CommunicationProtocol::createCancelFor(all.at(dec - 1)));
    if(CommunicationProtocol::getCanceled(conn_.receiveMessage()))
        std::cout << "Reservation cancelled." << std::endl;
    else
        std::cout << "Reservation couldn't be cancelled." << std::endl;
}

void Client::unlockIpAddress() {
    conn_.sendMessage(CommunicationProtocol::createUnlockFor());
    std::cout << "Unlock request sent." << std::endl;
    if(CommunicationProtocol::isUnlocked(conn_.receiveMessage()))
        std::cout << "IP unlocked." << std::endl;
    else
        std::cout << "IP couldn't be unlocked." << std::endl;
}