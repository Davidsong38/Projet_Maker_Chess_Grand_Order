//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"

#include <GameEngine.h>
#include "EffectHandler.h"

void Sesshoin_Kiara::setPieceGameMode(int piece_game_mode) {
    pieceGameMode = piece_game_mode;
}

vector<glm::ivec2> Sesshoin_Kiara::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == CHANGE_CONTROL_ADVANCE)
        return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    if (effect == CHANGE_CONTROL)
        return cross_2_pattern->get_positions(glm::ivec2(coordX, coordY));
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