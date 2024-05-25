#ifndef ENEMY_H
#define ENEMY_H

#include <atomic>
#include <cstddef>

class Enemy {
private:
    std::atomic<std::size_t> status;

public:
    Enemy();
    bool isAlive();
    bool updateRound();
    void kill();
    ~Enemy();

    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy) = delete;
    Enemy(Enemy&&) = delete;
    Enemy& operator=(Enemy&&) = delete;
};
#endif
