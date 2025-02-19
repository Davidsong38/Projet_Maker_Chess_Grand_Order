//
// Created by Megaport on 19/02/2025.
//

#include "Merlin.h"

#include <Context.h>
#include <GameEngine.h>


void Merlin::setPieceGameMode(int piece_game_mode) {
    return;
}


vector<pair<int, int> > Merlin::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;

    if (effect == IMMUNITY_EFFECT && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (effect == IMMUNITY_AOE && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (chooseSpell){
        if (effect == IMMUNITY_EFFECT) {
            for (int i = 0; i < 7; ++i){
                for (int j = 0; j < 7; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
        if (effect == IMMUNITY_AOE) {
            for (int i = 0; i < 7; ++i){
                for (int j = 0; j < 7; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
    }
    return effect_range;
}

bool Merlin::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (hasJustKilled) {
        if (canEvolve(context) || evolved)
            evolvedForm(context);
        passive(context);
    }
    return true;
}


bool Merlin::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    EffectHandler::applyEffectToTargets(this,EffectInstance{IMMUNITY_EFFECT,-1,1,1});
    EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{IMMUNITY_AOE,-1,1,1},
        GameEngine::getInstance()->getLastPieceTouchedByEffect()->getCoordX(),GameEngine::getInstance()->getLastPieceTouchedByEffect()->getCoordY());
    return true;
}

bool Merlin::canEvolve(void *arg) {
    //std::cout <<CNT_StunEffect<<std::endl;
    if (evolved == false ) {
        //std::cout <<"Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Merlin::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    evolved = true;
    EffectHandler::applyEffectToTargets(this,EffectInstance{AOE,1,1,-1});
    return true;
}