//
// Created by Megaport on 19/02/2025.
//

#include "Merlin.h"

#include <EffectHandler.h>
#include <GameEngine.h>

board_pattern *Merlin::getEffectRange(const Effect_List effect) {
    if (chooseSpell)
        if (effect == IMMUNITY_EFFECT || effect == IMMUNITY_AOE)
            return square_pattern;
    if (effect == IMMUNITY_EFFECT || effect == IMMUNITY_AOE)
        return cross_1_pattern;
    return getDefaultEffectsRanges();
}

bool Merlin::SpellActivationCheck() {
    if (canEvolve()) {
        MerlinPowerON = true;
        evolved = true;
    }
    chooseSpell = false;
    if (!passiveUsed)
        passive();
    if (evolved && MerlinPowerON) {
        chooseSpell = true;
        if (!evolvedForm())
            return true;
    }
    if (evolved)
        MerlinPowerON = !MerlinPowerON;
    passiveUsed = false;
    return true;
}

bool Merlin::passive() {
    auto* effect_instance_1 = new EffectInstance(
        IMMUNITY_EFFECT,
        this,
        -1,
        1,
        1
    );
    auto* effect_instance_2 = new EffectInstance(
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
    passiveUsed = true;
    return true;
}

bool Merlin::canEvolve() {
    if (!evolved && !getAllEffectUpdateCastedByMeEvent().empty())
        return true;
    return false;
}

bool Merlin::evolvedForm() {
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
    selection_request.whites = isWhite ? 1 : 0;
    selection_request.blacks = isWhite ? 0 : 1;
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
    if (EffectHandler::applyToTargets(effect_instance_1) && EffectHandler::applyToTargets(effect_instance_2)) {
        effect_instance_1 = nullptr;
        return true;
    }
    return false;
}