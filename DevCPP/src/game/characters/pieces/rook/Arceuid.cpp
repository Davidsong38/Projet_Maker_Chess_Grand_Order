//
// Created by david on 30/01/2025.
//

#include "Arceuid.h"
#include <iostream>
#include "EffectHandler.h"

void Arceuid::setPieceGameMode(int piece_game_mode) {

}

vector<glm::ivec2> Arceuid::getEffectRange(Effect_List effect) const {

    vector<glm::ivec2> effect_range;
    if (effect == AOE){
        if (CNTGainEffect >=16){
            if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
            if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
            if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
            if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
        }
        if (CNTGainEffect >=24){
            if (coordX + 2 < 8) effect_range.emplace_back(coordX + 2, coordY);
            if (coordX - 2 >= 0) effect_range.emplace_back(coordX - 2, coordY);
            if (coordY - 2 >= 0) effect_range.emplace_back(coordX, coordY - 2);
            if (coordY + 2 < 8) effect_range.emplace_back(coordX, coordY + 2);
            if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
            if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
            if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
            if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
        }
    }
    return effect_range;
}

bool Arceuid::SpellActivationCheck(void *arg) {
    if (canEvolve(arg)){
        evolved = true;
        CNTGainEffect = NB_TurnWithoutMoving;
        if (CNTGainEffect >= 8)
            this->default_piece_move = arceuid_buff_move;
    }
    evolvedForm(arg);
    return true;
}

bool Arceuid::passive(void* arg) {
    return true;
}

bool Arceuid::canEvolve(void *arg) {
    if (!evolved && hasRoqued) {
        evolved = true;
        CNTGainEffect = NB_TurnWithoutMoving;
        return true;
    }
    return false;
}

bool Arceuid::evolvedForm(void *arg) {
    if (evolved && (CNTGainEffect >= 30 || hasJustKilled && CNTGainEffect >= 8)) {
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
