//
// Created by david on 24/01/2025.
//

#include "Medusa_Saber.h"

#include <GameEngine.h>

#include "EffectHandler.h"

vector<glm::ivec2> Medusa_Saber::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (evolved) {
        if (effect == STUN)
            return cross_1_pattern->get_positions(glm::ivec2(coordX, coordY));
        if (effect == AOE)
            return x_cross_1_pattern->get_positions(glm::ivec2(coordX, coordY));
    } else {
        if (effect == STUN)
            return x_cross_1_pattern->get_positions(glm::ivec2(coordX, coordY));
    }
    return effect_range;
}

bool Medusa_Saber::SpellActivationCheck(void *arg) {
    if (getLastKillTurn() != GameEngine::getInstance()->getTurnNumber())
        return true;
    if (canEvolve(arg) || evolved)
        evolvedForm(arg);
    passive(arg);
    return true;
}

bool Medusa_Saber::passive(void* arg) {
    auto * effect_instance = new EffectInstance(
        STUN,
        this,
        2,
        2,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance))
        CNT_StunEffect++;
    return true;
}

bool Medusa_Saber::canEvolve(void *arg) {
    if (evolved == false && CNT_StunEffect>1) {
        return true;
    }
    return false;

}

bool Medusa_Saber::evolvedForm(void *arg) {
    evolved = true;
    auto *  effect_instance = new EffectInstance(
        AOE,
        this,
        1,
        1,
        -1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    return true;
}
