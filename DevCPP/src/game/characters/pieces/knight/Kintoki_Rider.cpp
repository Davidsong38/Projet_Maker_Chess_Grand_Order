//
// Created by david on 02/03/2025.
//

#include "Kintoki_Rider.h"

#include <EffectHandler.h>
#include <phase_context.h>
#include <GameEngine.h>


board_pattern *Kintoki_Rider::getEffectRange(const Effect_List effect) {
    if (effect == SUPP_MOVE)
        return cross_1_pattern;
    return getDefaultEffectsRanges();
}

bool Kintoki_Rider::SpellActivationCheck() {
    passive();
    if (canEvolve()) {
        evolved = true;
        evolvedForm();
    }
    return true;
}

bool Kintoki_Rider::passive() {
    if (const int chance = rand() % 100; chance >= suppMoveChance && GameEngine::getInstance()->getLastState() != SELECT_ANY)
        return true;
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            SUPP_MOVE,
            this,
            1,
            1,
            1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr && EffectHandler::cellIsInRange(static_cast<const chessboard_cell*>(cell),effect_instance))
                return true;
            return false;
        };
    }
    selection_request_type selection_request;
    selection_request.emptys = 1;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        return false;
    }
    if (EffectHandler::applyBuffToSelf(effect_instance)) {
        effect_instance = nullptr;
        CNT_SuppMove++;
        return true;
    }
    return false;
}

bool Kintoki_Rider::canEvolve() {
    if (!evolved && CNT_SuppMove > 1)
        return true;
    return false;
}

bool Kintoki_Rider::evolvedForm() {
    default_piece_move = kintoki_rider_buff_moves;
    return true;
}