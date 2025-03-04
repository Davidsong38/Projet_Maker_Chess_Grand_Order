//
// Created by Megaport on 22/02/2025.
//

#include "Gilgamesh.h"

#include <EffectHandler.h>

#include "GameEngine.h"


board_pattern *Gilgamesh::getEffectRange(const Effect_List effect) {
    if (effect == MOVE_CHANGING)
        return square_pattern;
    return getDefaultEffectsRanges();
}

bool Gilgamesh::SpellActivationCheck() {
    if (this->getPieceGameMode() != 0 && !evolved) {
        passive();
        pieceGameMode = 0;
        return true;
    }
    if (canEvolve()){
        if (evolvedFormMove == 0)
            default_piece_move = king_default_moves;
        if (evolvedFormMove == 1)
            default_piece_move = rook_default_moves;
        if (evolvedFormMove == 2)
            default_piece_move = bishop_default_moves;
        if (evolvedFormMove == 3)
            default_piece_move = knight_default_moves;
        if (evolvedFormMove == 4)
            default_piece_move = queen_default_moves;
        CNT_EvolvedForm = 5;
        evolvedForm();


    }
    if (!evolved) {
        return true;
    }
    CNT_EvolvedForm--;
    if (CNT_EvolvedForm == 0){
        default_piece_move = king_default_moves;
        evolved = false;
    }
    return true;
}

bool Gilgamesh::passive() {
    evolvedFormMove = rand() % 4 + 1;
    return true;
}

bool Gilgamesh::canEvolve() {
    if (!evolved && GameEngine::getInstance()->getPhaseNumber()-getSecondLastNormalMovePhase() > 8) {
        return true;
    }
    return false;
}

bool Gilgamesh::evolvedForm() {
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            MOVE_CHANGING,
            this,
            -1,
            3,
            1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr || !EffectHandler::cellIsInRange(static_cast<const chessboard_cell*>(cell),effect_instance))
                return false;
            return true;
        };
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 1 : 0;
    selection_request.blacks = isWhite ? 0 : 1;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance)) {
        evolved = true;
        effect_instance = nullptr;
        return true;
    }
    return false;
}

bool Gilgamesh::togglePieceGameMode() {
    if (!evolved) {
        pieceGameMode = !pieceGameMode;
        return pieceGameMode != 0;
    }
    return false;
}