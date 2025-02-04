//
// Created by sebas on 29/12/2024.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Engine.h"



class GameEngine final : public Engine {
public:
    GameEngine();
    ~GameEngine() override;
    static GameEngine* getInstance();
    void update(double deltaTime_ms) override;
private:
    inline static GameEngine* instance = nullptr;
};



#endif //GAMEENGINE_H
