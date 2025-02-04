//
// Created by sebas on 29/12/2024.
//

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Engine.h"



class PhysicsEngine final : Engine {
public:
    PhysicsEngine();
    ~PhysicsEngine() override;
    static PhysicsEngine* getInstance();
    void update(double deltaTime_ms) override;
private:
    inline static PhysicsEngine* instance = nullptr;
};



#endif //PHYSICSENGINE_H
