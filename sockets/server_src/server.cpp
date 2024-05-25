#include "server.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "server_protocol.h"

#define COD_UPPERCUT "JUMP JUMP HIT"
#define COD_SIDEKICK "HIT DUCK LEFT"
#define COD_HIGHKICK "LEFT RIGHT JUMP HIT"
#define UPPERCUT "UPPERCUT"
#define HIGHKICK "HIGHKICK"
#define SIDEKICK "SIDEKICK"


Server::Server(std::string& puerto):
        counter(0), is_running(false), servprot(puerto), finalStatus(0) {}


bool Server::isRunning() {
    is_running = this->servprot.isRunning() && !this->servprot.checkError();
    if (this->servprot.checkError()) {
        finalStatus = 1;
    }
    return is_running;
}

std::string Server::asyncGet() {
    std::string commands = this->servprot.receive();
    std::string conCombos = matchCombos(commands);
    countCombos(conCombos);
    return conCombos;
}

void Server::printActions() { std::cout << "Actions: " << counter << std::endl; }

void Server::accept() { servprot.accept(); }

void Server::returnProccesedCommand(std::string& command) {
    u_int16_t len = command.length();
    std::vector<char> data(command.begin(), command.end());
    this->servprot.sendCommand(len, data);
}

int Server::status() { return finalStatus; }

void Server::executeActions() {
    while (this->isRunning()) {
        std::string commands = this->asyncGet();
        this->returnProccesedCommand(commands);
    }
    this->printActions();
}

/// Private methods

std::string Server::matchCombos(const std::string& commands) {
    std::string primerFiltro = replaceString(commands, COD_UPPERCUT, UPPERCUT);
    std::string segundoFiltro = replaceString(primerFiltro, COD_SIDEKICK, SIDEKICK);
    std::string tercerFiltro = replaceString(segundoFiltro, COD_HIGHKICK, HIGHKICK);
    return tercerFiltro;
}

std::string Server::replaceString(const std::string& original, const std::string& target,
                                  const std::string& replacement) {
    std::string modified = original;
    size_t startPos = 0;
    while ((startPos = modified.find(target, startPos)) != std::string::npos) {
        modified.replace(startPos, target.length(), replacement);
        startPos += replacement.length();
    }
    return modified;
}

void Server::countCombos(const std::string& combos) {
    std::stringstream stream(combos);
    counter += std::distance(std::istream_iterator<std::string>(stream),
                             std::istream_iterator<std::string>());
}

Server::~Server() {}
