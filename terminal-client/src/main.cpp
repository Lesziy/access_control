#include "client-application.h"
#include <time.h>
int main(int argc, char** argv) {
    const auto ip = "209.85.225.103";
    const auto port = "80";
    ClientApplication app(ip, port);
    app.run();
}