//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"

#include <GameEngine.h>
#include "EffectHandler.h"

vector<glm::ivec2> Sesshoin_Kiara::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == CHANGE_CONTROL_ADVANCE)
        return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    if (effect == CHANGE_CONTROL)
        return cross_2_pattern->get_positions(glm::ivec2(coordX, coordY));
    return effect_range;
}

bool Sesshoin_Kiara::SpellActivationCheck() {
    if (this->getPieceGameMode() == 0)
        return true;
    pieceGameMode = 0;
    std::cout << "hein333" << endl;
    if (canEvolve())
        evolved = true;
    if (!canCharmAdvance && !passive()){
        return false;
    }
    if (!evolved)
        return true;
    if (!evolvedForm())
        return false;
    canCharmAdvance = false;
    return true;
}

bool Sesshoin_Kiara::passive() {
    auto *  effect_instance = new EffectInstance(
        CHANGE_CONTROL,
        this,
        5,
        1,
        1
    );
    effect_instance->check_condition = [](const void* cell) {
        const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
        if (piece == nullptr)
            return false;
        return !piece->isKing();
    };
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 0 : 1;
    selection_request.blacks = isWhite ? 1 : 0;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        std::cout << "hein" << endl;
        pieceGameMode = 1;
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance)){
        std::cout << "hein2" << endl;
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
    auto *  effect_instance = new EffectInstance(
        CHANGE_CONTROL_ADVANCE,
        this,
        -1,
        1,
        1
    );
    effect_instance->check_condition = [](const void* cell) {
        const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
        if (piece == nullptr)
            return false;
        return !piece->isKing();
    };
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 0 : 1;
    selection_request.blacks = isWhite ? 1 : 0;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        pieceGameMode = 1;
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance))
        return true;
    return false;
}

bool Sesshoin_Kiara::togglePieceGameMode() {
    pieceGameMode = !pieceGameMode;
    return pieceGameMode != 0;
}