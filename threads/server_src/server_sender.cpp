#include "server_sender.h"


Sender::Sender(ServerProtocol& servprot, Monitor& monitor): servprot(servprot), monitor(monitor) {
    this->monitor.addClientQueue(this->messages);
}

void Sender::run() {
    try {
        this->loopSender();
    } catch (ClosedQueue&) {
        this->_is_alive = false;
    }
}

void Sender::loopSender() {
    while (this->is_alive()) {
        MessageFormat msj = this->messages.pop();
        this->sendGameStatus(msj);
    }
}

void Sender::sendGameStatus(const MessageFormat& msj) {
    try {
        this->servprot.sendGameStatusMsj(msj);
        this->_is_alive = !this->servprot.isClosed();
    } catch (const std::runtime_error& err) {
        return;
    }
}

void Sender::kill() {
    this->_is_alive = false;
    this->messages.close();
    this->monitor.ereaseClientQueue(this->messages);
}

Sender::~Sender() {}
