//
// Created by david on 27/03/2025.
//

#include "Oberon.h"

#include <EffectHandler.h>

board_pattern *Oberon::getEffectRange(const Effect_List effect) {
    if (effect == KILLING)
        return square_pattern;
    if (effect == ALLY_TELEPORT)
        return square_pattern;
    if (effect == GIVING_AOE)
        return nitocris_alter_spawn_pattern;
    return getDefaultEffectsRanges();
}

board_pattern *Oberon::getEffectRangeGiven(const Effect_List effect) {
    if (effect == AOE)
        return cross_1_pattern;
    return getDefaultEffectsRanges();
}

EffectInstance *Oberon::getEffectInstanceGiven(Effect_List effect) {
    return new EffectInstance(
            AOE,
            this,
            -1,
            1,
            -1
            );

}

bool Oberon::SpellActivationCheck() {
    if (this->getPieceGameMode() == 0)
        return true;
    pieceGameMode = 0;
    passive();
    if (canEvolve())
        evolvedForm();
    return true;
}

bool Oberon::passive() {
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            GIVING_AOE,
            this,
            -1,
            1,
            -1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            return piece == nullptr;
        };
    }
    selection_request_type selection_request;
    selection_request.emptys = 1;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance, selection_request)){
        pieceGameMode = 1;
        return false;
    }
    if (EffectHandler::applyToTargets(effect_instance)){
        effect_instance = nullptr;
        CNT_Success++;
        return true;
    }

    return true;
}

bool Oberon::canEvolve() {
    if (!evolved && CNT_Success>2)
        return true;
    return false;
}

bool Oberon::evolvedForm() {
    static EffectInstance * effect_instance = nullptr;
    if (effect_instance == nullptr) {
        effect_instance = new EffectInstance(
            GIVING_AOE,
            this,
            -1,
            1,
            -1
        );
        effect_instance->check_condition = [](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr)
                return false;
            return true;
        };
    }
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    effect_instance->target_pieces.clear();
    EffectHandler::applyBuffToSelf(effect_instance);
    return true;
}