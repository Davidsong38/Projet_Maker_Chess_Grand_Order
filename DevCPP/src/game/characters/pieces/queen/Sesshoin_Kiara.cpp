//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"

#include <GameEngine.h>
#include "EffectHandler.h"

board_pattern *Sesshoin_Kiara::getEffectRange(const Effect_List effect) {
    if (effect == CHANGE_CONTROL_ADVANCE)
        return square_pattern;
    if (effect == CHANGE_CONTROL)
        return cross_2_pattern;
    return getDefaultEffectsRanges();
}

bool Sesshoin_Kiara::SpellActivationCheck() {
    if (this->getPieceGameMode() == 0)
        return true;
    pieceGameMode = 0;
    if (canEvolve())
        evolved = true;
    if (CNT_Charm != 3){
        passive();
        return true;
    }
    if (!evolved)
        return true;
    evolvedForm();
    return true;
}

bool Sesshoin_Kiara::passive() {
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            CHANGE_CONTROL,
            this,
            5,
            1,
            1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr || !EffectHandler::cellIsInRange(static_cast<const chessboard_cell*>(cell),effect_instance))
                return false;
            return !piece->isKing();
        };
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 0 : 1;
    selection_request.blacks = isWhite ? 1 : 0;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        pieceGameMode = 1;
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance)){
        effect_instance = nullptr;
        CNT_Charm++;
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::canEvolve() {
    if (evolved == false && CNT_Charm > 2) {
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::evolvedForm() {
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            CHANGE_CONTROL_ADVANCE,
            this,
            -1,
            1,
            1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr || !EffectHandler::cellIsInRange(static_cast<const chessboard_cell*>(cell),effect_instance))
                return false;
            return !piece->isKing();
        };
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 0 : 1;
    selection_request.blacks = isWhite ? 1 : 0;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        pieceGameMode = 1;
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance)) {
        effect_instance = nullptr;
        CNT_Charm++;
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::togglePieceGameMode() {
    pieceGameMode = !pieceGameMode;
    return pieceGameMode != 0;
}