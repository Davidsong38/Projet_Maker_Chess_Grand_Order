//
// Created by Megaport on 20/02/2025.
//

#include "Xu_Fu.h"

#include <EffectHandler.h>
#include <GameEngine.h>


void Xu_Fu::setPieceGameMode(int piece_game_mode) {
    pieceGameMode = piece_game_mode;
}

vector<glm::ivec2> Xu_Fu::getEffectRange(Effect_List effect) const {
    vector<glm::ivec2> effect_range;
    if (effect == IMMORTALITY){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }
    if (effect == SHIELD) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }
    if (effect == SUPP_RANGE){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }
    return effect_range;
}

bool Xu_Fu::SpellActivationCheck(void *arg) {
    if (this->getPieceGameMode() != 0){
        if (!getIsOnAMove())
            passive(arg);
        if (canEvolve(arg)){
            setIsOnAMove(true);
            evolved = true;
        }
        if (GameEngine::getInstance()->receivedClick && evolved){
            GameEngine::getInstance()->ghostClick = true;
            if (evolvedForm(arg)){
                setIsOnAMove(false);
                return true;
            }
            return false;
        }
        if (evolved && getIsOnAMove()){
            return false;
        }
    }
    return true;
}

bool Xu_Fu::passive(void* arg) {
    if (int chance = rand() % 100; chance < ShieldChance){
        auto *  effect_instance = new EffectInstance(
            SHIELD,
            this,
            -1,
            1,
            1
        );
        EffectHandler::selectRandomTargetPieces(effect_instance);
        if (EffectHandler::applyToTargets(effect_instance))
            CNT_Shield++;
    }
    return true;
}

bool Xu_Fu::canEvolve(void *arg) {
    if (evolved == false && CNT_Shield == 2) {
        return true;
    }
    return false;
}

bool Xu_Fu::evolvedForm(void *arg) {
    if (GameEngine::getInstance()->getLastClickX() == coordX && GameEngine::getInstance()->getLastClickY() == coordY){
        auto *  effect_instance = new EffectInstance(
            SHIELD,
            this,
            -1,
            2,
            1
        );
        EffectHandler::applyBuffToSelf(effect_instance);
        return true;
    }
    static EffectInstance * effect_instance_1 = nullptr;
    static EffectInstance * effect_instance_2 = nullptr;
    if (effect_instance_1 == nullptr) {
        effect_instance_1 = new EffectInstance(
            IMMORTALITY,
            this,
            -1,
            -1,
            1
        );
        effect_instance_1->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr) {
                return false;
            }
            return piece->isPawn();
        };
        effect_instance_2 = new EffectInstance(
            SUPP_RANGE,
            this,
            -1,
            1,
            1
        );
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 8 : 0;
    selection_request.blacks = isWhite ? 0 : 8;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance_1, selection_request))
        return false;
    effect_instance_2->copyTargets(effect_instance_1);
    if (EffectHandler::applyToTargets(effect_instance_1)) {
        EffectHandler::applyToTargets(effect_instance_2);
        effect_instance_1 = nullptr;
        effect_instance_2 = nullptr;
        return true;
    }
    return false;
}