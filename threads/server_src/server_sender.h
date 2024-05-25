#ifndef SENDER_H
#define SENDER_H

#include <stdexcept>

#include "../common_src/common_message.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"

#include "server_monitor.h"
#include "server_protocol.h"

class Sender: public Thread {
private:
    ServerProtocol& servprot;
    Queue<MessageFormat> messages;
    Monitor& monitor;

    void sendGameStatus(const MessageFormat& mjs);
    void loopSender();

public:
    explicit Sender(ServerProtocol& servprot, Monitor& monitor);
    void run() override;
    void kill();
    ~Sender();
};

#endif
