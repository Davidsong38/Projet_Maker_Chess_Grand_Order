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

vector<glm::ivec2> Merlin::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (chooseSpell)
        if (effect == IMMUNITY_EFFECT || effect == IMMUNITY_AOE)
            return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    if (effect == IMMUNITY_EFFECT || effect == IMMUNITY_AOE)
        return cross_1_pattern->get_positions(glm::ivec2(coordX, coordY));
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
    static EffectInstance * effect_instance_1 = nullptr;
    if (effect_instance_1 == nullptr) {
        effect_instance_1 = new EffectInstance(
            IMMUNITY_EFFECT,
            this,
            -1,
            1,
            1
        );
        effect_instance_1->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr)
                return false;
            return !piece->isKing();
        };
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 8 : 0;
    selection_request.blacks = isWhite ? 0 : 8;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance_1, selection_request))
        return false;
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