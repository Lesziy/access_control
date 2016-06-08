#include "client-application.h"

ClientApplication::ClientApplication(const std::string & ip, const std::string & port) {
    conn_ = ClientConnection::establishWith(ip, port);
}

ClientApplication::~ClientApplication() {
    conn_.clean();
}

void ClientApplication::run() {
    authenticate();
    while (running_)
    	executeCommand( chooseCommand() );
    std::cout << "Terminating application..." << std::endl;
}

void ClientApplication::authenticate() {
    do {
        std::cout << "Enter login: ";
        getline(std::cin, login_);

        std::cout << "Enter password: ";
        setPasswordMode(true);
        std::string password;
        getline(std::cin, password);
        setPasswordMode(false);
        std::cout << std::endl << "Authenticating..." << std::endl;

        conn_.sendMessage(autProt_.createHandshakeFor(login_));
        auto challenge = autProt_.getChallenge(conn_.receiveMessage());
        std::string response = hashPassword(password, challenge);
        conn_.sendMessage(autProt_.createResponseFor(response));
    } while(!autProt_.isAuthenticated(conn_.receiveMessage()));
}

std::string ClientApplication::hashPassword(const std::string & password, const std::string & challenge) {
    SHA3 sha3;
    std::string passwordHash = sha3(password);
    std::string response = challenge;
    response.append(passwordHash);
    response = sha3(response);
    return response;
}

int ClientApplication::chooseCommand() {
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

void ClientApplication::executeCommand(int decision) {
	static const std::map<int, std::function<void()>> handlers{
        { 1, [this]{ reserveRemoteMachine(); } },
        { 2, []{ std::cout << "Not implemented yet" << std::endl; } },
        { 3, [this]{ showMyReservations(); } },
        { 4, [this]{ running_ = false; } }
    };

    auto optionalHandler = handlers.find(decision);
    if(optionalHandler != end(handlers))
        (optionalHandler->second)();
}

void ClientApplication::reserveRemoteMachine() {

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

void ClientApplication::setPasswordMode(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    enable ? tty.c_lflag &= ~ECHO : tty.c_lflag |= ECHO;
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

std::vector<Reservation> ClientApplication::getCalendar() {
    conn_.sendMessage(CommunicationProtocol::createGetCalendarFor());
    return CommunicationProtocol::getCalendar(conn_.receiveMessage());
}

void ClientApplication::showMyReservations() {
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
    int dec = std::stoi(decision);

    if(!isInteger(decision)) {
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
        std::cout << "Reservation cancelled.";
    else
        std::cout << "Reservation couldn't be cancelled.";
}



