#include "server_receiver.h"

#define RUN
#define ATTACK "attack"

Receiver::Receiver(Monitor& monitor, Socket skt, Queue<std::string>& game_channel):
        monitor(monitor), servprot(std::move(skt)), died(false), game_channel(game_channel) {}

void Receiver::run() {
    Sender sender(this->servprot, this->monitor);
    sender.start();
    this->_wrapperLoopReceiver();
    sender.kill();
    sender.join();
}

void Receiver::_wrapperLoopReceiver() {
    try {
        this->loopReceiver();
    } catch (const std::runtime_error& err) {
        return;
    }
}

void Receiver::loopReceiver() {
    while (this->is_alive()) {
        std::string attack = this->servprot.receiveClientAttack();
        if (attack.compare(ATTACK) != 0) {
            this->_is_alive = false;
            break;
        }
        this->game_channel.push(attack);
    }
}


void Receiver::kill() {
    this->_is_alive = false;
    this->servprot.endComunication();
}
