//
// Created by david on 03/03/2025.
//

#include "Tamamo.h"

#include <EffectHandler.h>

board_pattern *Tamamo_No_Mae::getEffectRange(const Effect_List effect) {
    if (effect == ALTERNATE_RANGE)
        return cross_1_pattern;
    if (effect == GIVING_AOE)
        return square_pattern;
    return getDefaultEffectsRanges();
}

bool Tamamo_No_Mae::SpellActivationCheck() {
    if (!evolved)
        passive();
    return true;
}

bool Tamamo_No_Mae::passive() {
    auto *  effect_instance = new EffectInstance(
        ALTERNATE_RANGE,
        this,
        -1,
        -1,
        1
    );
    effect_instance->check_condition = [](const void* cell) {
        const auto* piece = static_cast<const chessboard_cell*>(cell)->piece;
        if (piece == nullptr)
            return false;
        return piece->isPawn();
    };
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    return true;
}

bool Tamamo_No_Mae::canEvolve() {
    if (!evolved && getAllDeathWithEffectCastedByMe().size()>1) {
        int CNT = 0;
        for (auto deadptr : getAllDeathWithEffectCastedByMe()) {
            if (auto *dead = static_cast<Pieces*>(deadptr); dead->isPawn() && dead->getIsWhite() == this->getIsWhite())
                CNT++;
            if (CNT > 1) {
                evolved = true;
                evolvedForm();
                return true;
            }
        }
    }
    return false;
}

bool Tamamo_No_Mae::evolvedForm() {
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
            if (piece->isKnight() || piece->isBishop())
                return true;
            return false;
        };
    }
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    effect_instance->target_pieces.clear();
    EffectHandler::applyBuffToSelf(effect_instance);
    return true;
}