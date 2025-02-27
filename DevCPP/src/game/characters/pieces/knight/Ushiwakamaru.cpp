//
// Created by david on 31/01/2025.
//

#include "Ushiwakamaru.h"

#include "phase_context.h"
#include <GameEngine.h>

bool Ushiwakamaru::SpellActivationCheck(void *arg) {
    if (canEvolve(arg))
        evolved = true;
    if (evolved && hasCharged){
        passive(arg);
        evolvedForm(arg);
        return true;
    }
    if (!passive(arg))
        this->setIsOnAMove(false);
    return true;
}

bool Ushiwakamaru::passive(void* arg) {
    if (evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && !isOnAMove){
        if (!hasCharged){
            hasCharged = true;
            return true;
        }
    }
    hasCharged = false;
    return false;
}

bool Ushiwakamaru::canEvolve(void *arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && !context->mainTargetPiece->isPawn() && !context->mainTargetPiece->isRook() && !evolved) {
        return true;
    }
    return false;
}

bool Ushiwakamaru::evolvedForm(void *arg) {
    // EffectHandler::applyBuffToSelf(EffectInstance{ONE_MORE_MOVE,1,1,1});
    return true;
}
