#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <list>
#include <mutex>
#include <string>
#include <thread>

#include <string.h>

#include "../common_src/common_message.h"
#include "../common_src/common_message_constructor.h"
#include "../common_src/thread.h"

#include "server_enemy.h"
#include "server_monitor.h"

class Game: public Thread {
private:
    // cppcheck-suppress unusedStructMember
    std::list<Enemy> game_players;
    std::atomic<std::size_t> alive_players;
    Monitor& server_monitor;
    Queue<std::string>& channel;
    MessageConstructor printer;
    void iterPlayers();
    // Mata a un enemigo (puede no ir aca)
    void killEnemy();

    void broadCastRevived(const bool& has_revived);
    void broadCastKillEnemy(const bool& is_killed);

    void broadCastAndPrint(const bool& is_revived);

public:
    explicit Game(Monitor& monitor, Queue<std::string>& channel);
    // Game();
    // Hilo que lanza la ejecucion del juego
    virtual void run() override;
    void kill();

    Game(const Game&) = delete;
    Game& operator=(const Game) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    ~Game();
};
#endif
