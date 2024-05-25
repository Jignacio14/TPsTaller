#include "server_enemy.h"

#define ALIVE_STATUS 0
#define RECENTLY_KILL_STATUS 15
#define TOTAL_ENEMIES

Enemy::Enemy(): status(ALIVE_STATUS) {}

bool Enemy::isAlive() { return this->status == ALIVE_STATUS; }

void Enemy::kill() { this->status = RECENTLY_KILL_STATUS; }

bool Enemy::updateRound() {
    if (this->isAlive()) {
        return false;
    }
    this->status--;
    return this->isAlive();
}

Enemy::~Enemy() {}
