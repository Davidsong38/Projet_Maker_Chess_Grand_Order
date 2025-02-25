//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"
#include <GameEngine.h>
#include "EffectHandler.h"

void Sesshoin_Kiara::setPieceGameMode(int piece_game_mode) {
    pieceGameMode = piece_game_mode;
}

vector<glm::ivec2> Sesshoin_Kiara::getEffectRange(Effect_List effect) const {
    vector<glm::ivec2> effect_range;
    if (effect == CHANGE_CONTROL_ADVANCE){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }

    if (effect == CHANGE_CONTROL) {
        for (int i = 1; i < 3; ++i) {
            if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
            if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
        }
    }
    return effect_range;
}

bool Sesshoin_Kiara::SpellActivationCheck(void *arg) {
    if (GameEngine::getInstance()->receivedRightClick){
        if (this->getIsWhite()){
            GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
            GameEngine::getInstance()->setState(SELECT_WHITE_PHASE);
        }
        else{
            GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
            GameEngine::getInstance()->setState(SELECT_BLACK_PHASE);
        }
        return true;
    }
    if (this->getPieceGameMode() != 0){
        if (GameEngine::getInstance()->receivedClick){
            if (canEvolve(arg)){
                if (evolvedForm(arg))
                    return true;
                return false;
            }
            if (passive(arg))
                return true;
        }
        return false;
    }
    return true;
}

bool Sesshoin_Kiara::passive(void* arg) {
    auto *  effect_instance = new EffectInstance(
        CHANGE_CONTROL,
        this,
        5,
        1,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance)) {
        CNT_Charm++;
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::canEvolve(void *arg) {
    if (evolved == false && CNT_Charm > 2) {
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::evolvedForm(void *arg) {
    auto *  effect_instance = new EffectInstance(
        CHANGE_CONTROL_ADVANCE,
        this,
        -1,
        1,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance)) {
        evolved = true;
        return true;
    }
    return false;
}