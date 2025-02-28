//
// Created by Megaport on 20/02/2025.
//

#include "Okita.h"

#include <EffectHandler.h>
#include <phase_context.h>
#include <GameEngine.h>

bool Okita::SpellActivationCheck() {
    passive();
    if (canEvolve())
        evolved = true;
    if (evolved && CNT_Charge != 0){
        CNT_Charge--;
        evolvedForm();
        return true;
    }
    if (CNT_Charge == 0)
        isOnAMove = false;
    return true;
}

bool Okita::passive() {
    if (const auto* context = GameEngine::getInstance()->getCurrentPhaseContext();
        !evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()
        && !isOnAMove
        && !context->mainTargetPiece->isPawn()
    ) CNT_Charge++;
    return true;
}

bool Okita::canEvolve() {
    if (const auto* context = GameEngine::getInstance()->getCurrentPhaseContext();
        getLastKillTurn() == GameEngine::getInstance()->getTurnNumber()
        && !evolved
        && (context->mainTargetPiece->isQueen() || context->mainTargetPiece->isRook())
    ) return true;
    return false;
}

bool Okita::evolvedForm() {
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