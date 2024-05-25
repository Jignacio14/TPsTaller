#include "server_monitor.h"

#include <mutex>


Monitor::Monitor() {}

void Monitor::addClientQueue(Queue<MessageFormat>& client) {
    std::lock_guard<std::mutex> lck(mtx);
    queues.push_back(client);
}

void Monitor::broadcast(const MessageFormat& msj) {
    std::lock_guard<std::mutex> lck(mtx);
    for (auto iter = this->queues.begin(); iter != this->queues.end(); iter++) {
        Queue<MessageFormat>& queue = iter->get();
        try {
            queue.push(msj);
        } catch (const ClosedQueue&) {
            iter = queues.erase(iter);
        }
    }
}

void Monitor::ereaseClientQueue(Queue<MessageFormat>& client) {
    std::lock_guard<std::mutex> lck(mtx);
    queues.remove_if([&client](const std::reference_wrapper<Queue<MessageFormat>>& queue_ref) {
        return &queue_ref.get() == &client;
    });
}

Monitor::~Monitor() {}
