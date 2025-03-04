//
// Created by david on 30/01/2025.
//

#include "Arceuid.h"

#include "EffectHandler.h"
#include <GameEngine.h>

board_pattern *Arceuid::getEffectRange(Effect_List effect) {
    if (effect == AOE) {
        if (CNTGainEffect >= 24)
            return nitocris_alter_spawn_pattern;
        if (CNTGainEffect >= 16)
            return cross_1_pattern;
    }
    return getDefaultEffectsRanges();
}

bool Arceuid::SpellActivationCheck() {
    if (canEvolve()){
        evolved = true;
        CNTGainEffect = GameEngine::getInstance()->getPhaseNumber() - getLastNormalMovePhase();
        if (CNTGainEffect >= 8)
            this->default_piece_move = arceuid_buff_move;
    }
    evolvedForm();
    return true;
}

bool Arceuid::passive() {
    return true;
}

bool Arceuid::canEvolve() {
    if (!evolved && getLastNormalMoveEventType() == MOVE_ROQUED) {
        evolved = true;
        CNTGainEffect = GameEngine::getInstance()->getPhaseNumber();
        return true;
    }
    return false;
}

bool Arceuid::evolvedForm() {
    ltr_log_info("Evolved  Arceuid : ", CNTGainEffect);
    if (evolved && (CNTGainEffect >= 30 || getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && CNTGainEffect >= 8)) {
        auto *  effect_instance = new EffectInstance(
            AOE,
            this,
            1,
            1,
            -1
        );
        EffectHandler::selectRandomTargetPieces(effect_instance);
        EffectHandler::applyToTargets(effect_instance);
    }
    return true;
}
