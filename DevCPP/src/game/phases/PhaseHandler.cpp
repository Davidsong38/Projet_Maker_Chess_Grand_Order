//
// Created by david on 24/02/2025.
//

#include "PhaseHandler.h"

#include <GameEngine.h>

int PhaseHandler::getTurnNumber() {
    return GameEngine::getInstance()->getTurnNumber();
}

int PhaseHandler::getPhaseNumber() {
    return GameEngine::getInstance()->getPhaseNumber();
}
