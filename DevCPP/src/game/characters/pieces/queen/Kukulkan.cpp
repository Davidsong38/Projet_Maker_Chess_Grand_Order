//
// Created by david on 03/03/2025.
//

#include "Kukulkan.h"

#include <EffectHandler.h>
#include <GameEngine.h>


bool Kukulkan::SpellActivationCheck() {
    passive();
    if (!evolved && static_cast<Event*>(events.back())->eventType == EVENT_KILL
        && getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && enableCNT) {
        CNT_Kill++;
        if (!GameEngine::getInstance()->getCurrentPhaseContext()->mainTargetPiece->isPawn()) {
            CNT_Figure++;
        }
    }
    enableCNT = true;
    if (canEvolve()) {
        evolved = true;
        CNT_Overcharge = 3;
        CNT_Figure = 0;
        CNT_Kill = 0;
        evolvedForm();
        return true;
    }
    if (CNT_Overcharge == 0 && evolved) {
        OMM_Chance = 20;
        evolved = false;
        enableCNT = false;
    }
    return true;
}

bool Kukulkan::passive() {
    if (getLastKillTurn() != GameEngine::getInstance()->getTurnNumber() || static_cast<Event*>(events.back())->eventType != EVENT_KILL)
        return true;
    if (const int chance = rand() % 100; chance >= OMM_Chance)
        return true;
    auto* effect_instance = new EffectInstance(
        ONE_MORE_MOVE,
        this,
        1,
        1,
        1
    );
    EffectHandler::applyBuffToSelf(effect_instance);
    if (CNT_Overcharge != 0)
        CNT_Overcharge--;
    return true;
}

bool Kukulkan::canEvolve() {
    if (!evolved && (CNT_Figure > 0 && CNT_Kill > 2
        || getLastKillTurn() == GameEngine::getInstance()->getTurnNumber() && GameEngine::getInstance()->getCurrentPhaseContext()->mainTargetPiece->isQueen()) )
        return true;
    return false;
}

bool Kukulkan::evolvedForm() {
    OMM_Chance = 60;
    return true;
}