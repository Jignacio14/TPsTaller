#include "server_acceptator.h"

#define HOW 2

Acceptator::Acceptator(const std::string& port, Monitor& monitor_ref,
                       Queue<std::string>& game_channel):
        listener(port.c_str()), monitor_ref(monitor_ref), game_channel(game_channel) {}

void Acceptator::run() {
    while (this->is_alive()) {
        try {
            this->startClientHandler();
            this->ereaseClient();
        } catch (std::exception&) {}
    }
}

void Acceptator::startClientHandler() {
    Socket peer = listener.accept();
    Receiver* receiver = new Receiver(this->monitor_ref, std::move(peer), this->game_channel);
    this->server_clients.push_back(receiver);
    receiver->start();
}

void Acceptator::ereaseClient() {
    for (auto thread: server_clients) {
        if (!thread->is_alive()) {
            thread->join();
            delete thread;
        }
    }
}

void Acceptator::kill() {
    this->_is_alive = false;
    listener.shutdown(HOW);
    listener.close();
    killAll();
}

void Acceptator::killAll() {
    for (auto thread: server_clients) {
        if (thread->is_alive()) {
            thread->kill();
        }
        thread->join();
        delete thread;
    }
}
