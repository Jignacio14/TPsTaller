#ifndef RECEIVER_H
#define RECEIVER_H

#include <atomic>
#include <string>

#include "../common_src/common_message.h"
#include "../common_src/thread.h"

#include "server_monitor.h"
#include "server_protocol.h"
#include "server_sender.h"
#include "utility"

class Receiver: public Thread {
private:
    Monitor& monitor;
    ServerProtocol servprot;
    std::atomic_bool died;
    Queue<std::string>& game_channel;
    void _wrapperLoopReceiver();
    void loopReceiver();

public:
    Receiver(Monitor& monitor, Socket skt, Queue<std::string>& game_channel);
    void run() override;
    bool isDead();
    void kill();
};

#endif
