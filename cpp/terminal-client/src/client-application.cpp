#include "client-application.h"

ClientApplication::ClientApplication(const std::string & ip, const std::string & port) {
    conn_ = Connection::establishWith(ip, port);
}

ClientApplication::~ClientApplication() {
    conn_.clean();
}

void ClientApplication::run() {
    // authenticate();
    while (running_)
    	executeCommand( chooseCommand() );
    std::cout << "Terminating application..." << std::endl;
}

void ClientApplication::authenticate() {
    do {
        std::string login, password;

        std::cout << "Enter login: ";
        getline(std::cin, login);

        std::cout << "Enter password: ";
        setPasswordMode(true);
        getline(std::cin, password);
        setPasswordMode(false);
        std::cout << std::endl << "Authenticating..." << std::endl;

        conn_.sendMessage(autProt_.createHandshakeFor(login));
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
        { 3, []{ std::cout << "Not implemented yet" << std::endl; } },
        { 4, [this]{ running_ = false; } }
    };

    auto optionalHandler = handlers.find(decision);
    if(optionalHandler != end(handlers))
        (optionalHandler->second)();
}

void ClientApplication::reserveRemoteMachine() {
    std::cout << "Date (ex. 01.01.2014 12:54:12): ";
    std::string date;
    getline(std::cin, date);

    std::cout << "Duration (in hours): ";
    std::string duration;
    getline(std::cin, duration);

    try {
        auto reservation = Reservation::create(date, std::stoi(duration));
        conn_.sendMessage(CommunicationProtocol::createReservationFor(reservation) + "\n");
    } catch(std::runtime_error& error) {
        std::cout << error.what();
    }
}

void ClientApplication::setPasswordMode(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if(enable)
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}