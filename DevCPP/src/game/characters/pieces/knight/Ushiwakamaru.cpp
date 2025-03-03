//
// Created by david on 31/01/2025.
//

#include "Ushiwakamaru.h"

#include <EffectHandler.h>

#include "phase_context.h"
#include <GameEngine.h>

bool Ushiwakamaru::SpellActivationCheck() {
    if (!isOnAMove)
        remaining_cooldown--;
    if (canEvolve()) {
        evolved = true;
        return true;
    }
    if (evolved && gotKillAtTurn(GameEngine::getInstance()->getTurnNumber() - 1)) {
        if (remaining_cooldown > 0)
            return true;
        evolvedForm();
        remaining_cooldown = cooldown;
        return true;
    }
    return true;
}

bool Ushiwakamaru::canEvolve() {
    if (const auto * context = GameEngine::getInstance()->getCurrentPhaseContext();
        getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()
        && !context->mainTargetPiece->isPawn()
        && !context->mainTargetPiece->isRook()
        && !evolved
    ) return true;
    return false;
}

bool Ushiwakamaru::evolvedForm() {
    auto* effect_instance = new EffectInstance(
        ONE_MORE_MOVE,
        this,
        1,
        1,
        1
    );
    EffectHandler::applyBuffToSelf(effect_instance);
    return true;
}
