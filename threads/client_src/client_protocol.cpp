#include "client_protocol.h"

#define DEATH_CODE 0x04
#define REVIVE_CODE 0x05
#define HEADER 0x06
#define ATTACK 0x03
#define ErrorMessage "Communication error: socket closed unexpectedly."

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& servername):
        skt(hostname.c_str(), servername.c_str()), was_close(false) {}

bool ClientProtocol::sendAttack() {
    try {
        return this->_sendAttack();
    } catch (const LibError& err) {
        was_close = true;
        return false;
    }
}

bool ClientProtocol::wasClosed() { return was_close; }

MessageFormat ClientProtocol::listenSocketData() {
    try {
        return this->_listenSocketData();
    } catch (const LibError& err) {
        was_close = true;
        return MessageFormat();
    }
}

// private methods

bool ClientProtocol::_sendAttack() {
    if (was_close) {
        return false;
    }
    u_int8_t attack = ATTACK;
    int result = skt.sendall(&attack, sizeof(attack), &this->was_close);
    if (sizeof(attack) != result) {
        throwCommunicationError();
    }
    return true;
}

MessageFormat ClientProtocol::_listenSocketData() {
    if (this->checkSocketCloseSatus()) {
        return MessageFormat();
    }
    std::pair<u_int16_t, uint16_t> counts = this->receive2BytesArray();
    if (was_close) {
        throwCommunicationError();
    }
    u_int8_t eventCode = this->receiveByteCode();
    if (was_close) {
        throwCommunicationError();
    }
    return MessageFormat(counts.first, counts.second, (eventCode == REVIVE_CODE ? true : false));
}

bool ClientProtocol::checkSocketCloseSatus() {
    u_int8_t header = receiveByteCode();
    if (was_close || header != HEADER) {
        return true;
    }
    return false;
}


u_int8_t ClientProtocol::receiveByteCode() {
    u_int8_t code = 0;
    skt.recvall(&code, sizeof(code), &this->was_close);
    return code;
}

std::pair<uint16_t, uint16_t> ClientProtocol::receive2BytesArray() {
    uint16_t counts[2] = {0};
    if (this->was_close) {
        return {0, 0};
    }
    skt.recvall(&counts, sizeof(counts), &was_close);
    uint16_t alive_count = ntohs(counts[0]);
    uint16_t death_count = ntohs(counts[1]);
    return {alive_count, death_count};
}

void ClientProtocol::throwCommunicationError() { throw std::runtime_error(ErrorMessage); }

ClientProtocol::~ClientProtocol() {}
