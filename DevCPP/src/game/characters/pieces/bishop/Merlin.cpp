//
// Created by Megaport on 19/02/2025.
//

#include "Merlin.h"

#include <EffectHandler.h>
#include <GameEngine.h>

void Merlin::setPieceGameMode(int piece_game_mode) {
    if (evolved && MerlinPowerON){
        pieceGameMode = piece_game_mode;
    } else{
        pieceGameMode = 0;
    }
}

vector<glm::ivec2> Merlin::getEffectRange(Effect_List effect) const {

    vector<glm::ivec2> effect_range;

    if (effect == IMMUNITY_EFFECT && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (effect == IMMUNITY_AOE && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (chooseSpell){
        if (effect == IMMUNITY_EFFECT) {
            for (int i = 0; i < 8; ++i){
                for (int j = 0; j < 8; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
        if (effect == IMMUNITY_AOE) {
            for (int i = 0; i < 8; ++i){
                for (int j = 0; j < 8; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
    }
    return effect_range;
}

bool Merlin::SpellActivationCheck(void *arg) {
    if (evolved && MerlinPowerON){
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
                chooseSpell = true;
                if (evolvedForm(arg)){
                    chooseSpell = false;
                    MerlinPowerON = false;
                    return true;
                }
                chooseSpell = false;
            }
            return false;
        }
    }
    if (evolved && !MerlinPowerON && !isOnAMove)
        MerlinPowerON = true;
    passive(arg);
    setIsOnAMove(false);
    return true;
}

bool Merlin::passive(void* arg) {
    auto *  effect_instance_1 = new EffectInstance(
        IMMUNITY_EFFECT,
        this,
        -1,
        1,
        1
    );
    auto *  effect_instance_2 = new EffectInstance(
        IMMUNITY_AOE,
        this,
        -1,
        1,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance_1);
    effect_instance_2->copyTargets(effect_instance_1);
    EffectHandler::applyToTargets(effect_instance_1);
    EffectHandler::applyToTargets(effect_instance_2);
    return true;
}

bool Merlin::canEvolve(void *arg) {
    if (evolved == false ) {
        return true;
    }
    return false;

}

bool Merlin::evolvedForm(void *arg) {
    auto *  effect_instance_1 = new EffectInstance(
        IMMUNITY_EFFECT,
        this,
        -1,
        1,
        1
    );
    auto *  effect_instance_2 = new EffectInstance(
        IMMUNITY_AOE,
        this,
        -1,
        1,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance_1);
    effect_instance_2->copyTargets(effect_instance_1);
    if (EffectHandler::applyToTargets(effect_instance_1) && EffectHandler::applyToTargets(effect_instance_2))
        return true;
    ///TODO ONE MORE MOVE
    return false;
}