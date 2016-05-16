#include "client-application.h"

int main(int argc, char** argv) {
    const auto ip = "127.0.0.1";
    const auto port = "3490";
    ClientApplication app(ip, port);
    try {
        app.run();
    } catch(std::runtime_error & err) {
        if(!std::string(err.what()).compare("SERVER_DISCONNECTED"))
            std::cout << "Server disconnected" << std::endl;
        return 0;
    }
}