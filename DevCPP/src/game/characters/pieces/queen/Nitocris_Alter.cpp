//
// Created by Megaport on 21/02/2025.
//

#include "Nitocris_Alter.h"

#include <EffectHandler.h>
#include <GameEngine.h>

board_pattern *Nitocris_Alter::getEffectRange(const Effect_List effect) {
    if (effect == SPAWN_PIECES)
        return nitocris_alter_spawn_pattern;
    if (effect == KILLING)
        return square_pattern;
    return getDefaultEffectsRanges();
}

bool Nitocris_Alter::SpellActivationCheck() {
    if (evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()) {
        evolvedForm();
    }
    return true;
}


bool Nitocris_Alter::passive() {
    if (isWhite)
        CNT_4Turn += GameEngine::getInstance()->getWhiteKillAmountLastPhase();
    else
        CNT_4Turn += GameEngine::getInstance()->getBlackKillAmountLastPhase();
    if (CNT_4Turn >= 4){
        Revive_Charge++;
        CNT_4Turn -= 4 ;
    }
    if (Revive_Charge == 0)
        return true;
    auto *  effect_instance = new EffectInstance(
        SPAWN_PIECES,
        this,
        1,
        1,
        1
    );
    EffectHandler::selectRandomTargetDeadPieces(effect_instance);
    EffectHandler::selectRandomTargetEmptyCells(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance)) {
        CNT_Revive++;
        Revive_Charge--;
    }
    return true;
}

bool Nitocris_Alter::canEvolve() {
    if (evolved == false && CNT_Revive>1) {
        evolved = true;
        return true;
    }
    return false;

}

bool Nitocris_Alter::evolvedForm() {
    auto *  effect_instance_1 = new EffectInstance(
        SPAWN_PIECES,
        this,
        1,
        1,
        1
    );
    auto *  effect_instance_2 = new EffectInstance(
        KILLING,
        this,
        1,
        1,
        1
    );
    effect_instance_2->check_condition = [](const void* cell) {
        const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
        if (piece == nullptr)
            return false;
        return piece->isPawn();
    };
    EffectHandler::selectRandomTargetDeadPieces(effect_instance_1);
    EffectHandler::selectRandomTargetEmptyCells(effect_instance_1);
    EffectHandler::applyToTargets(effect_instance_1);

    EffectHandler::selectRandomTargetPieces(effect_instance_2);
    EffectHandler::applyToTargets(effect_instance_2);
    return true;
}