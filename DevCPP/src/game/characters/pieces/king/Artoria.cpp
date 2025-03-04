//
// Created by david on 31/01/2025.
//

#include "Artoria.h"

#include <EffectHandler.h>
#include <GameEngine.h>

board_pattern *Artoria::getEffectRange(const Effect_List effect) {
    const auto lastMoveEvent = static_cast<const EventMove*>(getLastMoveEvent());
    if (lastMoveEvent == nullptr) {
        ltr_log_warn("Artoria::getEffectRange: hasn't moved yet");
        return star_pattern;
    }
    const glm::ivec2 lastPos = lastMoveEvent->getStartPos();
    ltr_log_info("Artoria::getEffectRange: lastMoveEvent->getStartPos() : ", lastPos.x, " ", lastPos.y, " coord : ", coordX, " ", coordY);
    if (effect == AOE) {
        if (lastPos.x < coordX && lastPos.y < coordY)
            return scatter_SE_pattern;
        if (lastPos.x < coordX && lastPos.y == coordY)
            return scatter_S_pattern;
        if (lastPos.x < coordX && lastPos.y > coordY)
            return scatter_SW_pattern;
        if (lastPos.x == coordX && lastPos.y < coordY)
            return scatter_E_pattern;
        if (lastPos.x == coordX && lastPos.y > coordY)
            return scatter_W_pattern;
        if (lastPos.x > coordX && lastPos.y < coordY)
            return scatter_NE_pattern;
        if (lastPos.x > coordX && lastPos.y == coordY)
            return scatter_N_pattern;
        if (lastPos.x > coordX && lastPos.y > coordY)
            return scatter_NW_pattern;
    }
    return getDefaultEffectsRanges();
}

bool Artoria::SpellActivationCheck() {
    evolvedForm();
    if (canEvolve()) {
        CNT_GodMove = 3;
        evolved = true;
        default_piece_move = queen_default_moves;
    }
    if (getLastKillTurn() == GameEngine::getInstance()->getTurnNumber())
        passive();
    if (CNT_GodMove == 0) {
        evolved = false;
        default_piece_move = king_default_moves;
    }
    return true;
}

bool Artoria::passive() {
    auto *  effect_instance = new EffectInstance(
        AOE,
        this,
        1,
        1,
        -1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    return true;
}

bool Artoria::canEvolve() {
    if (!evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()) {
        return true;
    }
    return false;

}

bool Artoria::evolvedForm() {
    if (evolved)
        CNT_GodMove--;
    return true;
}


