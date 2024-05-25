#include "server_protocol.h"


#define HEADER 0x06
#define DEATH_CODE 0x04
#define REVIVE_CODE 0x05
#define COUNT_LENGHT 2
#define HOW 2
#define ATTACK_CODE 0x03
#define ATTACK "attack"  // Elijo la a para ataque
#define ErrorMessage "Communication error: socket closed unexpectedly."

ServerProtocol::ServerProtocol(Socket skt): skt(std::move(skt)), was_close(false) {}

bool ServerProtocol::isClosed() { return this->was_close; }

void ServerProtocol::sendGameStatusMsj(const MessageFormat& msj) {
    try {
        this->prepareServerMsj(msj);
    } catch (const LibError& err) {
        was_close = true;
    }
}


std::string ServerProtocol::receiveClientAttack() {
    try {
        return this->_receiveClientAttack();
    } catch (const LibError& err) {
        was_close = true;
        return "";
    }
}

// private methods

std::string ServerProtocol::_receiveClientAttack() {
    char attack_code = 0;
    this->skt.recvall(&attack_code, sizeof(attack_code), &this->was_close);
    if (was_close) {
        return "";
    }
    if (attack_code != ATTACK_CODE) {
        this->throwCommunicationError();
    }
    return ATTACK;
}


void ServerProtocol::prepareServerMsj(const MessageFormat& msj) {
    uint16_t alive_count = msj.alives_count;
    uint16_t death_count = msj.death_count;
    uint8_t event = (msj.event_type ? REVIVE_CODE : DEATH_CODE);
    this->sendGameStatus(alive_count, death_count, event);
}

void ServerProtocol::sendHeader() {
    uint8_t header = HEADER;
    this->skt.sendall(&header, sizeof(header), &this->was_close);
}

std::array<uint16_t, 2> ServerProtocol::prepare2BytesNumbers(uint16_t alive, uint16_t death) {
    uint16_t alive_formated = htons(alive);
    uint16_t death_formated = htons(death);
    return {alive_formated, death_formated};
}

void ServerProtocol::sendGameStatus(uint16_t alive, uint16_t death, uint8_t event) {
    this->sendHeader();
    if (was_close) {
        return;
    }
    std::array<uint16_t, COUNT_LENGHT> bytesFormatted = this->prepare2BytesNumbers(alive, death);
    skt.sendall(&bytesFormatted, sizeof(bytesFormatted), &this->was_close);
    if (was_close) {
        this->throwCommunicationError();
    }
    skt.sendall(&event, sizeof(event), &this->was_close);
    if (was_close) {
        this->throwCommunicationError();
    }
}

void ServerProtocol::endComunication() {
    this->skt.shutdown(HOW);
    this->skt.close();
}

void ServerProtocol::throwCommunicationError() { throw std::runtime_error(ErrorMessage); }

ServerProtocol::~ServerProtocol() {}
