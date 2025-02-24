//
// Created by sebas on 24/02/2025.
//

#include "PhaseHandler.h"

#include <GameEngine.h>

int PhaseHandler::getTurnNumber() {
    return GameEngine::getInstance()->getTurnNumber();
}
