#include "server.h"

Server::Server(std::string& port):
        acceptator(port, this->monitor, this->comunication_channel),
        monitor(),
        game(this->monitor, this->comunication_channel) {}

void Server::executeServer() {
    try {
        game.start();
        acceptator.start();
        while (std::cin.get() != 'q') {}
        game.kill();
        game.join();
        acceptator.kill();
        acceptator.join();
    } catch (const std::exception& exception) {
        // printear mensaje de error;
    }
}
