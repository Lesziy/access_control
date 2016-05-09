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
    } while(autProt_.isAuthenticated(conn_.receiveMessage()));
}

std::string ClientApplication::hashPassword(const std::string & password, const std::string & challenge) {
    // TODO
    return password;
}

int ClientApplication::chooseCommand() {
	int decision;
	static const std::map<int, std::string> options{
		{ 1, "Reserve time on remote machine" },
		{ 2, "Unlock my IP on remote machine" },
		{ 3, "Cancel my reservation" },
		{ 4, "Exit" }
	};

    std::cout << "Choose action: " << std::endl;
	for(auto & option : options)
		std::cout << "[" << option.first << "] " << option.second << std::endl;

	std::cout << "Choice: ";
	std::cin >> decision;
	return decision;
}

void ClientApplication::executeCommand(int decision) {
	static const std::map<int, std::function<void(void)>> handlers{
        { 1, []{ std::cout << "Not implemented yet" << std::endl; } },
        { 2, []{ std::cout << "Not implemented yet" << std::endl; } },
        { 3, []{ std::cout << "Not implemented yet" << std::endl; } },
        { 4, [this]{ running_ = false; } }
    };

    auto optionalHandler = handlers.find(decision);
    if(optionalHandler != end(handlers)
       && optionalHandler->second != nullptr)
        (optionalHandler->second)();
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
