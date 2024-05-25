#ifndef ACCEPTATOR_H
#define ACCEPTATOR_H

#include <exception>
#include <string>
#include <utility>
#include <vector>

#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "server_game.h"
#include "server_monitor.h"
#include "server_receiver.h"


class Acceptator: public Thread {
private:
    Socket listener;
    // cppcheck-suppress unusedStructMember
    std::vector<Receiver*> server_clients;
    Monitor& monitor_ref;
    Queue<std::string>& game_channel;

    void startClientHandler();
    void ereaseClient();
    void killAll();

public:
    // explicit Acceptator(const std::string& port);
    explicit Acceptator(const std::string& port, Monitor& monitor_ref,
                        Queue<std::string>& game_channel);
    void run() override;

    void kill();
};
#endif
