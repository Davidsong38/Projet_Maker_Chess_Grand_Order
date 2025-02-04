//
// Created by sebas on 29/12/2024.
//

#include "PhysicsEngine.h"

#include <iostream>

PhysicsEngine::PhysicsEngine() {
    std::cout << "Creating Physics Engine" << std::endl;
}

PhysicsEngine::~PhysicsEngine() {
    std::cout << "Destroying Physics Engine" << std::endl;
}

PhysicsEngine* PhysicsEngine::getInstance() {
    if (instance == nullptr) {
        instance = new PhysicsEngine();
    }
    return instance;
}

void PhysicsEngine::update(double deltaTime_ms) {

}