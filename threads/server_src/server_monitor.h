#ifndef MONITOR_H
#define MONITOR_H

#include <functional>
#include <list>
#include <mutex>
#include <string>
#include <vector>

#include "../common_src/common_message.h"
#include "../common_src/queue.h"

class Monitor {
private:
    std::mutex mtx;
    // cppcheck-suppress unusedStructMember
    std::list<std::reference_wrapper<Queue<MessageFormat>>> queues;

public:
    Monitor();

    void addClientQueue(Queue<MessageFormat>& client);

    void broadcast(const MessageFormat& msj);

    void ereaseClientQueue(Queue<MessageFormat>& client);

    ~Monitor();
};

#endif
