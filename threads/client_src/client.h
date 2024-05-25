#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <string>
#include <vector>

#include "../common_src/common_message_constructor.h"

#include "client_protocol.h"
class Client {
private:
    // cppcheck-suppress unusedStructMember
    bool close;
    ClientProtocol clientprot;
    MessageConstructor printer;

    std::vector<std::string> proccesLine(std::string& input);
    void proccesCommand(const std::vector<std::string>& commands);
    void exit(const std::vector<std::string>& commands);
    void attack(const std::vector<std::string>& commands);
    void readNLines(const std::vector<std::string>& commands);
    void getLines(const int& counter);
    void readStdinCommand();

public:
    Client(const std::string& hostname, const std::string& port);
    void run();
    Client(const Client&) = delete;
    Client& operator=(Client&&) = delete;
    ~Client();
};

#endif
