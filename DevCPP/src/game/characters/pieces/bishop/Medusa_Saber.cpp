//
// Created by david on 24/01/2025.
//

#include "Medusa_Saber.h"

#include "EffectHandler.h"

void Medusa_Saber::setPieceGameMode(int piece_game_mode) {

}

vector<glm::ivec2> Medusa_Saber::getEffectRange(Effect_List effect) const {
    vector<glm::ivec2> effect_range;
    if (effect == STUN && evolved==false) {
        if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
    }
    if (evolved==true) {
        if (effect == STUN) {
            if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
            if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
            if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
            if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);

        }
        if (effect == AOE) {
            if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
            if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
            if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
            if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
        }
    }
    return effect_range;
}

bool Medusa_Saber::SpellActivationCheck(void *arg) {
    if (hasJustKilled) {
        if (canEvolve(arg) || evolved)
            evolvedForm(arg);
        passive(arg);
    }
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
