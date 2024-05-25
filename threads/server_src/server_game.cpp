#include "server_game.h"

#define START_ENEMIES_NUMBER 5

Game::Game(Monitor& monitor, Queue<std::string>& channel):
        game_players(START_ENEMIES_NUMBER),
        alive_players(START_ENEMIES_NUMBER),
        server_monitor(monitor),
        channel(channel),
        printer() {}

void Game::run() {
    while (this->_is_alive) {
        std::string attack;
        if (this->channel.try_pop(attack)) {
            this->killEnemy();
        }
        this->iterPlayers();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Game::kill() { this->_is_alive = false; }

void Game::iterPlayers() {
    for (auto it = game_players.begin(); it != game_players.end(); ++it) {
        auto& player = *it;
        bool has_revived = false;
        if (!player.isAlive()) {
            has_revived = player.updateRound();
        }
        this->broadCastRevived(has_revived);
    }
}


void Game::broadCastRevived(const bool& has_revived) {
    if (!has_revived) {
        return;
    }
    this->alive_players++;
    this->broadCastAndPrint(true);
}


void Game::killEnemy() {
    bool was_killed = false;
    for (auto it = game_players.begin(); it != game_players.end(); ++it) {
        auto& player = *it;
        if (player.isAlive()) {
            player.kill();
            was_killed = true;
            break;
        }
    }
    this->broadCastKillEnemy(was_killed);
}

void Game::broadCastKillEnemy(const bool& is_killed) {
    if (!is_killed) {
        return;
    }
    this->alive_players--;
    this->broadCastAndPrint(false);
}

void Game::broadCastAndPrint(const bool& is_revived) {
    MessageFormat msj =
            MessageFormat(alive_players, START_ENEMIES_NUMBER - alive_players, is_revived);
    this->server_monitor.broadcast(msj);
    this->printer.printMessage(msj);
}

Game::~Game() {}
