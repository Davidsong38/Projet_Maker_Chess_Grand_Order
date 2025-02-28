#include <chrono>
#include <keys.h>

#include "GameEngine.h"
#include "PhysicsEngine.h"
#include "RenderEngine.h"
#include "log.h"

#define MIN_DELAY 16.7

std::chrono::time_point<std::chrono::high_resolution_clock> t_start = std::chrono::high_resolution_clock::now();

void init() {
    PhysicsEngine::getInstance();
    RenderEngine::getInstance();
    GameEngine::getInstance();
}

void update() {
    static std::chrono::time_point<std::chrono::high_resolution_clock> t_last = t_start;
    const std::chrono::time_point<std::chrono::high_resolution_clock> t_curr = std::chrono::high_resolution_clock::now();
    const double delay_ms = std::chrono::duration<double, std::milli>(t_curr-t_last).count();
    t_last = t_curr;

    GameEngine::getInstance()->update(delay_ms);
    PhysicsEngine::getInstance()->update(delay_ms);
    RenderEngine::getInstance()->update(delay_ms);
}

void waitForNextUpdate() {
    static std::chrono::time_point<std::chrono::high_resolution_clock> t_last = t_start;
    while (
        std::chrono::duration<double, std::milli>
        (std::chrono::high_resolution_clock::now()-t_last)
        .count() < MIN_DELAY
    );
    t_last = std::chrono::high_resolution_clock::now();
}

int main(int argc, char** argv) {
    init();
    while (GameEngine::getInstance()->getCurrentState() != GAME_CLOSE && get_key(KEY_ESCAPE)->isKeyUp()) {
        update();
        waitForNextUpdate();
    }
    return EXIT_SUCCESS;
}

