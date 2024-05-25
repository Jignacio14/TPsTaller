#include "client_protocol.h"

#include <cstdint>
#include <string>
#include <unordered_map>

#include <arpa/inet.h>

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

const std::unordered_map<std::string, u_int8_t> ClientProtocol::translationMap = {
        {JUMP, COD_JUMP}, {RIGHT, COD_RIGHT}, {LEFT, COD_LEFT}, {DUCK, COD_DUCK}, {HIT, COD_HIT}};

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& service):
        skt(hostname.c_str(), service.c_str()), was_close(false), error_ocurred(false) {}


std::string ClientProtocol::receiveProcessedCommands() {
    uint16_t size = recv2ByteNumber();
    std::vector<char> combo = this->receiveString(size);
    std::string result(combo.begin(), combo.end());
    return result;
}

bool ClientProtocol::checkError() { return this->error_ocurred; }

void ClientProtocol::sendCommandListToServer(std::list<std::string>& commands) {
    for (auto& command: commands) {
        this->sendCommandToServer(command);
        if (this->setSendErrorFlag()) {
            return;
        }
    }
    this->sendFinalElement();
}

ClientProtocol::~ClientProtocol() {}

// Private Methods

void ClientProtocol::sendCommandToServer(std::string& message) {
    u_int8_t traslated = traslateMessengeToClientLanguage(message);
    int sz = skt.sendall(&traslated, sizeof(traslated), &was_close);
    this->setRecivedErrorFlag(sz, sizeof(traslated));
}

void ClientProtocol::sendFinalElement() {
    std::string endLineCommand(NOT);
    this->sendCommandToServer(endLineCommand);
}

u_int8_t ClientProtocol::traslateMessengeToClientLanguage(std::string& command) {
    auto it = translationMap.find(command);
    return (it != translationMap.end()) ? it->second : COD_NOT;
}

uint16_t ClientProtocol::recv2ByteNumber() {
    uint16_t buf[1];
    int sz = this->skt.recvall(buf, sizeof(buf), &was_close);
    this->setRecivedErrorFlag(sizeof(buf), sz);
    uint16_t localFormat = ntohs(buf[0]);
    return localFormat;
}

std::vector<char> ClientProtocol::receiveString(uint16_t size) {
    std::vector<char> vect(size, '\0');
    int sz = this->skt.recvall(vect.data(), size, &was_close);
    this->setRecivedErrorFlag(sz, size);
    return vect;
}

void ClientProtocol::setRecivedErrorFlag(size_t len, size_t otherlen) {
    if (len != otherlen) {
        error_ocurred = true;
    }
}

bool ClientProtocol::setSendErrorFlag() {
    if (this->was_close) {
        error_ocurred = true;
        return true;
    }
    return false;
}
