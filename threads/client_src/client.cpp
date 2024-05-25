#include "client.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <sys/types.h>

#include "client_protocol.h"
#define COMMAND_ATTACK "Atacar"
#define COMMAND_READ "Leer"
#define COMMAND_EXIT "Salir"

Client::Client(const std::string& hostname, const std::string& port):
        close(false), clientprot(hostname, port), printer() {}


void Client::run() {
    std::string line = "";
    while (!this->close) {
        std::getline(std::cin, line);
        if (line == "") {
            continue;
        }
        std::vector<std::string> command_list = this->proccesLine(line);
        this->proccesCommand(command_list);
        if (this->clientprot.wasClosed()) {
            break;
        }
    }
}

// private methods

std::vector<std::string> Client::proccesLine(std::string& input) {
    std::istringstream stream(input);
    std::vector<std::string> commands;
    std::string word;
    while (stream >> word) {
        commands.push_back(word);
    }
    return commands;
}

void Client::proccesCommand(const std::vector<std::string>& commands) {
    if (commands.empty() || commands.size() > 2) {
        return;
    }
    this->attack(commands);
    this->readNLines(commands);
    this->exit(commands);
}

void Client::exit(const std::vector<std::string>& commands) {
    if (commands.at(0).compare(COMMAND_EXIT) != 0) {
        return;
    }
    this->close = true;
}


void Client::attack(const std::vector<std::string>& commands) {
    if (commands.at(0).compare(COMMAND_ATTACK) != 0) {
        return;
    }
    if (!this->clientprot.sendAttack()) {
        this->close = true;
    }
}

void Client::readNLines(const std::vector<std::string>& commands) {
    if (commands.size() < 2 || (commands.at(0).compare(COMMAND_READ) != 0)) {
        return;
    }

    std::string line_number = commands.at(1);
    int coverted = std::stoi(line_number);
    this->getLines(coverted);
    return;
}

void Client::getLines(const int& counter) {
    for (int i = 0; i < counter; ++i) {
        MessageFormat status = this->clientprot.listenSocketData();
        if (status.hasDefaultFormat()) {
            this->close = true;
            break;
        }
        printer.printMessage(status);
    }
}

Client::~Client() {}
