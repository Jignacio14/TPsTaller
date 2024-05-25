#include "server_protocol.h"

#include <string>
#include <vector>

#include <arpa/inet.h>

#define BUFFER_SIZE 1

#define JUMP "JUMP"
#define RIGHT "RIGHT"
#define LEFT "LEFT"
#define DUCK "DUCK"
#define HIT "HIT"
#define NOT "NOT"
#define COD_JUMP 0x01
#define COD_RIGHT 0x02
#define COD_LEFT 0x03
#define COD_DUCK 0x04
#define COD_HIT 0x05
#define COD_NOT 0x00

ServerProtocol::ServerProtocol(const std::string& port):
        skt(port.c_str()), was_close(false), error_ocurred(false) {}

const std::unordered_map<u_int8_t, std::string> ServerProtocol::translationMap = {
        {COD_JUMP, JUMP}, {COD_RIGHT, RIGHT}, {COD_LEFT, LEFT}, {COD_DUCK, DUCK}, {COD_HIT, HIT}};

std::string ServerProtocol::receive() {
    std::string commands;
    bool reading = false;
    while (!was_close) {
        u_int8_t buf[BUFFER_SIZE] = {0};
        skt.recvall(buf, sizeof(buf), &this->was_close);
        if (was_close) {
            /// Si estaba leyendo claramente es error, en caso contrario el cliente se cerro y el
            /// programa debe cerrarse correctamente
            this->setErrorFlag(reading);
            break;
        }
        if (buf[0] != COD_NOT) {
            std::string command = decode(buf[0]);
            commands.append(command);
            commands.append(" ");
            reading = true;
        }
        if (buf[0] == COD_NOT) {
            // cppcheck-suppress unreadVariable
            reading = false;
            break;
        }
    }
    if (!commands.empty()) {
        commands.pop_back();
    }
    return commands;
}

void ServerProtocol::accept() { this->skt = skt.accept(); }

bool ServerProtocol::isRunning() { return !this->was_close; }

void ServerProtocol::sendCommand(u_int16_t len, const std::vector<char>& data) {
    if (was_close) {
        return;
    }
    this->sendString(data, len);
}

bool ServerProtocol::checkError() { return this->error_ocurred; }

// Private Methods

std::string ServerProtocol::decode(u_int8_t code) {
    auto it = translationMap.find(code);
    return (it != translationMap.end()) ? it->second : std::string(NOT);
}

void ServerProtocol::sendString(const std::vector<char>& data, u_int16_t size) {
    this->send2ByteNumber(size);
    this->sendStringData(data, size);
}

void ServerProtocol::send2ByteNumber(u_int16_t len) {
    u_int16_t networkFormat = htons(len);
    this->skt.sendall(&networkFormat, sizeof(len), &was_close);
    this->setErrorFlag(was_close);
}

void ServerProtocol::sendStringData(const std::vector<char>& data, u_int16_t size) {
    this->skt.sendall(data.data(), size, &was_close);
    this->setErrorFlag(was_close);
}

void ServerProtocol::setErrorFlag(bool condition) {
    if (condition) {
        this->error_ocurred = true;
    }
}

ServerProtocol::~ServerProtocol() {}
