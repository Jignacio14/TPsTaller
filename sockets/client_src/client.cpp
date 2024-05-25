#include "client.h"

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "client_protocol.h"


Client::Client(std::string& hostname, std::string& service):
        clienprot(hostname, service), is_running(false), final_status(0) {}

bool Client::isRunning() { return this->is_running; }

void Client::sendInputLine(std::string& message) {
    if (message == "") {
        return;
    }
    std::list<std::string> cleanString = cleanMessage(message);
    this->clienprot.sendCommandListToServer(cleanString);
}

void Client::receiveString() {
    std::string combo = this->clienprot.receiveProcessedCommands();
    if (this->checkErrors()) {
        final_status = 1;
        return;
    }
    std::cout << combo << std::endl;
}

std::list<std::string> Client::cleanMessage(std::string& message) {
    std::list<std::string> list_commands;
    std::stringstream commands(message);
    std::string command;
    while (commands >> command) {
        list_commands.push_back(command);
    }
    return list_commands;
}

bool Client::checkErrors() { return this->clienprot.checkError(); }

int Client::status() { return this->final_status; }

void Client::executeActions() {
    for (std::string line; !this->checkErrors() && std::getline(std::cin, line);) {
        this->sendInputLine(line);
        this->receiveString();
    }
}

Client::~Client() {}
