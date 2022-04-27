#include <iostream>
#include "Server.h"
#include "PGGSpy.h"


int main() {
    std::cout << "PGGSpy!" << std::endl;

    Server server;
    server.add<PGGSpy>();
    server.launch();

    while (true) {
        if (std::cin.get() == '0') {
            server.stop();
            exit(0);
        }
    }
}
