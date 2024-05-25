#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "server_acceptator.h"
#include "server_game.h"
#include "server_monitor.h"
#include "server_protocol.h"

class Server {
private:
    Acceptator acceptator;
    Monitor monitor;
    Game game;
    Queue<std::string> comunication_channel;

public:
    explicit Server(std::string& port);
    void executeServer();
};

#endif
