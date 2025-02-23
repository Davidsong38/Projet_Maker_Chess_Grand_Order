//
// Created by Megaport on 20/02/2025.
//

#include "Xu_Fu.h"

#include <Context.h>
#include <GameEngine.h>


void Xu_Fu::setPieceGameMode(int piece_game_mode) {
    pieceGameMode = piece_game_mode;
}


vector<pair<int, int> > Xu_Fu::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;
    if (effect == IMMORTALITY){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }

    if (effect == SHIELD) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }


    if (effect == SUPP_RANGE){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }
    return effect_range;
}

bool Xu_Fu::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (this->getPieceGameMode() != 0){
        if (!getIsOnAMove())
            passive(context);
        if (canEvolve(context)){
            setIsOnAMove(true);
            evolved = true;
        }
        if (GameEngine::getInstance()->receivedClick && evolved){
            //std::cout << "ayayayayayayayayayayayayayayayayayay" << std::endl;
            if (evolvedForm(context)){
                setIsOnAMove(false);
                return true;
            }
            return false;
        }

        if (evolved && getIsOnAMove()){
            return false;
        }
    }
    return true;
}


bool Xu_Fu::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    int chance = rand() % 100;
    if (chance < ShieldChance){
        if (EffectHandler::applyEffectToTargets(this,EffectInstance{SHIELD,-1,1,1,this})){
            CNT_Shield++;
        }
    }
    return true;
}

bool Xu_Fu::canEvolve(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (evolved == false && CNT_Shield == 2) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;
}

bool Xu_Fu::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (GameEngine::getInstance()->getLastClickX() == coordX && GameEngine::getInstance()->getLastClickY() == coordY){
        EffectHandler::applyBuffToSelf(this,EffectInstance{SHIELD,-1,2,1});
        return true;
    }
    if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{IMMORTALITY,-1,-1,1,this})){
        EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{SUPP_RANGE,-1,1,1,this});
        std::cout << "Special competence"<<std::endl;
        return true;
    }
    return false;
}