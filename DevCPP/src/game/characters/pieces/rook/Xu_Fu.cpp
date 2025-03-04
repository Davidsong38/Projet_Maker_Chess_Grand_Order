//
// Created by Megaport on 20/02/2025.
//

#include "Xu_Fu.h"

#include <EffectHandler.h>
#include <GameEngine.h>


board_pattern *Xu_Fu::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == IMMORTALITY)
        return cross_1_pattern;
    if (effect == SHIELD)
        return cross_1_pattern;
    if (effect == SUPP_RANGE)
        return cross_1_pattern;
    return getDefaultEffectsRanges();
}

bool Xu_Fu::SpellActivationCheck() {
    if (this->getPieceGameMode() == 0)
        return true;
    pieceGameMode = 0;
    passive();
    if (canEvolve())
        evolved = true;
    if (!evolved)
        return true;
    evolvedForm();
    return true;
}

bool Xu_Fu::passive() {
    if (const int chance = rand() % 100; chance >= shieldChance)
        return true;
    auto *  effect_instance = new EffectInstance(
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
        return piece->isPawn();
    };
    EffectHandler::selectRandomTargetPieces(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance))
        CNT_Shield++;
    return true;
}

bool Xu_Fu::evolvedForm() {
    // Selection of target
    static EffectInstance * effect_instance_1 = nullptr;
    if (effect_instance_1 == nullptr) {
        effect_instance_1 = new EffectInstance(
            IMMORTALITY,
            this,
            -1,
            -1,
            1
        );
        effect_instance_1->check_condition = [this](const void* cell) {
            const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
            if (piece == nullptr)
                return false;
            return piece->isPawn() || piece == this;
        };
    }
    selection_request_type selection_request;
    selection_request.whites = isWhite ? 1 : 0;
    selection_request.blacks = isWhite ? 0 : 1;;
    selection_request.instantValidation = false;
    if (!EffectHandler::selectManualTargetCells(effect_instance_1, selection_request)) {
        pieceGameMode = 1;
        return false;
    }
    // Case where we selected this
    if (effect_instance_1->target_pieces[0] == this) {
        auto * effect_instance = new EffectInstance(
            SHIELD,
            this,
            -1,
            2,
            1
        );
        EffectHandler::applyBuffToSelf(effect_instance);
        effect_instance_1 = nullptr;
        return true;
    }
    // Case where we selected a pawn
    auto * effect_instance_2 = new EffectInstance(
        SUPP_RANGE,
        this,
        -1,
        1,
        1
    );
    effect_instance_2->copyTargets(effect_instance_1);
    if (EffectHandler::applyToTargets(effect_instance_1)) {
        EffectHandler::applyToTargets(effect_instance_2);
        effect_instance_1 = nullptr;
        return true;
    }
    return false;
}

bool Xu_Fu::togglePieceGameMode() {
    pieceGameMode = !pieceGameMode;
    return pieceGameMode != 0;
}