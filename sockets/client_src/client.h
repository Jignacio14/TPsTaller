#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <string>

#include "client_protocol.h"

class Client {
private:
    ClientProtocol clienprot;
    // cppcheck-suppress unusedStructMember
    bool is_running;
    // cppcheck-suppress unusedStructMember
    int final_status;

    void sendInputLine(std::string& message);

    void receiveString();

    bool checkErrors();

    std::list<std::string> cleanMessage(std::string& message);

public:
    Client(std::string& hostname, std::string& service);

    bool isRunning();

    void executeActions();

    int status();

    ~Client();
};
#endif
