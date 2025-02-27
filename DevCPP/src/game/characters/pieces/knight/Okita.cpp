//
// Created by Megaport on 20/02/2025.
//

#include "Okita.h"

#include <phase_context.h>
#include <GameEngine.h>

bool Okita::SpellActivationCheck(void *arg) {
    passive(arg);
    if (canEvolve(arg))
        evolved = true;
    if (evolved && CNT_Charge != 0){
        CNT_Charge--;
        evolvedForm(arg);
        return true;
    }
    if (CNT_Charge == 0)
        setIsOnAMove(false);
    return true;
}

bool Okita::passive(void* arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (!evolved && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && !isOnAMove && !context->mainTargetPiece->isPawn()){
        CNT_Charge++;
    }
    return true;
}

bool Okita::canEvolve(void *arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && !evolved &&(context->mainTargetPiece->isQueen() || context->mainTargetPiece->isRook())) {
        return true;
    }
    return false;
}

bool Okita::evolvedForm(void *arg) {
    // EffectHandler::applyBuffToSelf(this,EffectInstance{ONE_MORE_MOVE,1,1,1});
    return true;
}