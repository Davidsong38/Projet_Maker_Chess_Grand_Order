//
// Created by sebas on 29/12/2024.
//

#include "PhysicsEngine.h"

#include <log.h>

PhysicsEngine::PhysicsEngine() {
    ltr_log_info("Creating Physics Engine");
}

PhysicsEngine::~PhysicsEngine() {
    ltr_log_info("Destroying Physics Engine");
}

PhysicsEngine* PhysicsEngine::getInstance() {
    if (instance == nullptr) {
        instance = new PhysicsEngine();
    }
    return instance;
}

void PhysicsEngine::update(double deltaTime_ms) {

}