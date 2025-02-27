//
// Created by Megaport on 21/02/2025.
//

#include "Nitocris_Alter.h"

#include <EffectHandler.h>
#include <GameEngine.h>

vector<glm::ivec2> Nitocris_Alter::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == SPAWN_PIECES)
        return merge_patterns(
            cross_2_pattern->get_positions(glm::ivec2(coordX, coordY)),
            x_cross_1_pattern->get_positions(glm::ivec2(coordX, coordY))
        );
    if (effect == KILLING)
        return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    return effect_range;
}

bool Nitocris_Alter::SpellActivationCheck(void *arg) {
    if (evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()) {
        evolvedForm(arg);
    }
    return true;
}


bool Nitocris_Alter::passive(void* arg) {
    if (isWhite){
        CNT_4Turn += GameEngine::getInstance()->NB_WhiteDead - GameEngine::getInstance()->NB_WhiteDeadLastPhase;
    } else {
        CNT_4Turn += GameEngine::getInstance()->NB_BlackDead - GameEngine::getInstance()->NB_BlackDeadLastPhase;
    }
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

bool Nitocris_Alter::canEvolve(void *arg) {
    if (evolved == false && CNT_Revive>1) {
        evolved = true;
        return true;
    }
    return false;

}

bool Nitocris_Alter::evolvedForm(void *arg) {
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