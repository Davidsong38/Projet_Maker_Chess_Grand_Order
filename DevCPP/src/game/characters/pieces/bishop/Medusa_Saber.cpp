//
// Created by david on 24/01/2025.
//

#include "Medusa_Saber.h"

#include <GameEngine.h>

#include "EffectHandler.h"

board_pattern *Medusa_Saber::getEffectRange(const Effect_List effect) {
    if (evolved) {
        if (effect == STUN)
            return cross_1_pattern;
        if (effect == AOE)
            return x_cross_1_pattern;
    } else {
        if (effect == STUN)
            return x_cross_1_pattern;
    }
    return getDefaultEffectsRanges();
}

bool Medusa_Saber::SpellActivationCheck() {
    if (getLastKillTurn() != GameEngine::getInstance()->getTurnNumber())
        return true;
    if (canEvolve() || evolved)
        evolvedForm();
    passive();
    return true;
}

bool Medusa_Saber::passive() {
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

bool Medusa_Saber::canEvolve() {
    if (evolved == false && CNT_StunEffect>1) {
        return true;
    }
    return false;

}

bool Medusa_Saber::evolvedForm() {
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
