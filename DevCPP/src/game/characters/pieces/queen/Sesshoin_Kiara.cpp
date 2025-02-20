//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"
#include <GameEngine.h>
#include "Context.h"


//vector<Effect_List> Sesshoin_Kiara::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}

void Sesshoin_Kiara::setPieceGameMode(int piece_game_mode) {
    pieceGameMode = piece_game_mode;
}


vector<pair<int, int> > Sesshoin_Kiara::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;
    if (effect == CHANGE_CONTROL_ADVANCE){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }

    if (effect == CHANGE_CONTROL) {
        for (int i = 1; i < 3; ++i) {
            if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
            if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
        }
    }
    return effect_range;
}

bool Sesshoin_Kiara::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (GameEngine::getInstance()->receivedRightClick){
        std::cout << "gekooo" << std::endl;
        if (this->getIsWhite()){
            GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
            GameEngine::getInstance()->setState(SELECT_WHITE_PHASE);
        }
        else{
            GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
            GameEngine::getInstance()->setState(SELECT_BLACK_PHASE);
        }
        return true;
    }
    if (this->getPieceGameMode() != 0){
        if (GameEngine::getInstance()->receivedClick){
            //std::cout << "ayayayayayayayayayayayayayayayayayay" << std::endl;
            if (canEvolve(context)){
                if (evolvedForm(context))
                    return true;
                return false;
            }
            if (passive(context))
                return true;
        }
        return false;
    }
    return true;
}


bool Sesshoin_Kiara::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{CHANGE_CONTROL,5,1,1})){
        CNT_Charm++;
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::canEvolve(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (evolved == false && CNT_Charm > 2) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;
}

bool Sesshoin_Kiara::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{CHANGE_CONTROL_ADVANCE,-1,1,1})){
        std::cout << "Special competence"<<std::endl;
        evolved = true;
        return true;
    }
    return false;
}